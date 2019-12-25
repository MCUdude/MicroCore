/*** MicroCore - millis.S ***
An Arduino core designed for ATtiny13
File created and maintained by MCUdude
https://github.com/MCUdude/MicroCore
This file contains the interrupt
service routine for millis().
Thanks to Ralph Doncaster for
creating this ISR in assembly.
*/

/*
// Replaces
ISR(WDT_vect)
{
  wdt_interrupt_counter++;
  wdt_interrupt_counter *= 16;
}
*/

; needed for <avr/io.h> to give io constant addresses
#define __SFR_OFFSET 0
#include <avr/io.h>

;.section .bss
.global __do_clear_bss

; 4 byte global variable in RAM + overflow byte
.lcomm wdt_millis_counter, 5
.global wdt_millis_counter

.section .text.millis

; return millis counter ulong in r24:r27
.global _millis
_millis:
    ldi ZL, lo8(wdt_millis_counter)
    cli
    ld r24, Z+
    ld r25, Z+
    ld r26, Z+
    ld r27, Z+
    reti

#define tmp1 r16

.global WDT_vect
WDT_vect:
    push ZL
    in ZL, SREG
    push ZL                             ; Save SREG
    push tmp1
    ldi ZL, lo8(wdt_millis_counter)
    ld tmp1, Z
    subi tmp1, -19                      ; Add 19 (not 16) because the WDT clock isn't really 128 kHz
    rjmp save
add1:
    ld tmp1, Z
    sbci tmp1, -1
save:
    st Z+, tmp1
    brcc add1
    pop tmp1
    pop ZL
    out SREG, ZL
    pop ZL
    reti

.section .init8
    ldi r16, 1<<WDTIE
    out WDTCR, r16
    sei
