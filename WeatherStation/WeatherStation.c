/******************************************************************************
 * This is the main application for the project, it repuses most of the
 * code from the test applikations
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
#include "htu21d.h"

/* Create instances for both GPIO and SPI */
extern XGpio gpio0;
extern XSpi  SpiInstance;

#define DELAY 	30000	// 30 second wait


int main()
{
	int Status;
    htu21d_status stat;
    float temperature;
    float humidity;
	char tempbuffer[16];
	char humibuffer[16];

    //Initialize MiniZed board
    init_platform();
    xil_printf("---------------------------------------------------\n");
    xil_printf("-     MiniZed Temperature and Humidity monitor    -\n");
    xil_printf("---------------------------------------------------\n");

	/* Initialize AXI GPIO 0 */
	Status = XGpio_Initialize(&gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO 0 Initialization Failed\n");
		return XST_FAILURE;
	}

	// Initialize AXI SPI Controller
	Status = XSpi_Init(&SpiInstance,SPI_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Mode Failed\n");
		return XST_FAILURE;
	}

    // Initialize the AXI I2C interface
    htu21d_init(XPAR_AXI_IIC_0_BASEADDR);

	// Set relative humidity resolution to 12-bit and temperature to 14-bit
	stat = htu21d_set_resolution(htu21d_resolution_t_14b_rh_12b);

    // Initialize LCD display
	LCD_SCAN_DIR LCD_ScanDir = SCAN_DIR_DFT; // Scan direction U2D_R2L
	LCD_Init(LCD_ScanDir);

	// Start of program loop
    while(1){
		// Read temperature and humidity
		stat = htu21d_read_temperature_and_relative_humidity(&temperature, &humidity);

		// Format readings in a string for use with display string command
		sprintf(tempbuffer, "%2.2fC ", temperature);
		sprintf(humibuffer, "%2.2f%%", humidity);

		// Update LCD to display new reading
		LCD_Clear(BLACK);
		GUI_DrawRectangle(0,0,159,127,BLACK,DRAW_EMPTY,DOT_PIXEL_2X2);

		GUI_DisString_EN(10,10,"Temperatur",&Font20,GUI_BACKGROUND,CYAN);
		GUI_DisString_EN(10,40,tempbuffer,&Font24,GUI_BACKGROUND,YELLOW);

		GUI_DisString_EN(10,70,"Fugtighed",&Font20,GUI_BACKGROUND,CYAN);
		GUI_DisString_EN(10,100,humibuffer,&Font24,GUI_BACKGROUND,YELLOW);

		delay_ms(DELAY);
    }

    return 0;

}
