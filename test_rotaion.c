#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long  UINT64;
typedef unsigned int      	UINT32;
typedef unsigned short      USHORT16;
typedef unsigned char       UCHAR8;
typedef void       			VOID;

#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define WIDTH	(10)
#define HEIGHT  (8)
#define BIT8    (8)

typedef enum {
    OSD_PIXEL_FORMAT_RGBA8888 = 0,
    OSD_PIXEL_FORMAT_RGBA4444,
    OSD_PIXEL_FORMAT_RGB565,
    OSD_PIXEL_FORMAT_Alpha8,
    OSD_PIXEL_FORMAT_Alpha4,
    OSD_PIXEL_FORMAT_RGB888,
    OSD_PIXEL_FORMAT_MAX,
} OsdPixelFormat;

typedef enum {
    OSD_ROTATION_90,
    OSD_ROTATION_180,
    OSD_ROTATION_270,
} OsdRotationType;

VOID Hv_Drv_OsdRotateBuffer(UCHAR8* pucBuf, USHORT16 usWidth, USHORT16 usHeight, USHORT16 usPixelBit, OsdRotationType enRotate)
{
    USHORT16 usX = 0, usY = 0, usByte = 0, usIdSrc = 0, usIdDst = 0;
    USHORT16 usPixelWidth = 0;
    UCHAR8* pucTempBuf = NULL;
    UCHAR8 ucTemp = 0;
    USHORT16 usBufLen = 0;

    if (!pucBuf || !usWidth || !usHeight)
    {
        return;
    }

    usBufLen = usWidth * usHeight * usPixelBit / 8;
    pucTempBuf = malloc(usBufLen);
    if (!pucTempBuf)
    {
        return;
    }
    memset(pucTempBuf, 0, usBufLen);

    usPixelWidth = usPixelBit / 8;
    switch (enRotate)
    {
        case OSD_ROTATION_90:
            if (usPixelBit == 4)
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) / 2;
                        if ((usY * usWidth + usX) % 2)
                        {
                            ucTemp = pucBuf[usIdSrc] & 0x0f;
                        }
                        else
                        {
                            ucTemp = (pucBuf[usIdSrc] >> 4) & 0x0f;
                        }

                        usIdDst = (usX * usHeight + (usHeight - usY - 1)) / 2;
                        if ((usX * usHeight + (usHeight - usY - 1)) % 2)
                        {
                            pucTempBuf[usIdDst] |= ucTemp;
                        }
                        else
                        {
                            pucTempBuf[usIdDst] |= ucTemp << 4;
                        }
                    }
                }
            }
            else
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) * usPixelWidth;
                        usIdDst = (usX * usHeight + (usHeight - usY - 1)) * usPixelWidth;
                        for (usByte = 0; usByte < usPixelWidth; usByte++)
                        {
                            pucTempBuf[usIdDst + usByte] = pucBuf[usIdSrc + usByte];
                        }
                    }
                }
            }
            break;
        case OSD_ROTATION_180:
            if (usPixelBit == 4)
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) / 2;
                        if ((usY * usWidth + usX) % 2)
                        {
                            ucTemp = pucBuf[usIdSrc] & 0x0f;
                        }
                        else
                        {
                            ucTemp = (pucBuf[usIdSrc] >> 4) & 0x0f;
                        }

                        usIdDst = ((usHeight - usY - 1) * usWidth + (usWidth - usX - 1)) / 2;
                        if (((usHeight - usY - 1) * usWidth + (usWidth - usX - 1)) % 2)
                        {
                            pucTempBuf[usIdDst] |= ucTemp;
                        }
                        else
                        {
                            pucTempBuf[usIdDst] |= ucTemp << 4;
                        }
                    }
                }
            }
            else
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) * usPixelWidth;
                        usIdDst = ((usHeight - usY - 1) * usWidth + (usWidth - usX - 1)) * usPixelWidth;
                        for (usByte = 0; usByte < usPixelWidth; usByte++)
                        {
                            pucTempBuf[usIdDst + usByte] = pucBuf[usIdSrc + usByte];
                        }
                    }
                }
            }
            break;
        case OSD_ROTATION_270:
            if (usPixelBit == 4)
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) / 2;
                        if ((usY * usWidth + usX) % 2)
                        {
                            ucTemp = pucBuf[usIdSrc] & 0x0f;
                        }
                        else
                        {
                            ucTemp = (pucBuf[usIdSrc] >> 4) & 0x0f;
                        }

                        usIdDst = ((usWidth - usX - 1) * usHeight + usY) / 2;
                        if (((usWidth - usX - 1) * usHeight + usY) % 2)
                        {
                            pucTempBuf[usIdDst] |= ucTemp;
                        }
                        else
                        {
                            pucTempBuf[usIdDst] |= ucTemp << 4;
                        }
                    }
                }
            }
            else
            {
                for (usY = 0; usY < usHeight; usY++)
                {
                    for (usX = 0; usX < usWidth; usX++)
                    {
                        usIdSrc = (usY * usWidth + usX) * usPixelWidth;
                        usIdDst = ((usWidth - usX - 1) * usHeight + usY) * usPixelWidth;
                        for (usByte = 0; usByte < usPixelWidth; usByte++)
                        {
                            pucTempBuf[usIdDst + usByte] = pucBuf[usIdSrc + usByte];
                        }
                    }
                }
            }
            break;
        default:
            free(pucTempBuf);
            return;
    }
    memcpy(pucBuf, pucTempBuf, usBufLen);    //DMA
    free(pucTempBuf);
}

