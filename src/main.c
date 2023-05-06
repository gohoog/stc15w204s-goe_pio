#include <STC15Fxx.h>
#include <pinout.h>
#include <uart.h>
#include <delay.h>
#include <oscilator.h>

#define START_TRIES 3
#define WIN_TRIES 10
#define LOST_TRIES 0

int question;

void led_flash(int t, uint32_t ms_on, uint32_t ms_off)
{
    for (int i = 0; i < t; i++)
    {
        LED_BUILDIN = LED_BUILDIN_ON;
        delay_ms(ms_on);
        LED_BUILDIN = LED_BUILDIN_OFF;
        delay_ms(ms_off);
    }
}

int read_input_button()
{
    unsigned int i, ms;
    int inp = 0, up = 0;
    question++;
    while (P32 != SWITCH_PRESSED)
        question++;
    ms = 30 - (question & 3);
    do
    {
        i = OSCILATOR_FREQ / 13000;
        while (--i)
            question++; // 14T per loop

        if (SWITCH_GND != SWITCH_PRESSED)
        {
            up = 1;
        }
        if (SWITCH_GND == SWITCH_PRESSED && up)
        {
            inp++;
            question--;
            up = 0;
        }

        delay_ms(100);
        question++;
    } while (--ms);
    
    led_flash(1, 100, 100);
    
    return inp;
}

void itoa_10(int value, char *result)
{
    char *ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= 10;
        *ptr++ = "0123456789"[(tmp_value - value * 10)];
    } while (value);

    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void main()
{
    uart_init(BAUD_9600_110592);
    uart_send_string("VK STC15W204S Игра Чет или Нечет V1.0\n");
    uart_send_string("В начале три попытки\n");
    uart_send_string("Угадай чет или нечет, нажимая кнопку\n"); // or vice versa
    uart_send_string("Не угадал - попыток станет меньше, угадал - больше\n");
    uart_send_string("Если закончились попытки - проиграл - светодиод будет светить 10 раз\n");
    uart_send_string("Если попыток будет 10 - выиграл - светодиод горит\n");
    int tries = START_TRIES;

    question = 1;
    int guess = 0;
    char stries[10];
    while (1)
    {
        LED_BUILDIN = LED_BUILDIN_OFF;
        uart_send_string("У тебя ");
        itoa_10(tries, stries);
        uart_send_string(stries);
        if(tries == 1)
            uart_send_string(" попытка\n");
        if(tries > 1 && tries < 5)
            uart_send_string(" попытки\n");
        if(tries > 4)
            uart_send_string(" попыток\n");

        delay_ms(1000);
        led_flash(tries, 1000, 1000);
        delay_ms(1000);
        uart_send_string("Нажми кнопку несколько раз\n");

        led_flash(4, 100, 100);
        guess = read_input_button();
        if ((question & 1) == (guess & 1))
        {
            uart_send_string("Угадал!\n");
            tries++;
        }
        else
        {
            uart_send_string("Не угадал!\n");
            tries--;
        }

        if (tries == LOST_TRIES)
        {
            uart_send_string("Проиграл!\n");
            led_flash(10, 3000, 1000);
        }

        if (tries == WIN_TRIES)
        {
            uart_send_string("Победил!\n");
            LED_BUILDIN = LED_BUILDIN_ON;
        }

        if (tries == LOST_TRIES || tries == WIN_TRIES)
        {
            read_input_button();
            tries = START_TRIES;
        }
    }
}

INTERRUPT(tm0, 1)
{
    uart_handle();
}
