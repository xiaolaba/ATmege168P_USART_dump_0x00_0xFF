/*
 * UART, dump 0x00 to 0xff to terminal
 * hardware, Arduino Nano, Atmega168p/328p, 16MHz XTAL
 * compiler, Arduino IDE 1.8.9
 * xiaolaba
 * 2020-JAN-24
 */

#include <avr/io.h>
#include <util/twi.h>

#define F_CPU 16000000L
#define SERIAL_BAUD 115200L

#define UBBRn_V ((F_CPU / 16 + SERIAL_BAUD / 2) / SERIAL_BAUD - 1)

static void usart_putstr(char *s);
static void usart_putchar(char data);
static void usart_printhex(uint8_t v);
static void serial_init(void);

static char HEX_TABLE[]="0123456789ABCDEF";

int main(void)
{   
    serial_init();
    return 0;
}


/** Initializes the serial port and prints "banner. */
// serial port 115200 N81
static void serial_init(void)
{
  UBRR0H = UBBRn_V >> 8;
  UBRR0L = UBBRn_V;
  UCSR0B = _BV(TXEN0);

  usart_putstr("xiaolaba\n");
  usart_putstr("Arduino Nano, ATmega168, 115200, N81, serial print 0x00 - 0xff\n");

  short i;
  for (i = 0; i <=0xff; i++) {
    usart_printhex(i);
    usart_putchar(' ');
    if ((i & 0x0f) == 0x0f){ usart_putchar('\n');}
  }
}


/** Prints a 8 bit hex number. */
static void usart_printhex(uint8_t v)
{
    usart_putchar('0');
    usart_putchar('x');

    uint8_t vn = v >> 4;
    usart_putchar(HEX_TABLE[vn]);
    vn = v & 0x0F;
    usart_putchar(HEX_TABLE[vn]);
}

// ********************************************************************************
// usart Related
// ********************************************************************************
static void usart_putchar(char data) {
    // Wait for empty transmit buffer
    while ( !(UCSR0A & (_BV(UDRE0))) );
    // Start transmission
    UDR0 = data; 
}

static void usart_putstr(char *s) {
    // loop through entire string
    while (*s) { 
        usart_putchar(*s);
        s++;
    }
}
