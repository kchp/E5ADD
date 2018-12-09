/******************************************************************************
 * Test application for testing I2C connectivity and running a test of the
 * Si7021 sensor module.
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include "delay.h"
#include "htu21d.h"

int main()
{
	int i = 0;			// Used for while loop
    htu21d_status stat;	// For reply from the sensor commands
    float temperature;
    float humidity;

    //Initialize MiniZed board
    init_platform();
    xil_printf("----------------------------------\n");
    xil_printf("-     MiniZed I2C sensor test    -\n");
    xil_printf("----------------------------------\n");

    // Initialize the AXI I2C interface
    xil_printf("Initializing I2C\n");
    htu21d_init(XPAR_AXI_IIC_0_BASEADDR);

	// Set relative humidity resolution to 12-bit and temperature to 14-bit
	xil_printf("Setting HTU21D Resolution to maximum\n");
	xil_printf("14-bit Temperature\n");
	xil_printf("12-bit Relative Humidity\n");
	stat = htu21d_set_resolution(htu21d_resolution_t_14b_rh_12b);

	// Start of test routine
	xil_printf("\nTest routine will make 5 readings from the sensor\n");
	xil_printf("at 5 second intervals\n");

    while(i < 5){
		// Read temperature and humidity
		 xil_printf("\n--- Reading temperature and humidity ---\n");
		 stat = htu21d_read_temperature_and_relative_humidity(&temperature, &humidity);

		 // If the operation successfully read the temperature and humidity, these will be displayed
		 // if unsuccessful the reason for the error will be displayed
		 printf("Read Temperature and humidity, completed with status: ");
		 if(stat==htu21d_status_ok){
			 printf("Ok.\n");
			 printf("Temperature : %5.2f°C, \tRelative Humidity : %4.1f%%\n",temperature,humidity);
		 }else if(stat==htu21d_status_i2c_transfer_error){
			 printf("Transfer Error.\n");
		 }else if(stat==htu21d_status_crc_error){
			printf("CRC Error.\n");
		 }

		i++;
		delay_ms(5000);
    }

    printf("\nTest completed\n");

    return 0;
}
