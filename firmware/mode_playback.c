#include "main.h"

#define OFF_TIME 500
#define ON_TIME 1000

void Playback_buttonPressed(Color color, MachineState* state) {
    
    // Button presses are ignored
    (void) color;
    (void) state;
}

void Playback_timeout(MachineState* state) {
    if (state->currentRound == 0) // Reveal a new color every round
    {
        DBG("Starting playback:\n");
        state->nbRounds++; 
    }
    
    if (state->currentRound % 2 == 1)
    {
        Color color = getColor(state->currentRound/2, state->colors);
        
        setLed(color, 0);
        startBuzzer(tones[color]);
        setTimeout(ON_TIME);
        
        DBG("Displaying color: %d\n", color);

    } else
    {
        clearLeds();
        stopBuzzer();
        setTimeout(OFF_TIME);
    }
    
    if (state->currentRound >= state->nbRounds*2)
    {
        setMode(Listening);
        return;
    }
    
    state->currentRound++;
}