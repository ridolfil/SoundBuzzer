#ifndef _SOUND_BUZZ_H_
#define _SOUND_BUZZ_H_

#define BUZZER  DDB4
#define F_CLK   1000000

#include <avr/io.h>
#include <avr/interrupt.h>


//static void increase_counter() {counter++;}
static volatile unsigned int counter;

class SoundBuzz
{
private:
    unsigned int counter_max;
    bool _beeping;

public:
    SoundBuzz(/* args */);
    ~SoundBuzz();
    void init();
    void beep(int, int);
    void no_beep(int);
    void buzzer_stop();
    bool done();
    bool beeping();

};

#endif