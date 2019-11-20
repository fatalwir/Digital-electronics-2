;* Includes ------------------------------------------------------------
; Set offset for control register addresses (NEEDED FOR I/O REGISTERS)
#define __SFR_OFFSET    0
#include <avr/io.h>

;* Define --------------------------------------------------------------
#define inout   r24
#define OUT_PIN PB5

;* Functions -----------------------------------------------------------
.global burst_asm
burst_asm:
    sbi     PORTB, OUT_PIN  ; Set output
    cbi     PORTB, OUT_PIN  ; Clear output
    dec     inout           ; Decrement index
    brne    burst_asm       ; Jump to the start if not done yet
    ret                     ; Return from subroutine