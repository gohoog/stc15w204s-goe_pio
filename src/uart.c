#include <STC15Fxx.h>
#include <pinout.h>
#include <uart.h>

BYTE TBUF, RBUF;
BYTE TDAT, RDAT;
BYTE TCNT, RCNT;
BYTE TBIT, RBIT;
BYTE TING, RING;
BYTE TEND, REND;

//-----------------------------------------
// Timer interrupt routine for UART

void uart_handle()
{
    if (RING)
    {
        if (--RCNT == 0)
        {
            RCNT = 3; // reset send baudrate counter
            if (--RBIT == 0)
            {
                RBUF = RDAT; // save the data to RBUF
                RING = 0;    // stop receive
                REND = 1;    // set receive completed flag
            }
            else
            {
                RDAT >>= 1;
                if (RXB)
                    RDAT |= 0x80; // shift RX data to RX buffer
            }
        }
    }
    else if (!RXB)
    {
        RING = 1; // set start receive flag
        RCNT = 4; // initial receive baudrate counter
        RBIT = 9; // initial receive bit number (8 data bits + 1 stop bit)
    }

    if (--TCNT == 0)
    {
        TCNT = 3; // reset send baudrate counter
        if (TING) // judge whether sending
        {
            if (TBIT == 0)
            {
                TXB = 0;     // send start bit
                TDAT = TBUF; // load data from TBUF to TDAT
                TBIT = 9;    // initial send bit number (8 data bits + 1 stop bit)
            }
            else
            {
                TDAT >>= 1; // shift data to CY
                if (--TBIT == 0)
                {
                    TXB = 1;
                    TING = 0; // stop send
                    TEND = 1; // set send completed flag
                }
                else
                {
                    TXB = CY; // write CY to TX port
                }
            }
        }
    }
}
void uart_init(unsigned long baudrate)
{
    TR0 = 1; // timer0 start running
    ET0 = 1; // enable timer0 interrupt
    PT0 = 1; // improve timer0 interrupt priority
    EA = 1;  // open global interrupt switch

    TMOD = 0x00; // timer0 in 16-bit auto reload mode
    AUXR = 0x80; // timer0 working at 1T mode
    TL0 = baudrate;
    TH0 = baudrate >> 8; // initial timer0 and set reload value

    TING = 0;
    RING = 0;
    TEND = 1;
    REND = 0;
    TCNT = 0;
    RCNT = 0;

}

void uart_send_byte(unsigned char dt)
{
    while (!TEND)
        ;
    TEND = 0;
    TBUF = dt;
    TING = 1;
}

void uart_send_string(char *str)
{
    while (*str != '\0')
        uart_send_byte(*str++);
}

BYTE uart_get_byte()
{
    while (!REND)
        ;

    REND = 0;
    return RBUF;   
}