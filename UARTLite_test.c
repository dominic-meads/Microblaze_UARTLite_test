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

	// instance of UART Lite core
	XUartLite UartLite0;

    init_platform();

    // perform initialization tests
    UartLite_status = XUartLite_Initialize(&UartLite0, XPAR_UARTLITE_0_DEVICE_ID);
    if (XST_SUCCESS == UartLite_status)
    {
    	print("UartLite initialization PASS\n\r");
    }
    else{
    	print("UartLite initialization FAIL\n\r");
    }
    while(XUartLite_IsSending(&UartLite0) == 1);  // wait until all characters read out so don't overwrite terminal

    // perform self-test tests
    UartLite_status = XUartLite_SelfTest(&UartLite0);
    if (XST_SUCCESS == UartLite_status)
    {
    	print("UartLite self-test PASS\n\r");
    }
    else{
    	print("UartLite self-test FAIL\n\r");
    }
    while(XUartLite_IsSending(&UartLite0) == 1);

    print("Hello World\n\r");
    print("Successfully ran Hello World application\n\r");
    cleanup_platform();
    return 0;
}
