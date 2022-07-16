#include    <avr/io.h>


.global pulse


; n*1us pulse routine
; 16 MHz clock -> 16 cycles for each 1 us

D_COR:
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    RET


pulse:
    CLI
    ; R24 contains pulse length in 1us
    ; R22 contains port value
    ; R20 contains post-delay in .5us

    ; if pulse length is 0, don't do anything

    CPI R24, 0
    BREQ RETURN

    ; push registers

    PUSH R16
    PUSH R17

    ; load current port value and set bit

    IN R16, _SFR_IO_ADDR(PORTD)
    OR R16, R22

    ; if R24>=2. (short delay), run following loop, else jump to NORMAL_LOOP2

    CPI R24, 2
    BRSH NORMAL_LOOP2


    ; small 1us wait loop

    OUT _SFR_IO_ADDR(PORTD), R16
    LDI R17, 255
    EOR R22, R17 ; negate R22
    AND R16, R22

    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    NOP
    NOP
    NOP
    NOP
    OUT _SFR_IO_ADDR(PORTD), R16


    RJMP LOOP_END2


NORMAL_LOOP2:

    DEC R24

    ; the following takes ()R24)*16 cycles
    OUT _SFR_IO_ADDR(PORTD), R16 ; set pin high - 1 cycle

    RCALL D_COR ; additional correction delay (~750 nsec)

LOOP_B: ; beginning of wait loop
    NOP ; 1 cycle
    DEC R24 ; 1 cycle

    ; 12 more cycles
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

    BRNE LOOP_B ; 2 cycles, 1 when finished



    LDI R17, 255
    EOR R22, R17 ; negate R22
    AND R16, R22
    OUT _SFR_IO_ADDR(PORTD), R16

    EOR R22, R17

LOOP_END2:

    ; post delay

    CPI R20, 0
    BREQ NO_POSTPULSE_DLY

POSTPULSE_DELAY_LOOP:
    NOP ; 4 cyc + 4 cyc, another wait loop
    DEC R20

    NOP
    NOP
    NOP
    NOP

    BRNE POSTPULSE_DELAY_LOOP

NO_POSTPULSE_DLY:

    ; pop registers and return

    POP R17
    POP R16

RETURN:

    SEI
    RET

