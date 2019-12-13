#include "main.h"
#include "string.h"

extern uint8_t RxBuffer[12];
extern UART_HandleTypeDef huart1;

uint32_t writeFlashData = 0x55555555;
uint32_t addr = 0x08007000;


/*************************
Flash写入流程:
1.解锁FLASH
2.擦除FLASH
3.写入数据到FLASH
4.锁住FLASH
**************************/
void writeFlash(void)
{
		HAL_FLASH_Unlock();
	
		FLASH_EraseInitTypeDef f;
		f.TypeErase = FLASH_TYPEERASE_PAGES;
		f.PageAddress = addr;
		f.NbPages = 1;
		uint32_t pageError = 0;
		HAL_FLASHEx_Erase(&f, &pageError);
	
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, writeFlashData);
	
		HAL_FLASH_Lock();
}

void readFlash(void)
{
		uint32_t val = *(__IO uint32_t*)(addr);
		printf("addr:0x%x, data:0x%x\r\n",addr, val);
}

void flashTest()
{
	readFlash();
}
int hex2Int(uint8_t *hexStr)
{
	#pragma optimize(0)
	volatile int rv = 0;
	uint8_t len = strlen(hexStr);
	for(;len > 0; len --)
	{
		rv *= 16;
		volatile uint8_t hex = *hexStr++;
		hex = (hex >= '0' && hex <= '9')?(hex -= '0'): (hex -= 'A');
		rv += hex ;
	}
	return rv;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart1)
{
	
	printf("rx:%s",RxBuffer);		
	addr = hex2Int(&RxBuffer[2]); 
	if(RxBuffer[0] == 'R')//R:0x08007000
		readFlash();
	if(RxBuffer[0] == 'W')//W:0x08007000
		writeFlash();
	startRx();
	
}