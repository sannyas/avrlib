#include <avr/io.h>
#include <stdint.h>
#include <usart.h>
#include <util/delay.h>
#include "../nrf24l01.h"

uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
/* ------------------------------------------------------------------------- */
int main()
{
    usart_init();
    stdout = &usart_file;


    printf("\n> TX device ready\n");
    
    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(2,4);

    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);    

    q = 'A';
    while(1)
    {                
        /* Fill the data buffer */
        data_array[0] = '-';
        data_array[1] = '>';
        data_array[2] = q++;
        data_array[3] = 0;                                    
	if( q > 'Z' )
		q = 'A';

        /* Automatically goes to TX mode */
        nrf24_send(data_array);        
        
        /* Wait for transmission to end */
        while(nrf24_isSending());

        /* Make analysis on last tranmission attempt */
        temp = nrf24_lastMessageStatus();

        if(temp == NRF24_TRANSMISSON_OK)
        {                    
            printf("> Tranmission went OK\n");
        }
        else if(temp == NRF24_MESSAGE_LOST)
        {                    
            printf("> Message is lost ...\n");    
        }
        
	/* Retranmission count indicates the tranmission quality */
	temp = nrf24_retransmissionCount();
	printf("> Retranmission count: %d\n",temp);

	/* Optionally, go back to RX mode ... */
	nrf24_powerUpRx();

	/* Or you might want to power down after TX */
	// nrf24_powerDown();            

	/* Wait a little ... */
	_delay_ms(1000);
    }
}
/* ------------------------------------------------------------------------- */
