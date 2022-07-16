#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "luts.h"

uint8_t settings_selector;
uint8_t ch_mixed = 1;
uint8_t set_duty = 1;
uint8_t set_pw = 125;
uint8_t dr_duty_lim = 5;
uint8_t ch2_sync = 1;
uint8_t baud_selector=0;
uint8_t block=0;

#define CH0_PIN PD2
#define CH0_PORT PORTD
#define CH0_DDR DDRD
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_ACT_PIN PB5 // Active LED
#define LED_PWR_PIN PB4 // power LED

#define SWITCH_PORT_IN PINC
#define SWITCH_MODE_PIN PC5 // Mode switch
#define SWITCH_ENABLE_PIN PC4 // Enable switch
#define BAUDRATE1 38400
#define BAUDRATE2 31250
#define BAUD_PRE_38400 ((F_CPU/(16UL*BAUDRATE1))-1)
#define BAUD_PRE_31250 ((F_CPU/(16UL*BAUDRATE2))-1)

/// Using 16 MHz external xtal
/// Fuse bits: E:FF, H:DF, L:FF

extern void pulse(uint8_t pulse_len_us, uint8_t shifted_pin, uint8_t postdelay);

uint8_t pw0; // pulse widths capped at reasonable maximums at certain frequencies
uint8_t dual = 0; // single/dual output mode
uint8_t mode = 1; // output mode - control duty or pulse width
uint8_t receive_status=255, command, channel, ch_stat, sw_prescaler, sw_prescaler2, timer2_sw_prescaler, btns_old;
volatile uint8_t note_state, note, note_change=0b11;
#define NOTE_DISABLE 0b10000000
uint8_t old_note_state=255, old_note;
uint8_t note_note, note_octave, note_dval[4];

uint8_t prolong_pulse = 1;

void note_off(uint8_t ch)
{
    cli();
    if (ch == 0) {
        TCCR1B = 0;
        TIMSK1 = 0;
        TCNT1 = 0;
        note_state &= ~1;
        LED_PORT &= ~(1<<LED_ACT_PIN);
        CH0_PORT &= ~(1<<CH0_PIN);
    }
    sei();
}

void note_on(uint8_t channel) {
    if (note_state & NOTE_DISABLE) return;
    cli();

    if (channel == 0) {
        uint16_t tim_val1 = note_tim_value[note] - 1; // timer compare value
        uint16_t tim_val2 = tim_val1/32;
        tim_val2 *= (set_duty);
        tim_val2--;

        uint8_t prescale_value = note_prescale_value[note];

        TCNT1 = 0;
        OCR1A = tim_val1;
        OCR1B = tim_val2;
        TCCR1B = prescale_value | (1<<WGM12);
        TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B); // compare interrupts

    }
    sei();
}

ISR (USART_RX_vect) /// UART Received Byte
{
    uint8_t rcvd_data = UDR0;

    /// is command?
    if (rcvd_data & 0b10000000) {
        receive_status = 0;
    }

    if (receive_status == 1 && !(note_state & NOTE_DISABLE)) {
        if (command==0x80) { // Note OFF event
            if (rcvd_data == note) note_off(channel);
            receive_status++;

            note_state &= ~(1<<channel);
            note_change = 1;
        }
        else if (command==0x90) { // note ON event

            note = rcvd_data;

            receive_status++;

            if (1) {
                note_on(channel);
                note_state |= (1<<channel);
                note_change = 1;
                LED_PORT |= (1<<LED_ACT_PIN);
            }
        }
    }

    if (receive_status == 0) {
        command = (rcvd_data & 0b11110000);
        channel = (rcvd_data & 0b00001111);
        if (channel == 0 || channel == 1) receive_status ++;
    }
}

ISR (TIMER0_COMPA_vect) { /// Timer 0 Compare A ISR
    TCCR0B &= ~((1<<CS01)|(1<<CS00));
    CH0_PORT &= ~(1<<CH0_PIN);
    TCNT0 = 0;
}

ISR (TIMER1_COMPA_vect) { /// Timer 1 Compare A ISR - Set output
    if (mode == 1) {// Pulse Width mode
        pw0 = set_pw;
        uint8_t pw_max = 0;
        if (note<96) pw_max = max_pw_value[note];
        else pw_max = max_pw_value[95];
        if (pw0 > pw_max) pw0 = pw_max;

        pulse(pw0, (1<<CH0_PIN), pw0);
    }
    else if (mode == 0) CH0_PORT |= (1<<CH0_PIN);
}

ISR (TIMER1_COMPB_vect) { /// Timer 1 Compare B ISR - Clear output
    if (mode == 0) CH0_PORT &= ~(1<<CH0_PIN);
}

const uint16_t baud38400 = BAUD_PRE_38400;
const uint16_t baud31250 = BAUD_PRE_31250;
uint16_t baud_pre = BAUD_PRE_38400;

int main(void) {
    cli();

    DDRB = 0;
    DDRC = 0;
    DDRD = 0;

    CH0_DDR |= (1<<CH0_PIN);
    LED_DDR |= (1<<LED_ACT_PIN) | (1<<LED_PWR_PIN);
    LED_PORT |= (1<<LED_PWR_PIN);
    PORTC |= (1<<PC4);

    _delay_ms(10);

    /// Set up USART
    baud_pre = baud38400;
    UBRR0H = (unsigned char)((baud_pre)>>8); // Set prescaler
    UBRR0L = (unsigned char)(baud_pre);
    UCSR0B = (1<<RXEN0)|(1<<RXCIE0); // Enable RX, RX interrupt:
    UCSR0C = (1<<UCSZ00)|(1<<UCSZ01); // Set frame format

    /// Set up Timer 0 in "Clear Timer on Compare" mode
    TCCR0A = (1<<WGM01); // CTC mode
    TCCR0B = 0; // stop clock
    TCNT0 = 0; // clear timer
    TIMSK0 = (1<<OCIE0A); // enable interrupt
    sei();

    note_state = 0;

    while (1) {
        while((PINC & (1<<PC4)) == 0) {
            note_off(0);
            note_state |= NOTE_DISABLE;
        }
        note_state &= ~NOTE_DISABLE;
    }
    return 0;
}
