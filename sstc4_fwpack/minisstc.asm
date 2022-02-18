; ATtiny13 based Tesla coil protection (Undervoltage lockout and duty cycle limiter)
;
; Fuse settings:
; High fuse: 0xFD
; Low fuse: 0x7A
; 
; Clock source: internal RC oscillator at 9.6 MHz, SUT: 14 cycles + 64 ms
; Brown-out detection: Set at 1.8V
; SPI programming: enabled
;
; Copyright (c) 2016-2017, Frantisek Štefanec
;
; Firmware version 1.1
;

.include "tn13def.inc"


; ----- USER DEFINED VALUES -----

; Duty limiter time constant is set by hardware: resistor between PB0 and PB4, capacitor between PB4 and ground
; Duty cycle limiting is implemented on an external RC circuit, the percentages may not be accurate for lower BPS.
; Duty limiter values in %
	
	.equ MAX_DUTY=20
	.equ RELEASE_DUTY=10


; Output and input pins

	.equ PSEL_PIN=PB0
	.equ LED_PIN=PB1
	.equ OUT_PIN=PB2
	.equ IN_PIN=PB4

; ----- END OF USER DEFINED VALUES -----

; ------------------------------
; Register names

.def REG=R16
.def AD_STAT=R17
.def PINB_BUF=R18
.def ERR_V=R19
.def ERR_D=R20
.def REG2=R21


; ------------------------------
; ADC release/lockout values

.equ DCL_ADC_VALUE=256-(256*MAX_DUTY/100)
.equ DCL_REL_ADC_VALUE=256-(256*RELEASE_DUTY/100)

; ADLAR = result will be left-adjusted (top byte in ADCH), don't change
; ADC pins:

.equ ADMUX_DUTY=(1<<ADLAR)|(1<<MUX1)|(1<<MUX0) ; ADC3/PB3
.equ ADMUX_BPS=(1<<ADLAR)|(1<<MUX1)|(0<<MUX0) ; ADC2/PB4

; ------------------------------
; Code segment

.CSEG

; ISR addresses - no ISRs here, just jump to main.

.ORG 0x00
	RJMP MAIN
.ORG 0x01
	NOP
.ORG 0x02
	NOP
.ORG 0x03
	NOP
.ORG 0x04
	NOP
.ORG 0x05
	NOP
.ORG 0x06
	NOP
.ORG 0x07
	NOP
.ORG 0x08
	NOP
.ORG 0x09
	NOP
.ORG 0x0A

MAIN:
	; clear interrupts
	CLI

	; set stack pointer
	LDI 	REG, LOW(RAMEND)
	OUT 	SPL, REG

	; set outputs and inputs
	LDI	REG, (1<<OUT_PIN)
	OUT	PORTB, REG
	LDI 	REG, (1<<OUT_PIN)|(1<<LED_PIN)
	OUT 	DDRB, REG

	
	; check if watchdog reset happened - if yes, freeze and blink SOS
	IN	REG, MCUSR
	ANDI	REG, (1<<WDRF)
	BREQ 	NO_WDR
	LDI	REG, 0
	OUT	MCUSR, REG
	RJMP 	FREEZE_BLINK_SOS

	; Wait for 300 ms before doing anything else (time for voltage to stabilize)
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms

NO_WDR:
	
	; disable watchdog
	RCALL 	DISABLE_WD

	; set up AD converter
	LDI	REG, (1<<ADEN)|(1<<ADPS0)|(1<<ADPS0)|(1<<ADPS2) ; enable ADC, set :128 prescaler - ADC is running at ~750 kHz, sample rate ~6 kSPS
	OUT 	ADCSRA, REG
	LDI	REG, ADMUX_DUTY
	OUT 	ADMUX, REG

	; wait 200 ms before doing anything else (time for voltages to stabilize)
	RCALL	DLY_100ms
	RCALL	DLY_100ms

	; start another AD conversion
	SBI	ADCSRA,	ADSC

	; enable LED
	SBI	PORTB, LED_PIN

	; call the Enable Watchdog subroutine
	; The microcontroller is running near strong electromagnetic fields, in case it crashes and keeps its output high,
	; the watchdog should restart it to prevent potential damage to the power electronics.
	; Upon restart, the indicator LED will blink with an "SOS" patern.
	RCALL 	ENABLE_WD

