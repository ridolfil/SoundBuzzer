#include "SoundBuzz.h"
#include <avr/interrupt.h>

SoundBuzz::SoundBuzz()
{
    init();
}

SoundBuzz::~SoundBuzz()
{
}

void SoundBuzz::init()
{
    TCCR1 = 0;              // Reset timer register A
    GTCCR = 0;              // Reset timer register B

    DDRB |= 1 << BUZZER;    //piezo buzzer pin corresponding to the OC1B pin set to output

    TCCR1 |= 1 << CTC1;     // Set clear timer on compare. See Paragraph 12.3.1 datasheet
    TCCR1 |= 5 << CS10;     //Set the prescaler to 16 bit (for 1MHz)(freq range from 31kHz to 250 Hz)
    //GTCCR |= 1 << COM1B0;   // Toggle OC1B on Compare Match. Every time the comparator matches the OCR1C the pin goes low or high
    TIMSK |= 1 << OCIE1B;    //Enable timer overflow interrupt
}

void SoundBuzz::beep(int frequency /*Herz*/, int duration /*milliseconds*/ )
{
    /*
    *  The frequency is given by:
    *  Freq = F_CLK / (prescaler * (1 + OCR1C))
    */

    GTCCR |= 1 << COM1B0;           // Timer Counter Comparator B connected to output pin OC1B.
    OCR1C = (uint8_t) ((F_CLK / frequency) >> 5);     // divided by the prescaler 
    counter_max = duration * ((float)frequency / 1000.) * 2;
    _beeping = true;

}


void SoundBuzz::no_beep(int duration)
{
    GTCCR &= ~(1 << COM1B0);        // Timer Counter Comparator B disconnected from output pin OC1B.
    OCR1C = ((F_CLK / 1000) >> 4)+1;

    counter_max = duration;
    _beeping = false;
}

void SoundBuzz::buzzer_stop(){
    DDRB &= ~(1 << BUZZER);     // deactivate buzzer pin
    TCCR1 &= ~_BV(15);         //~_BV(15);   // Disable timer/counter: T/C1 Stopped
}

bool SoundBuzz::done()
{
    if(counter>=counter_max){
        counter = 0;
        return true;
    }
    return false;
}

bool SoundBuzz::beeping(){
    return _beeping;
}

ISR(TIMER1_COMPB_vect){
    //increase_counter();
    counter++;
}
