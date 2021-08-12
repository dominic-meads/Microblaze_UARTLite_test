/*
 * UartLite_test.c: simple test application for the UART Lite core
 *
 * This application does some testing of the UART Lite core configured
 * with baud rate 9600, 8 data bits, no parity.
*
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xuartlite.h"


int main()
{
	int UartLite_status;

	// Tx/Rx buffers
	u8 Tx_buff[16];

	int Index;

	// Tx/Rx byte counts
	unsigned int Tx_bytes_sent;

	// instance of UART Lite core
	XUartLite UartLite0;

    init_platform();

    // perform initialization tests
    UartLite_status = XUartLite_Initialize(&UartLite0, XPAR_UARTLITE_0_DEVICE_ID);
    if (UartLite_status != XST_SUCCESS)
    {
    	return XST_FAILURE;
    }

    // perform self-test tests
    UartLite_status = XUartLite_SelfTest(&UartLite0);
    if (UartLite_status != XST_SUCCESS)
    {
    	return XST_FAILURE;
    }

	for (Index = 0; Index < 16; Index++) {
		Tx_buff[Index] = Index;
	}

    // send some data
    // since not in interrupt mode, max data sent per function call is 16 bytes (size of FIFOs)
    Tx_bytes_sent = XUartLite_Send(&UartLite0, Tx_buff, 16);



    cleanup_platform();
    return 0;
}

