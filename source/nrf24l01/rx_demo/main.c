#include <avr/io.h>
#include <stdint.h>
#include <usart.h>

#include "../nrf24l01.h"


uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
/* ------------------------------------------------------------------------- */
int main()
{
	usart_init();
	stdout = &usart_file;

	/* simple greeting message */
	printf("\r\n> RX device ready\r\n");

	/* init hardware pins */
	nrf24_init();
    
	/* Channel #2 , payload length: 4 */
	nrf24_config(2,4);
 
	/* Set the device addresses */
	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);

	while(1)
	{    
		if(nrf24_dataReady())
		{
			nrf24_getData(data_array);        
			printf("> ");
			printf("%2X ",data_array[0]);
			printf("%2X ",data_array[1]);
			printf("%2X ",data_array[2]);
			printf("%2X\r\n",data_array[3]);
		}
	}
}
/* ------------------------------------------------------------------------- */
