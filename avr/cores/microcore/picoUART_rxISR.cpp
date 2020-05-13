#include <avr/io.h>
#include <avr/interrupt.h>
#include "picoUART.h"


// RX ISR storage
volatile uint8_t purx_data __attribute(( section(".noinit") ));

// PCINT latency is 6 cycles minimum + 2 for rjmp 
// nerdralph.blogspot.com/2020/04/measuring-avr-interrupt-latency.html  
ISR(PCINT0_vect, ISR_NAKED)
{
  register char c asm ("r16");
  asm volatile (
  "push %[c]\n"
  "in %[c], __SREG__\n"               // Save SREG
  "push %[c]\n"
  "cbi %[pcint], %[rx_bit]\n"         // Turn off PCINT
  "push r24\n"                        // delay_cycles will use r25:r24
  "push r25\n"
  "ldi %[c], 0x80\n"                  // Bit shift counter
  : [c] "=d" (c)
  : [pcint] "I" (_SFR_IO_ADDR(PCMSK)),
    [rx_bit] "I" (PURXBIT)
  );
  // ISR is 18 cycles slower than purx
  if (PURXSTART >= 18 )
    __builtin_avr_delay_cycles(PURXSTART - 18);
  asm volatile (".Lrxbit:");
  __builtin_avr_delay_cycles(PURXWAIT);
  // 5 cycle loop
  asm volatile (
  "lsr %[c]\n"
  "sbic %[rx_pin], %[rx_bit]\n"
  "ori %[c], 0x80\n"
  "brcc .Lrxbit\n"
  "sts purx_data, %[c]\n"             // Save received byte
  "pop r25\n"
  "pop r24\n"
  "pop %[c]\n"
  "out __SREG__, %[c]\n"              // Restore SREG
  "pop %[c]\n"
  "reti\n"
  : [c] "+d" (c)
  : [rx_pin] "I" (_SFR_IO_ADDR(PURXPIN)),
    [rx_bit] "I" (PURXBIT)
  );
}

// Setup receive interrupt - automatically called before main
__attribute((naked, used, section (".init8")))
void purx_isr_init()
{
  PCMSK |= 1<<PURXBIT;
  GIMSK = 1<<PCIE;
  asm("sei");
}

// Read purx_data & re-enable ISR
uint8_t pu_read()
{
  uint8_t data = purx_data;
  // Wait for line idle before re-enabling interrupt
  loop_until_bit_is_set(PURXPIN, PURXBIT);
  PCMSK |= 1<<PURXBIT; // Re-enable ISR
  return data;
}

// Read purx_data
uint8_t pu_peek()
{
  return purx_data;
}
