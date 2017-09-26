
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


#define VFD_DATA_LO_DDR  DDRC
#define VFD_DATA_LO_PORT PORTC

#define VFD_DATA_HI_DDR  DDRD
#define VFD_DATA_HI_PORT PORTD

#define VFD_CTRL_DDR  DDRB
#define VFD_CTRL_PORT PORTB

#define VFD_WR    (1<<PB0)
#define VFD_CS    (1<<PB1)
#define VFD_BUSY  (1<<PB2)




/*
 * VFD
 */

void VFD_init()
{
    VFD_DATA_LO_DDR = 0x0f;
    VFD_DATA_HI_DDR = 0xf0;

    VFD_CTRL_DDR |= VFD_WR | VFD_CS | VFD_BUSY;

    // Pull up busy and not select
    VFD_CTRL_PORT |= VFD_BUSY | VFD_CS | VFD_WR;
}

void VFD_begin()
{
    // Pull down busy and select
    VFD_CTRL_PORT &= ~VFD_BUSY;
    _delay_us(2);
    VFD_CTRL_PORT &= ~VFD_CS;
}

void VFD_data(unsigned char data)
{
    VFD_DATA_HI_PORT = data & 0xf0;
    VFD_DATA_LO_PORT = data & 0x0f;

    // Pull down busy and select
    VFD_CTRL_PORT &= ~VFD_BUSY;
    _delay_us(2);
    VFD_CTRL_PORT &= ~VFD_CS;

    // Pull down not write
    VFD_CTRL_PORT &= ~VFD_WR;
    _delay_us(1);
    VFD_CTRL_PORT |= VFD_WR | VFD_BUSY;
    _delay_us(200);
    VFD_CTRL_PORT &= ~VFD_BUSY;
}


void VFD_end()
{
    VFD_CTRL_PORT |= VFD_BUSY | VFD_CS | VFD_WR;
}

void VFD_clear()
{
    VFD_begin();

    VFD_DATA_LO_PORT = 0x0e;
    VFD_DATA_HI_PORT = 0x00;

    VFD_CTRL_PORT &= ~VFD_WR;
    _delay_us(1);
    VFD_CTRL_PORT |= VFD_WR;
    _delay_us(900);
    VFD_end();
}


void VFD_write(const char* data)
{
    uint8_t i;
    uint8_t len = strlen(data);
    for (i = 0; i < len; i++) {
        VFD_data(data[i]);
    }
}



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

  _cmd_helo();

  VFD_init();
  VFD_clear();
  VFD_write("Fnord!");

  for(;;) {
    cmd = USART_read();
    if (cmd == NULL) {
      _delay_ms(10);
      continue;
    }


    sei();
  }
}


