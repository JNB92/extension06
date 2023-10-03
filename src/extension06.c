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

int main(void)
{

}

/** CODE: Write your code for Ex E6.0 above this line. */
