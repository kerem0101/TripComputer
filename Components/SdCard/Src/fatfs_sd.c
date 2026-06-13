#include "fatfs_sd.h"
#include "stm32f0xx_hal.h"
#include <stdbool.h>

// SD Card types
#define CT_MMC   0x01
#define CT_SD1   0x02
#define CT_SD2   0x04
#define CT_SDC   (CT_SD1|CT_SD2)
#define CT_BLOCK 0x08

static volatile DSTATUS Stat = STA_NOINIT;
static BYTE CardType;

static void SPI_TxByte(BYTE data) {
    while(!__HAL_SPI_GET_FLAG(SD_SPI_HANDLE, SPI_FLAG_TXE));
    *(__IO uint8_t *)&SD_SPI_HANDLE->Instance->DR = data;
    while(!__HAL_SPI_GET_FLAG(SD_SPI_HANDLE, SPI_FLAG_RXNE));
    *(__IO uint8_t *)&SD_SPI_HANDLE->Instance->DR; // Read to clear RXNE
}

static BYTE SPI_RxByte(void) {
    BYTE dummy = 0xFF;
    while(!__HAL_SPI_GET_FLAG(SD_SPI_HANDLE, SPI_FLAG_TXE));
    *(__IO uint8_t *)&SD_SPI_HANDLE->Instance->DR = dummy;
    while(!__HAL_SPI_GET_FLAG(SD_SPI_HANDLE, SPI_FLAG_RXNE));
    return *(__IO uint8_t *)&SD_SPI_HANDLE->Instance->DR;
}

static void SELECT(void) {
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
}

static void DESELECT(void) {
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
    SPI_RxByte(); // Dummy clock
}

static BYTE SD_SendCmd(BYTE cmd, DWORD arg) {
    BYTE n, res;

    if (cmd & 0x80) {
        cmd &= 0x7F;
        res = SD_SendCmd(CMD55, 0);
        if (res > 1) return res;
    }

    DESELECT();
    SELECT();
    SPI_RxByte();

    SPI_TxByte(cmd);
    SPI_TxByte((BYTE)(arg >> 24));
    SPI_TxByte((BYTE)(arg >> 16));
    SPI_TxByte((BYTE)(arg >> 8));
    SPI_TxByte((BYTE)arg);

    n = 0x01; 
    if (cmd == CMD0) n = 0x95;
    if (cmd == CMD8) n = 0x87;
    SPI_TxByte(n);

    if (cmd == CMD12) SPI_RxByte();

    n = 10;
    do {
        res = SPI_RxByte();
    } while ((res & 0x80) && --n);

    return res;
}

static bool SD_WaitReady(void) {
    BYTE res;
    uint32_t tickstart = HAL_GetTick();
    do {
        res = SPI_RxByte();
    } while (res != 0xFF && (HAL_GetTick() - tickstart) < 500);
    return (res == 0xFF);
}

DSTATUS SD_disk_initialize (BYTE pdrv) {
    BYTE n, ty, cmd, ocr[4];
    if (pdrv) return STA_NOINIT;
    if (Stat & STA_NODISK) return Stat;

    DESELECT();
    for (n = 10; n; n--) SPI_RxByte();

    ty = 0;
    if (SD_SendCmd(CMD0, 0) == 1) {
        uint32_t tickstart = HAL_GetTick();
        if (SD_SendCmd(CMD8, 0x1AA) == 1) {
            for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
                while ((HAL_GetTick() - tickstart) < 1000 && SD_SendCmd(CMD41, 1UL << 30));
                if (SD_SendCmd(CMD58, 0) == 0) {
                    for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        } else {
            if (SD_SendCmd(CMD41, 0) <= 1) {
                ty = CT_SD1; cmd = CMD41;
            } else {
                ty = CT_MMC; cmd = CMD1;
            }
            while ((HAL_GetTick() - tickstart) < 1000 && SD_SendCmd(cmd, 0));
            if (SD_SendCmd(CMD16, 512) != 0) ty = 0;
        }
    }
    CardType = ty;
    DESELECT();

    if (ty) {
        Stat &= ~STA_NOINIT;
    } else {
        Stat = STA_NOINIT;
    }
    return Stat;
}

DSTATUS SD_disk_status (BYTE pdrv) {
    if (pdrv) return STA_NOINIT;
    return Stat;
}

DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (!(CardType & CT_BLOCK)) sector *= 512;

    if (count == 1) {
        if ((SD_SendCmd(CMD17, sector) == 0) && SD_WaitReady()) {
            while(SPI_RxByte() != 0xFE);
            for(int i=0; i<512; i++) buff[i] = SPI_RxByte();
            SPI_RxByte(); SPI_RxByte();
            DESELECT();
            return RES_OK;
        }
    } else {
        if (SD_SendCmd(CMD18, sector) == 0) {
            do {
                if(!SD_WaitReady()) break;
                while(SPI_RxByte() != 0xFE);
                for(int i=0; i<512; i++) *buff++ = SPI_RxByte();
                SPI_RxByte(); SPI_RxByte();
            } while (--count);
            SD_SendCmd(CMD12, 0);
            DESELECT();
            return count ? RES_ERROR : RES_OK;
        }
    }
    DESELECT();
    return RES_ERROR;
}

DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;
    if (!(CardType & CT_BLOCK)) sector *= 512;

    if (count == 1) {
        if ((SD_SendCmd(CMD24, sector) == 0) && SD_WaitReady()) {
            SPI_TxByte(0xFE);
            for(int i=0; i<512; i++) SPI_TxByte(buff[i]);
            SPI_TxByte(0xFF); SPI_TxByte(0xFF);
            if ((SPI_RxByte() & 0x1F) != 0x05) { DESELECT(); return RES_ERROR; }
            DESELECT();
            return RES_OK;
        }
    } else {
        if (CardType & CT_SDC) {
            SD_SendCmd(CMD55, 0);
        }
        SD_SendCmd(CMD23, count);
        if ((SD_SendCmd(CMD25, sector) == 0) && SD_WaitReady()) {
            do {
                SPI_TxByte(0xFC);
                for(int i=0; i<512; i++) SPI_TxByte(*buff++);
                SPI_TxByte(0xFF); SPI_TxByte(0xFF);
                if ((SPI_RxByte() & 0x1F) != 0x05) { DESELECT(); return RES_ERROR; }
                if(!SD_WaitReady()) break;
            } while (--count);
            SPI_TxByte(0xFD);
            if(!SD_WaitReady()) { DESELECT(); return RES_ERROR; }
            DESELECT();
            return count ? RES_ERROR : RES_OK;
        }
    }
    DESELECT();
    return RES_ERROR;
}

DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff) {
    DRESULT res = RES_ERROR;
    if (pdrv) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    SELECT();
    switch (cmd) {
    case CTRL_SYNC:
        if (SD_WaitReady()) res = RES_OK;
        break;
    case GET_SECTOR_COUNT:
        *(DWORD*)buff = 131072; // Dummy size
        res = RES_OK;
        break;
    case GET_BLOCK_SIZE:
        *(DWORD*)buff = 1;
        res = RES_OK;
        break;
    default:
        res = RES_PARERR;
    }
    DESELECT();
    return res;
}