LOOP:	; main program loop
	; "pat" the watchdog
	WDR

	; load Port B value into register
	IN	PINB_BUF, PINB

	; if potentiometer select pin grounded, call the interrupt signal generator routine
	SBRS	PINB_BUF, PSEL_PIN
	RCALL	SIG_GEN

	; transfer REVERSED interrupter pin value from register to output pin
	SBRS 	PINB_BUF, IN_PIN
	SBI	PORTB, OUT_PIN
	SBRC	PINB_BUF, IN_PIN
	CBI	PORTB, OUT_PIN


 	; skip if conversion in progress - just repeat loop
	SBIC	ADCSRA, ADSC
	RJMP	LOOP
	
	; if ADC conversion is finished:
	; were we measuring duty cycle?

	IN	REG, ADCH
	CPI	REG, DCL_ADC_VALUE
	; if duty cycle lower than maximum, OK (if AD value HIGHER than set limit - reversed)
	BRSH	DT_OK 
	RCALL	DUTY_LIMIT
DT_OK:	SBI	ADCSRA, ADSC
	RJMP LOOP

DLY_5ms:
	; 5 ms delay loop at 9.6MHz
	; generated using http://www.bretmulvey.com/avrdelay.html
	PUSH	R19
	PUSH	R18
	LDI	R18, 63
	LDI	R19, 82
L3:	DEC	R19
	BRNE 	L3
	DEC	R18
	BRNE	L3
	POP	R18
	POP	R19
	RET

DLY_2ms:
	; approx. 2 ms delay loop at 9.6MHz
	; generated using http://www.bretmulvey.com/avrdelay.html
	PUSH	R19
	PUSH	R18
	LDI	R18, 25
	LDI	R19, 234
L4:	DEC	R19
	BRNE 	L4
	DEC	R18
	BRNE	L4
	POP	R18
	POP	R19
	RET

DLY_10ms:
	; 10 ms delay loop at 9.6MHz
	; generated using http://www.bretmulvey.com/avrdelay.html
	PUSH	R19
	PUSH	R18
	LDI	R18, 125
	LDI	R19, 168
L2:	DEC	R19
	BRNE 	L2
	DEC	R18
	BRNE	L2
	RJMP	PC+1
	POP	R18
	POP	R19
	RET

DLY_100ms:
	; 100 ms delay loop at 9.6MHz
	; generated using http://www.bretmulvey.com/avrdelay.html
	PUSH	R18
	PUSH	R19
	PUSH	R20
	LDI	R18, 5
	LDI	R19, 223
	LDI	R20, 182
L1:	DEC	R20
	BRNE	L1
	DEC 	R19
	BRNE	L1
	DEC	R18
	BRNE 	L1
	POP 	R20
	POP	R19
	POP	R18
	RET
	
AD_CONV:
	; start Analog to digital conversion
	SBI	ADCSRA, ADSC
AD_WAIT:
	; wait while running, then return from subroutine
	SBIC 	ADCSRA, ADSC
	RJMP 	AD_WAIT
	RET


	; interrupt signal generator routine
SIG_GEN:
	PUSH	REG
	PUSH	REG2

	; change ADMUX to read from BPS pot
	LDI	REG, ADMUX_BPS
	OUT	ADMUX, REG
	; start an ADC conversion
	SBI 	ADCSRA, ADSC
GEN_LOOP:
	; reset WDT
	WDR
	; read ADC and start new conversion
