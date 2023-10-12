/** EX: E6.0

TASK:

Your task is to write code which handles a sequence of input characters
from the UART interface, and respond with the output specified below.

On receipt of the character sequence:

- "foo" your programme should print '0' to the UART interface.
- "bar" your programme should print '1' to the UART interface.
- "foobar" your program should not print either '0' nor '1' as
  specified above, but should instead print a linefeed ('\n')
  character.

NOTE:

It is strongly recommended that you design a state machine to complete
this task, and practice drawing a state machine diagram before you
begin coding.

Your solution should use a baud rate of 9600, and 8N1 frame format.
Your solution MUST NOT use qutyio.o or qutyserial.o.

EXAMPLES:

INPUT: ...foo.bar.foo.barfoobarfood
OUTPUT: 0101\n0

0101
0(END)

INPUT: barsfoosbarforbarfoobarrforfoobarfoobarfood
OUTPUT: 1011\n\n\n0

1011


0(END)

*/

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>





// States for our state machine
typedef enum {
    STATE_INITIAL,
    STATE_F,
    STATE_FO,
    STATE_FOO,
    STATE_B,
    STATE_BA,
    STATE_BAR,
    STATE_FOOF,
    STATE_FOOFO,
    STATE_FOOFOO
} State;

State currentState = STATE_INITIAL;

void UART_init() {

    PORTB.DIRSET = PIN2_bm; // Set TX pin as output

    USART0.BAUD = 1369; // Set baud rate to 9600

    // uint16_t UBRR_val = (F_CPU/16/BAUD_RATE) - 1;
    // USART0.BAUD = (uint16_t)UBRR_val;

    // Enable receiver and transmitter
    USART0.CTRLA = USART_RXCIE_bm | USART_DREIE_bm; // Enable interrupts for RX and TX

    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable receiver and transmitter

    // Set frame format to 8N1
    //USART0.CTRLC = USART_CHSIZE_8BIT_gc;
}

char UART_receive() {
    while (!(USART0.STATUS & USART_RXCIE_bm));
    return USART0.RXDATAL;
}

void UART_transmit(char data) {
    while (!(USART0.STATUS & USART_DREIE_bm));
    USART0.TXDATAL = data;
}

int main(void) {
    UART_init();

    char inputChar;

    while (1) {
        inputChar = UART_receive();

        switch (currentState) {
            case STATE_INITIAL:
                if (inputChar == 'f') currentState = STATE_F;
                else if (inputChar == 'b') currentState = STATE_B;
                break;

            case STATE_F:
                if (inputChar == 'o') currentState = STATE_FO;
                else currentState = STATE_INITIAL;
                break;

            case STATE_FO:
                if (inputChar == 'o') currentState = STATE_FOO;
                else if (inputChar == 'f') currentState = STATE_FOOF;
                else currentState = STATE_INITIAL;
                break;

            case STATE_FOO:
                if (inputChar == 'b') currentState = STATE_INITIAL, UART_transmit('0');
                else if (inputChar == 'f') currentState = STATE_FOOF;
                else currentState = STATE_INITIAL;
                break;

            case STATE_B:
                if (inputChar == 'a') currentState = STATE_BA;
                else currentState = STATE_INITIAL;
                break;

            case STATE_BA:
                if (inputChar == 'r') currentState = STATE_BAR;
                else currentState = STATE_INITIAL;
                break;

            case STATE_BAR:
                UART_transmit('1');
                currentState = STATE_INITIAL;
                break;

            case STATE_FOOF:
                if (inputChar == 'o') currentState = STATE_FOOFO;
                else currentState = STATE_INITIAL;
                break;

            case STATE_FOOFO:
                if (inputChar == 'o') currentState = STATE_FOOFOO;
                else if (inputChar == 'b') currentState = STATE_INITIAL, UART_transmit('0');
                else currentState = STATE_INITIAL;
                break;

            case STATE_FOOFOO:
                UART_transmit('\n');
                currentState = STATE_INITIAL;
                break;
        }
    }

    return 0;
}


/** CODE: Write your code for Ex E6.0 above this line. */
