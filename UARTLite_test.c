/*
 * UartLite_test.c: simple test application for the UART Lite core
 *
 * This application does some testing of the UART Lite core and 
 * sends the entire english alphabet at 9600 Buad, 8 data bits, no parity,
*
 */

#include "platform.h"
#include "xstatus.h"
#include "xparameters.h"
#include "xuartlite.h"


#define UARTLITE_DEVICE_ID  XPAR_UARTLITE_0_DEVICE_ID
#define DATA_SIZE  26  // 26 letters in alphabet
#define BUFF_SIZE  16  // max size of FIFOs


int UARTLite_Init_SelfTest(u16 Device_ID);

int SendData(u8 *DataPointer, u8 *DataBufferPointer);


// Data to send
u8 TxData[DATA_SIZE];

// Tx/Rx buffers
u8 TxBuff[BUFF_SIZE];

// instance of UART Lite core
XUartLite UartLite0;


int main()
{
	int UartLiteStatus;
  int TxStatus;

  init_platform();
  
  UartLiteStatus = UARTLite_Init_SelfTest(UARTLITE_DEVICE_ID);

  // intialize buffer with alphabet data
  for(int i = 0; i < DATA_SIZE; i++)
  {
    TxData[i] = 65 + i;  // start at decimal 65 which is ASCII 'A'
  }
  
  TxStatus = SendData(TxData, TxBuff);

  cleanup_platform();
  return 0;
}


int UARTLite_Init_SelfTest(u16 Device_ID)
{
  int Status;
  
  // perform initialization tests
  Status = XUartLite_Initialize(&UartLite0, Device_ID);
  if (Status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }

  // perform self-test tests
  Status = XUartLite_SelfTest(&UartLite0);
  if (Status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }

  return XST_SUCCESS;
}

int SendData(u8 *DataPointer, u8 *DataBufferPointer)
{
  unsigned int TxBytesSent;
  
  // max data sent per function call is 16 bytes (size of FIFOs)
  while (TxBytesSent != DATA_SIZE)
  { 
    // put only 16 bytes of data into Tx_buff
    for(int i = 0; i < BUFF_SIZE; i++)
    {
      DataBufferPointer[i] = DataPointer[TxBytesSent+i];
    }

  // Send buffer
    TxBytesSent += XUartLite_Send(&UartLite0, DataBufferPointer, BUFF_SIZE);
  }
    
  // make sure all data was sent out
  if (TxBytesSent != DATA_SIZE)
  {
   return XST_FAILURE;
  }

  return XST_SUCCESS;
}  

