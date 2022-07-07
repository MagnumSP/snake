#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define uint8	unsigned char
#define uint32	unsigned int

#define BOARD_HIGH_MIN        3
#define BOARD_WIDTH_MIN       3
#define BOARD_HIGH_MAX        99
#define BOARD_WIDTH_MAX       99
#define MAX_SNAKE_LENGTH      (BOARD_HIGH_MAX * BOARD_WIDTH_MAX)

#define ALIVE        (0)
#define DIE          (-1)

typedef enum
{
    NONE,
    HEAD,
    BODY,
    FOOD,
    WALL,
    DEAD,
    EAT,
} board_t;

typedef enum
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
} direct_t;

typedef struct
{
    uint32 x;
    uint32 y;
} body_point;

typedef struct
{
    body_point    body[MAX_SNAKE_LENGTH];
    body_point    trace;
    uint32        length;
    uint32        speed;
    direct_t      direction_now;
} snake_str;

static snake_str snake;
static board_t board[BOARD_WIDTH_MAX+2][BOARD_HIGH_MAX+2];
static uint32 score = 0;
int board_width, board_high;

void put_food()
{
    int a,b;

    do {
        a = rand() % (board_width+1);
        b = rand() % (board_high+1);
    } while (NONE != board[a][b]);
    board[a][b] = FOOD;
}

void snake_init()
{
    snake.body[0].x = 2;
    snake.body[0].y = 1;
    snake.body[1].x = 1;
    snake.body[1].y = 1;
    snake.length = 2;
    snake.speed = 5;
    snake.direction_now = RIGHT;
}

void board_init(uint32 width, uint32 high)
{
    int i;

    memset(board, NONE, sizeof(board));
    for(i=0;i<width+2;i++)
        board[i][0] = WALL;
    for(i=0;i<width+2;i++)
        board[i][high+1] = WALL;
    for(i=0;i<high+2;i++)
        board[0][i] = WALL;
    for(i=0;i<high+2;i++)
        board[width+1][i] = WALL;

    board[snake.body[0].x][snake.body[0].y] = HEAD;
    board[snake.body[1].x][snake.body[1].y] = BODY;
    put_food();
}

void body_move(void)
{
    int i;

    for (i=snake.length;i>0;i--)
        snake.body[i] = snake.body[i-1];

    if(snake.length < board_width * board_high)
        snake.trace = snake.body[snake.length];
}

void head_move(void)
{
    switch (snake.direction_now) {
        case UP:
            snake.body[0].y--;
            break;
        case DOWN:
            snake.body[0].y++;
            break;
        case LEFT:
            snake.body[0].x--;
            break;
        case RIGHT:
            snake.body[0].x++;
            break;
    }
}

void snake_move()
{
    body_move();
    head_move();
}

void longer(void)
{
    snake.body[snake.length] = snake.trace;
    snake.length++;
}

int board_update()
{
    snake_move();

    if (FOOD != board[snake.trace.x][snake.trace.y])
        board[snake.trace.x][snake.trace.y] = NONE;

    if (FOOD == board[snake.body[0].x][snake.body[0].y]) {
        longer();
        score += snake.speed;
        if (snake.length < board_width * board_high) {
            put_food();
        } else {
            board[snake.trace.x][snake.trace.y] = BODY;
        }

        board[snake.body[0].x][snake.body[0].y] = EAT;
        board[snake.body[1].x][snake.body[1].y] = BODY;
        return ALIVE;
    } else if(BODY == board[snake.body[0].x][snake.body[0].y] ||
		      WALL == board[snake.body[0].x][snake.body[0].y]) {
        board[snake.body[0].x][snake.body[0].y] = DEAD;
        board[snake.body[1].x][snake.body[1].y] = BODY;
        return DIE;
    }

    board[snake.body[0].x][snake.body[0].y] = HEAD;
    board[snake.body[1].x][snake.body[1].y] = BODY;
    return ALIVE;
}

direct_t buf_to_direct(char buf)
{
    switch(buf) {
        case 'w':
            return UP;
        case 'd':
            return RIGHT;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
    }
}

void turn(direct_t direction)
{
    if(direction == snake.direction_now) {
        if(10 == snake.speed)
            return;
        snake.speed++;
    } else if(2 == direction+snake.direction_now ||
              4 == direction+snake.direction_now) {
        if(1 == snake.speed)
            return;
        snake.speed--;
    } else {
        snake.direction_now = direction;
    }
}

void speed_keep(uint32 speed)
{
    int i, j;

    for(i = 0; i < 100000; i++)
        for(j = 0; j < (11-speed)*1000; j++);
}

