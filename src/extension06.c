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
#include <avr/interrupt.h>
#include <stdbool.h>

typedef enum {
    STATE_INITIAL,
    STATE_F,
    STATE_FO,
    STATE_FOO,
    STATE_FOOB,
    STATE_B,
    STATE_BA
} State;

State currentState = STATE_INITIAL;

void UART_init() {
    // Set baud rate to 9600
    PORTB.DIRSET = PIN2_bm;
    USART0.BAUD = 1389;                          
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; 
}

char uart_getc() {
    while (!(USART0.STATUS & USART_RXCIF_bm)); // Wait for data
    return USART0.RXDATAL;
}

void uart_putc(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for TXDATA empty
    USART0.TXDATAL = c;
}
// Reset state machine
void resetState() {
    currentState = STATE_INITIAL;
}

int main(void) {
    cli();
    UART_init();
    sei();

    bool wasFooDetected = false; // Flag to check if foo was detected

    while (1) {
        char c = uart_getc();

        switch (currentState) {
            case STATE_INITIAL:
                if (c == 'f') currentState = STATE_F; 
                else if (c == 'b') currentState = STATE_B;
                break;

            case STATE_F:
                currentState = (c == 'o') ? STATE_FO : STATE_INITIAL;
                break;

            case STATE_FO:
                if (c == 'o') {
                    currentState = STATE_FOO;
                    wasFooDetected = true;
                } else {
                    currentState = STATE_INITIAL;
                }
                break;

            case STATE_FOO:
                if (c == 'b') {
                    currentState = STATE_FOOB;
                } else if (c == 'f') {
                    if (wasFooDetected) {
                        uart_putc('0'); // Print '0' to UART
                        wasFooDetected = false;
                    }
                    currentState = STATE_F;
                } else {
                    currentState = STATE_INITIAL;
                    if (wasFooDetected) {
                        uart_putc('0'); // Print '0' to UART
                        wasFooDetected = false;
                    }
                }
                break;

            case STATE_FOOB:
                if (c == 'a') {
                    currentState = STATE_BA;
                } else {
                    currentState = STATE_INITIAL;
                    if (wasFooDetected) {
                        uart_putc('0'); // Print '0' to UART
                        wasFooDetected = false;
                    }
                }
                break;

            case STATE_B:
                currentState = (c == 'a') ? STATE_BA : STATE_INITIAL;
                if (wasFooDetected) {
                    uart_putc('0'); // Print '0' to UART
                    wasFooDetected = false;
                }
                break;

            case STATE_BA:
                if (c == 'r') {
                    if (wasFooDetected) {
                        uart_putc('\n'); // Print '\n' to UART
                        wasFooDetected = false;
                    } else {
                        uart_putc('1'); // Print '1' to UART
                    }
                    resetState();
                } else {
                    currentState = STATE_INITIAL;
                    if (wasFooDetected) {
                        uart_putc('0'); // Print '0' to UART
                        wasFooDetected = false;
                    }
                }
                break;
        }
    }

    return 0;
}

/** CODE: Write your code for Ex E6.0 above this line. */