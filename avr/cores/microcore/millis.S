/* Ralph Doncaster 2018 public domain software
 * timer ISR counter
 */

; needed for <avr/io.h> to give io constant addresses
#define __SFR_OFFSET 0
#include <avr/io.h>

.section .bss
.global __do_clear_bss

; 4 byte (long) global variable in RAM
.lcomm wdt_interrupt_counter, 4
.global wdt_interrupt_counter

.section .text

#define tmp1 r16
#define tmp2 r17

.global WDT_vect
WDT_vect:
    push ZL
    in ZL, SREG
    push ZL                             ; save SREG
    push ZH
    push tmp1
    ldi ZL, lo8(wdt_interrupt_counter)  ; must be 8-byte aligned
    sub ZH, ZH                          ; also clears carry
add:
    ld tmp1, Z
    sbci tmp1, -16
    st Z+, tmp1
    andi ZL, 0x04
    breq add
    pop tmp1
    pop ZH
    pop ZL
    out SREG, ZL
    pop ZL
    reti
