#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"



int main(void)
{
    char rChar;
    char echoC;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";//43+6=49

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool stateChk = false;
    states stateFSM = STATE0;


    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    //Declare a declaration for the struct and give it a name that will use the parameters
    eUSCI_UART_ConfigV1 uartConfig = {
       EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
       19,                                           // UCBR   = 312 //divided by 16
       8,                                            // UCBRF  = 8 // modulus of 16
       0x55,                                         // UCBRS  = 0x55 //modulation  pattern decided by the fractional part
       EUSCI_A_UART_NO_PARITY,                       // No Parity
       EUSCI_A_UART_LSB_FIRST,                       // LSB First
       EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
       EUSCI_A_UART_MODE,                            // UART mode
       EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
       EUSCI_A_UART_8_BIT_LEN                        //8bit length
    };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Initialize EUSCI_A0  // use input pin as RX and use OUTPUTfunction as Tx
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
        {
            rChar = UART_receiveData(EUSCI_A0_BASE);
            stateChk = charFSM(rChar, stateFSM);

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
            if (stateChk && rChar != '4')
            {
                if (rChar == '2')
                {
                    echoC = '2';
                }
                else if (rChar =='3')
                {
                    echoC = '3';
                }
                else if (rChar =='5')
                {
                    echoC = '5';
                }
                if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
                {
                    UART_transmitData(EUSCI_A0_BASE, echoC);
                }
            }
        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
            else if(stateChk && rChar == '4')
            {
                if (UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
                {
                    int i;
                    for (i = 0;i<49;i++)
                    {
                        echoC = response[i];
                        UART_transmitData(EUSCI_A0_BASE, echoC);
                    }
                }

            }

        }
    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar, states currentState)
{
    bool finished = false;
    switch (currentState)
    {
    case STATE0:
        if (rChar == '2')
        {
            currentState = STATE1;
            finished = true;
        }
        else
        {
            currentState = STATE0;
            finished = false;
        }
    break;
    case STATE1:
        if (rChar =='2')
        {
            currentState = STATE1;
            finished = true;
        }
        else if(rChar =='5')
        {
            currentState =STATE2;
            finished = true;
        }
        else
        {
            currentState = STATE0;
        }
    break;
    case STATE2:
        if (rChar =='3')
        {
            currentState = STATE3;
            finished = true;
        }
        else
        {
            currentState = STATE0;
        }
    break;
    case STATE3:
        if (rChar =='4')
        {
            currentState = STATE4;
            finished = true;
        }
        else
        {
            currentState = STATE0;
        }
    break;
    case STATE4:
            currentState = STATE0;
            finished = true;
    break;
    }
    return finished;
}