W1:	SBIC	ADCSRA, ADSC
	RJMP	W1
	IN	REG, ADCH
	SBI 	ADCSRA, ADSC
	; if pot value too low, don't do anything
	CPI	REG, 10
	BRLO	NO_SIG
	; calculate wait delay for off time - will vary approximately from 10 to 140, the higher the pot value, the lower the off delay
	LDI	REG2, 255
	SUB	REG2, REG
	LSR	REG2
	LDI	REG, 10
	ADD	REG2, REG
	; off time wait loop
	SBI	PORTB, OUT_PIN
OFF_WAIT:
	RCALL	DLY_2ms
	WDR ; don't forget to reset the WDT!
	DEC	REG2
	BRNE 	OFF_WAIT

	; 5 ms on pulse
	CBI	PORTB, OUT_PIN
	RCALL	DLY_2ms
	SBI	PORTB, OUT_PIN
	WDR
NO_SIG:
	; check if pot enable input still pulled low, if yes, repeat loop
	SBIS	PINB, PSEL_PIN
	RJMP 	GEN_LOOP
	; set admux back to duty
	LDI	REG, ADMUX_DUTY
	OUT	ADMUX, REG
	SBI	ADCSRA, ADSC
	POP	REG2
	POP 	REG
	RET	


	; Duty Cycle Limiter routine
DUTY_LIMIT:
	SBI	PORTB, OUT_PIN
	RCALL	DISABLE_WD
	; 400 ms wait loop with blinking led with 1/2 duty and 200 ms period
DUTY_LIMIT_REPEAT:
	SBI	PORTB, LED_PIN
	RCALL	DLY_100ms
	CBI	PORTB, LED_PIN
	RCALL	DLY_100ms
	; measure duty limiter voltage again
	LDI	REG, ADMUX_DUTY
	OUT	ADMUX, REG
	RCALL	AD_CONV
	IN	REG, ADCH
	; if still too high (AD value too low), repeat the wait cycle and duty measurement
	CPI	REG, DCL_REL_ADC_VALUE
	BRLO	DUTY_LIMIT_REPEAT
	; if interrupter input pin in ON, repeat the wait cycle
	IN 	PINB_BUF, PINB
	SBRC	PINB_BUF, IN_PIN
	RJMP	DUTY_LIMIT_REPEAT	
	; if OK, return
	RCALL	ENABLE_WD
	SBI	PORTB, LED_PIN
	RET
	
	; Watchdog disable
DISABLE_WD:
	WDR
	; clear WDRF in MCUSR
	IN	REG, MCUSR
	ANDI	REG, (0xFF - (1<<WDRF))
	OUT	MCUSR, REG
	; watchdog change enable
	IN	REG, WDTCR
	ORI	REG, (1<<WDCE)|(1<<WDE)
	OUT	WDTCR, REG
	; turn off WDT
	LDI	REG, (0<<WDE)
	OUT	WDTCR, REG
	RET

	; Watchdog enable
ENABLE_WD:
	WDR
	; watchdog change enable
	IN	REG, WDTCR
	ORI	REG, (1<<WDCE)|(1<<WDE)
	OUT	WDTCR, REG
	; set prescaler
	LDI	REG, (1<<WDE)|(1<<WDP1) ; watchdog timeout = 64 ms
	OUT	WDTCR, REG
	RET


; subroutine to freeze and blink with a SOS pattern on microcontroller crash

FREEZE_BLINK_SOS:
	; disable output pin and watchdog
	CBI	PORTB, OUT_PIN
	RCALL	DISABLE_WD

	; 3 short blinks
	LDI	REG, 3
LP1:	SBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	CBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	DEC	REG
	BRNE	LP1

	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms

	; 3 long blinks
	LDI	REG, 3
LP2:	SBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	CBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	DEC	REG
	BRNE	LP2
	
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms

	; 3 short blinks
	LDI	REG, 3
LP3:	SBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	CBI	PORTB, LED_PIN
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	DEC	REG
	BRNE	LP3

	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms
	RCALL 	DLY_100ms

	RJMP 	FREEZE_BLINK_SOS
