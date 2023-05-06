#include <delay.h>
#include <oscilator.h>

void delay_ms(uint32_t ms)
{
    unsigned int i;
    do
    {
        i = OSCILATOR_FREQ / 13000;
        while (--i)
            ; //14T per loop
    } while (--ms);
}
