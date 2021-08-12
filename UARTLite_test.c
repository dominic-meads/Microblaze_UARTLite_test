/******************************************************************************/
/**
*
* File: UARTLite_test.c
*
* Description: example of minimal Tx testing of the UART Lite core v2.0, and sends   
*              the entire english alphabet at 9600 Buad, 8 data bits, no parity,
*
* Notes:
*
*
* MODIFICATION HISTORY:
* <pre>
* Ver   Who  Date	 Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a DM   8/12/21  First release
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "platform.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "xparameters.h"
#include "xuartlite.h"

/************************** Constant Definitions *****************************/

#define UARTLITE_DEVICE_ID  XPAR_UARTLITE_0_DEVICE_ID

// 26 letters in alphabet
#define TX_DATA_SIZE  26  

// max size in non-interrput mode
#define TX_BUFF_SIZE  16  

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int UARTLite_Init_SelfTest(u16 DeviceID);

int SendData(u8 *TxDataPointer, u8 *TxDataBufferPointer);

/************************** Variable Definitions *****************************/

// Data to send
u8 TxData[TX_DATA_SIZE];

// Tx buffers
u8 TxBuff[TX_BUFF_SIZE];

// instance of UART Lite core
XUartLite UartLite0;


/*****************************************************************************/
/**
*
* Description: main function to test out the UART Lite
*
*
* Arguments:
*
*
* Returns: XST_SUCCESS if successful, otherwise XST_FAILURE.
*
*
* Notes: 		
*
******************************************************************************/
int main()
{
	int UartLiteStatus;
  int TxStatus;

  init_platform();
  
  UartLiteStatus = UARTLite_Init_SelfTest(UARTLITE_DEVICE_ID);

  // intialize buffer with alphabet data
  for(int i = 0; i < TX_DATA_SIZE; i++)
  {
    TxData[i] = 65 + i;  // start at decimal 65 which is ASCII 'A'
  }
  
  TxStatus = SendData(TxData, TxBuff);

  xil_printf("UART Lite Tx test successfull\n\r");
  cleanup_platform();
  return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* Description: Initializes UART Lite and does a self test
*
*
* Arguments: DeviceID is the DeviceId is the Device ID of the UartLite and is the
*		         XPAR_<uartlite_instance>_DEVICE_ID value from xparameters.h.
*
*
* Returns: XST_SUCCESS if successful, otherwise XST_FAILURE.
*
*
* Notes: 		
*
******************************************************************************/
int UARTLite_Init_SelfTest(u16 DeviceID)
{
  int Status;
  
  // perform initialization tests
  Status = XUartLite_Initialize(&UartLite0, DeviceID);
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


/*****************************************************************************/
/**
*
* Description: This function is responsible for data transmission
*
*
* Arguments: DataPointer is a ptr to the data that needs to be sent out
*            DataBufferPointer is the pointer the 16 byte Tx buffer
*
*
* Returns: XST_SUCCESS if successful, otherwise XST_FAILURE.
*
*
* Notes: 		
*
******************************************************************************/
int SendData(u8 *TxDataPointer, u8 *TxDataBufferPointer)
{
  unsigned int TxBytesSent;
  
  // max data sent per function call is 16 bytes (size of FIFOs)
  while (TxBytesSent != TX_DATA_SIZE)
  { 
    // put only 16 bytes of data into Tx_buff
    for(int i = 0; i < TX_BUFF_SIZE; i++)
    {
      TxDataBufferPointer[i] = TxDataPointer[TxBytesSent+i];
    }

  // Send buffer
    TxBytesSent += XUartLite_Send(&UartLite0, TxDataBufferPointer, TX_BUFF_SIZE);
  }
    
  // make sure all data was sent out
  if (TxBytesSent != TX_DATA_SIZE)
  {
   return XST_FAILURE;
  }
  
  xil_printf("\n\r");
  return XST_SUCCESS;
}  