VOID Hv_Drv_OsdRotateGetPart(UCHAR8* pucSrc, UCHAR8* pucPart, UINT32 uiSrcWidth, UINT32 uiSrcHeight,
    UINT32 uiStartX, UINT32 uiStartY, UINT32 uiWidth, UINT32 uiHeight, USHORT16 usPixelBit)
{
    UCHAR8* pucPartAddr, * pucSrcAddr = pucSrc + (uiStartY * uiSrcWidth + uiStartX) * usPixelBit / 8;
    UINT32 uiSrcStride = uiSrcWidth * usPixelBit / 8;
    UINT32 uiDstStride = uiWidth * usPixelBit / 8;
    UINT32 uiLength = MIN(uiWidth, (uiSrcWidth - uiStartX)) * usPixelBit / 8;
    UINT32 uiSrcTotalLength = uiSrcWidth * uiSrcHeight * usPixelBit / 8;

    for (UINT32 uiY = 0; uiY < uiHeight; uiY++)
    {
        if (pucSrcAddr >= pucSrc + uiSrcTotalLength)
            break;
        pucPartAddr = pucPart + uiY * uiDstStride;
        memcpy((VOID*)pucPartAddr, (VOID*)pucSrcAddr, uiLength);
        memset((VOID*)pucSrcAddr, 0, uiLength);
        pucSrcAddr += uiSrcStride;
    }
}

VOID Hv_Drv_OsdRotateSetPart(UCHAR8* pucPart, UCHAR8* pucDst, UINT32 uiDstWidth, UINT32 uiDstHeight,
    UINT32 uiStartX, UINT32 uiStartY, UINT32 uiWidth, UINT32 uiHeight, USHORT16 usPixelBit)
{
    UCHAR8* pucPartAddr, * pucDstAddr = pucDst + (uiStartY * uiDstWidth + uiStartX) * usPixelBit / 8;
    UINT32 uiDstStride = uiDstWidth * usPixelBit / 8;
    UINT32 uiSrcStride = uiWidth * usPixelBit / 8;
    UINT32 uiLength = MIN(uiWidth, (uiDstWidth - uiStartX)) * usPixelBit / 8;
    UINT32 uiDstTotalLength = uiDstWidth * uiDstHeight * usPixelBit / 8;

    for (UINT32 uiY = 0; uiY < uiHeight; uiY++)
    {
        if (pucDstAddr >= pucDst + uiDstTotalLength)
            break;
        pucPartAddr = pucPart + uiY * uiSrcStride;
        memcpy((VOID*)pucDstAddr, (VOID*)pucPartAddr, uiLength);
        pucDstAddr += uiDstStride;
    }
}