void display(void)
{
    int i, j;

    system("clear");
    printf("------------------------------------------\n");
    printf("Score: %d   Speed: %d  Length: %d\n", score, snake.speed, snake.length);
    for(j = 0; j<board_high+2; j++) {
        for(i = 0; i<board_width+2; i++) {
            switch(board[i][j]) {
                case NONE:
                    printf("  ");
                    break;
                case HEAD:
                    printf("0 ");
                    break;
                case BODY:
                    printf("o ");
                    break;
                case FOOD:
                    printf("* ");
                    break;
                case WALL:
                    printf("# ");
                    break;
                case DEAD:
                    printf("X ");
                    break;
                case EAT:
                    printf("@ ");
                    break;
            }
        }
        printf("\n");
    }
}

void start_info_print()
{
    printf("##################################################################\n");
    printf("##                      Magnum Presents.                        ##\n");
    printf("##                                                              ##\n");
    printf("## 'w'-up, 's'-down, 'a'-left, 'd'-right.                       ##\n");
    printf("## 'q'-Pause.                                                   ##\n");
    printf("## Press front key to speed up, press offsite key to slow down. ##\n");
    printf("##################################################################\n");
}

int main(void)
{
    int flag,ret;
    char buf;
    uint8 direction;
    int is_alive;

    //将键盘设置为阻塞
    flag &= ~O_NONBLOCK;
    fcntl(0,F_SETFL,flag);

    start_info_print();
    do {
        printf("Set the board width (%d~%d): ", BOARD_WIDTH_MIN, BOARD_WIDTH_MAX);
        scanf("%d", &board_width);
        printf("Set the board high (%d~%d): ", BOARD_HIGH_MIN, BOARD_HIGH_MAX);
        scanf("%d", &board_high);
    } while(board_width > BOARD_WIDTH_MAX ||
            board_high > BOARD_HIGH_MAX ||
            board_width < BOARD_WIDTH_MIN ||
            board_high < BOARD_HIGH_MIN);

    system("stty -icanon"); //关闭缓冲区，输入字符无需回车直接接受
    system("stty -echo"); //取消输入字符时的显示
    printf("\nPress any button to start…\n");
    //将键盘设置为阻塞
    flag &= ~O_NONBLOCK;
    fcntl(0,F_SETFL,flag);
    while(1) {
        read(0,&buf,1);
        flag |= O_NONBLOCK;
        fcntl(0,F_SETFL,flag);
        break;
    }
START:
    //将键盘设置为非阻塞
    flag = fcntl(0,F_GETFL);
    flag |=O_NONBLOCK;
    fcntl(0,F_SETFL,flag);
    score = 0;
    snake_init();
    board_init(board_width, board_high);
    display();
    while(1) {
        buf = 0;
        read(0,&buf,1);
        if(buf=='w' || buf=='a' || buf=='s' || buf=='d')
            turn(buf_to_direct(buf));
        else if(buf=='q') {
            printf("-------------Pause------------\n");
            printf("Press any button to continue…\n");
            //将键盘设置为阻塞
            flag &= ~O_NONBLOCK;
            fcntl(0,F_SETFL,flag);
            while(1) {
                read(0,&buf,1);
                flag |= O_NONBLOCK;
                fcntl(0,F_SETFL,flag);
                break;
            }
        }
        is_alive = board_update();
        display();
        speed_keep(snake.speed);
        if(DIE == is_alive) {
            printf("--------------Dead-------------\n");
            printf("Score: %d!\n", score);
            printf("Play again? (y/n)\n");
            //将键盘设置为阻塞
            flag &= ~O_NONBLOCK;
            fcntl(0,F_SETFL,flag);
            while(1) {
                read(0,&buf,1);
                if('n' == buf) {
                    printf("\nBye~\n");
                    break;
                } else if('y' == buf)
                    goto START;
            }
            break;
        }
        if(board_width * board_high == snake.length) {
            printf("-----卧槽牛逼!!!-----");
            printf("Score: %d!\n", score);
            printf("Play again? (y/n)\n");
            //将键盘设置为阻塞
            flag &= ~O_NONBLOCK;
            fcntl(0,F_SETFL,flag);
            while(1) {
                read(0,&buf,1);
                if('n' == buf) {
                    printf("\nBye~\n");
                    break;
                } else if('y' == buf) {
                    goto START;
                }
            }
            break;
        }
    }
    //恢复
    system("stty icanon");
    system("stty echo");

    return 0;
}

