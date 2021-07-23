#include "main.h"

#define ANIMATION_LENGTH 9
#define LOOP_LENGTH 2

typedef struct
{
    uint16 length;
    uint16 tone;
    uint8  colors;
} Frame;

const Frame ANIMATION[ANIMATION_LENGTH] = {
//  LENGTH TONE 0bYGBR
    {100, 493, 0b0001},
    {100, 523, 0b0011},
    {100, 587, 0b0111},
    {100, 659, 0b1111},
    {100, 587, 0b1111},
    {100, 523, 0b1101},
    {100, 493, 0b1001},
    {75, 493, 0b0001},
    {100, 493, 0b0001},
};

const Frame LOOP[LOOP_LENGTH] = {
//  LENGTH TONE 0bYGBR
    {800,  0, 0b0001},
    {400, 0, 0b0000}
};

void playFrame(const Frame* frame)
{
    uint8 mask = 1; 
    for (Color color = 0; color != NoColor; color++)
    {
        setLed(color, (frame->colors & mask) == 0);
        mask <<= 1;
    }
    
    if (frame->tone)
    {
        startBuzzer(frame->tone);
    } else
    {
        stopBuzzer();
    }
    
    setTimeout(frame->length);
}

void Menu_buttonPressed(Color color, MachineState* state) {
    if (state->currentRound >= ANIMATION_LENGTH)
    {
        switch (color)
        {
            case 0:
                setMode(Playback);
                break;
            default:
                break;
        }
    }
}

void Menu_timeout(MachineState* state) {
    const Frame* frame;
    if (state->currentRound < ANIMATION_LENGTH)
    {
         frame = &ANIMATION[state->currentRound];
    } else {
        frame = &LOOP[(state->currentRound - ANIMATION_LENGTH) % LOOP_LENGTH];
    }
    
    playFrame(frame);
    state->currentRound++;
}

