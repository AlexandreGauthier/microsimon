#include "main.h"

#define ON_TIME 600
#define MAX_TIME 6000

void Listening_buttonPressed(Color color, MachineState* state) {
    Color expected = getColor(state->currentRound, state->colors);
    DBG("Recieved color %d, expecting color: %d\n", color, expected);
    
    if (color != expected)
    {
        // Wrong color
        setMode(Ending);
        return;
    }
    
    if (state->currentRound == state->nbRounds - 1)
    {
        // Done listening
        setMode(Playback);
        return;
    }
    
    // Provide visual feedback
    clearLeds();
    setLed(color, 0);
    startBuzzer(tones[color]);
    setTimeout(ON_TIME);
    state->currentRound++;
}

void Listening_timeout(MachineState* state) {
    (void) state;
    
    clearLeds();
    stopBuzzer();

    if (getClockValue() >= MAX_TIME)
    {
        // User didn't anwser in time
        setMode(Ending);
        return;
    }
    
    setTimeout(MAX_TIME);
}