
/**
 * VFD Firmware
 */

#include "config.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"

#define RX_CMD(buf, x) (strncmp(buf, x, sizeof(x)-1)==0)

#define BT_PORT PORTB
#define BT_PIN  PINB
#define BT_DDR  DDRB

#define BT_CANCEL (1<<PB0)
#define BT_CANCEL_MSK (1<<PCIE0)
#define BT_CANCEL_INT (1<<PCINT0)



/*
 * Print welcome and version
 */
void _cmd_helo()
{
  USART_writeln("VFD 0.0.1");
}


int main(void)
{
  char *cmd;

  // Setup UART
  USART_init();

  // Enable interrupts
  sei();

  for(;;) {
    cmd = USART_read();
    if (cmd == NULL) {
      _delay_ms(10);
      continue;
    }


    sei();
  }
}


