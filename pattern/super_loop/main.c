#include <stdio.h>
#include <usart.h>
#include "task1.h"
#include "task2.h"


/***************************
 * main function
 **************************/
int main( void ){
	// Initialize module
	usart_init();
	stdout = &usart_file;

	task1_init();
	task2_init();
	

	// super loop
	while( 1 ){
		task1_do();
		task2_do();
	}
	return 0;
}

