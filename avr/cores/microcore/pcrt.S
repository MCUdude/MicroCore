/* picoCRT 2020 Ralph Doncaster MIT License
 * for tiny AVRs up to 8kB flash & 19 interrupt vectors
 * default vector is empty interrupt (reti)
 * 20200310 v0.1
 */

#include <avr/io.h>

  .macro  vector name
  .if (. - __vectors < _VECTORS_SIZE)
  .weak \name
  .set  \name, __empty_interrupt
  rjmp  \name
  .endif
  .endm

  .section .vectors
  .global __vectors
__vectors:
  rjmp __init
  vector __vector_1
  vector __vector_2
  vector __vector_3
  vector __vector_4
  vector __vector_5
  vector __vector_6
  vector __vector_7
  vector __vector_8
  vector __vector_9
  vector __vector_10
  vector __vector_11
  vector __vector_12
  vector __vector_13
  vector __vector_14
  vector __vector_15
  vector __vector_16
  vector __vector_17
  vector __vector_18
  vector __vector_19

  ;.text
  .global __empty_interrupt
__empty_interrupt:
  reti

    .section .init0
__init:

    .section .init2
    clr r1

    .section .init9
    .global main
; main must go in .init9 to save rjmp
;    rjmp main

; The device information avr-libc's start-up code records, which MicroCore does without because it
; links with -nostartfiles: debuggers (PyAvrOCD, for instance) read the MCU an ELF file was built for
; from here. The section is not loaded into flash. Byte for byte what avr-libc writes.
#define STR_(x) #x
#define STR(x) STR_(x)
  .section .note.gnu.avr.deviceinfo, "", @note
  .long 4                       ; length of the note's name
  .long 2f - 1f                 ; length of the description
  .long 1                       ; note type
  .asciz "AVR"
  .balign 4
1:
  .long 0                       ; flash start
  .long FLASHEND + 1            ; flash size
  .long RAMSTART                ; SRAM start
  .long RAMEND - RAMSTART + 1   ; SRAM size
  .long 0                       ; EEPROM start
  .long E2END + 1               ; EEPROM size
  .long 8                       ; size of the offset table
  .long 1                       ; offset of the device name in the string table
  .byte 0                       ; string table: the empty string, the device name, the end
  .asciz STR(__AVR_DEVICE_NAME__)
  .byte 0
2:
  .balign 4