VOID Hv_Drv_OsdRotationPart(UCHAR8* pucStartAddr, UINT32 uiTotalWidth, UINT32 uiTotalHeight, UINT32 uiSrcX, UINT32 uiSrcY,
    UINT32 uiWidth, UINT32 uiHeight, UINT32 uiDstX, UINT32 uiDstY, OsdPixelFormat enFormat, OsdRotationType enRotate)
{
    UCHAR8* pucPartBuf = NULL;
    UINT32 uiBufLen = 0;
    USHORT16 usPixelBit = 0;

    if (!pucStartAddr || !uiWidth || !uiHeight || !uiTotalWidth || !uiTotalHeight)
    {
        return;
    }

    usPixelBit = 8; //Hv_Cal_Osd_GetPixelBits(enFormat);

    uiBufLen = uiWidth * uiHeight * usPixelBit / 8;
    pucPartBuf = malloc(uiBufLen);
    if (!pucPartBuf)
    {
        return;
    }

    Hv_Drv_OsdRotateGetPart(pucStartAddr, pucPartBuf, uiTotalWidth, uiTotalHeight, uiSrcX, uiSrcY, uiWidth, uiHeight, usPixelBit);

    Hv_Drv_OsdRotateBuffer(pucPartBuf, uiWidth, uiHeight, usPixelBit, enRotate);

    if ((enRotate == OSD_ROTATION_90) || (enRotate == OSD_ROTATION_270))
        Hv_Drv_OsdRotateSetPart(pucPartBuf, pucStartAddr, uiTotalWidth, uiTotalHeight, uiDstX, uiDstY, uiHeight, uiWidth, usPixelBit);
    else if (enRotate == OSD_ROTATION_180)
        Hv_Drv_OsdRotateSetPart(pucPartBuf, pucStartAddr, uiTotalWidth, uiTotalHeight, uiDstX, uiDstY, uiWidth, uiHeight, usPixelBit);

    free(pucPartBuf);
}

void print_buf(UCHAR8 *pucBuf, USHORT16 usWidth, USHORT16 usHeight, USHORT16 usPixelBit, int bInit)
{
    USHORT16 usX, usY, usByte;
    USHORT16 usId;

    printf("\n");
    for (usY = 0; usY < usHeight; usY++)
    {
        for (usX = 0; usX < usWidth; usX++)
        {
            usId = (usY * usWidth + usX) * usPixelBit / 8;
            //printf("[");
            for (usByte = 0; usByte < usPixelBit / 8; usByte++)
            {
#if 0
				if (bInit)
				{
					pucBuf[usId + usByte] = usId + usByte;
				}
				printf("%02x", pucBuf[usId] / (usPixelBit / 8));
#else
				if (pucBuf[usId + usByte])
					printf("* ");
				else
					printf("  ");
#endif
            }
            //printf("]");
		}
	printf("\n");
    }
	printf("====================\n");
}

void main(void)
{
    UCHAR8 pucBuf[WIDTH * HEIGHT] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	};
    print_buf(pucBuf, WIDTH, HEIGHT, BIT8, 0);
#if 0
    while(1) {
    	getchar();
    	Hv_Drv_OsdRotateBuffer(pucBuf, WIDTH, 8, BIT8, OSD_ROTATION_270);
    	print_buf(pucBuf, 8, WIDTH, BIT8, 0);
    	getchar();
    	Hv_Drv_OsdRotateBuffer(pucBuf, 8, 10, BIT8, OSD_ROTATION_270);
    	print_buf(pucBuf, WIDTH, 8, BIT8, 0);
    }
#else
	UINT32 width = 2, height = 4;
    while(1) {
    	getchar();
		Hv_Drv_OsdRotationPart(pucBuf, WIDTH, HEIGHT, 6, 4, width, height, 6, 3, OSD_PIXEL_FORMAT_RGB565, OSD_ROTATION_270);
    	print_buf(pucBuf, WIDTH, HEIGHT, BIT8, 0);
    	getchar();
		Hv_Drv_OsdRotationPart(pucBuf, WIDTH, HEIGHT, 6, 3, height, width, 5, 1, OSD_PIXEL_FORMAT_RGB565, OSD_ROTATION_270);
    	print_buf(pucBuf, WIDTH, HEIGHT, BIT8, 0);
    	getchar();
		Hv_Drv_OsdRotationPart(pucBuf, WIDTH, HEIGHT, 5, 1, width, height, 3, 4, OSD_PIXEL_FORMAT_RGB565, OSD_ROTATION_270);
    	print_buf(pucBuf, WIDTH, HEIGHT, BIT8, 0);
    	getchar();
		Hv_Drv_OsdRotationPart(pucBuf, WIDTH, HEIGHT, 3, 4, height, width, 6, 4, OSD_PIXEL_FORMAT_RGB565, OSD_ROTATION_270);
    	print_buf(pucBuf, WIDTH, HEIGHT, BIT8, 0);
    }
#endif
}
