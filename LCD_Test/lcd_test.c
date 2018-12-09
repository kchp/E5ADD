/******************************************************************************
 * Test application for testing SPI connectivity and running a test of the
 * TFT-LCD display.
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "Delay.h"
#include "SPI.h"
#include "LCD_Driver.h"
#include "LCD_GUI.h"

/* Create instances for both GPIO and SPI */
extern XGpio gpio0;
extern XSpi  SpiInstance;

#define DELAY 	1000	// 1 second wait


int main()
{
	int Status;

    //Initialize MiniZed board
    init_platform();
    xil_printf("-------------------------------\n");
    xil_printf("-     MiniZed TFT LCD test    -\n");
    xil_printf("-------------------------------\n");

	/* Initialize AXI GPIO 0 */
	Status = XGpio_Initialize(&gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO 0 Initialization Failed\r\n");
		return XST_FAILURE;
	}
	xil_printf("----- GPIO 0 Initialized -----\n");

	// Initialize AXI SPI Controller
	Status = XSpi_Init(&SpiInstance,SPI_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Mode Failed\r\n");
		return XST_FAILURE;
	}
	xil_printf("----- SPI Initialized -----\n");

	xil_printf("----- Init LCD -----\n");
	LCD_SCAN_DIR LCD_ScanDir = SCAN_DIR_DFT; // Scan direction U2D_R2L
	LCD_Init(LCD_ScanDir);

	xil_printf("----- TFT LCD test -----\n");
	LCD_Clear(GUI_BACKGROUND);
	GUI_DrawRectangle(0,0,159,127,BLACK,DRAW_EMPTY,DOT_PIXEL_2X2);
	GUI_DisString_EN(10,30,"LCD Test",&Font20,GUI_BACKGROUND,CYAN);
	delay_ms(2000);	// 2 second wait

	LCD_Clear(GUI_BACKGROUND);
	GUI_DisString_EN(10,30,"Starting",&Font20,GUI_BACKGROUND,CYAN);
	GUI_DisString_EN(10,55,"test",&Font20,GUI_BACKGROUND,CYAN);
	delay_ms(DELAY);

	GUI_Show();
	delay_ms(DELAY);

	Test_Fonts();
	delay_ms(DELAY);

	Test_Background();
	delay_ms(DELAY);

	xil_printf("----- LCD test completed -----\n");
    return 0;
}
