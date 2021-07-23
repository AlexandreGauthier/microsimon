#include "main.h"

/*
    To save battery, this firmare is always racing to sleep.
    On an interrupt (button press or timeout), the ARM
    core executes the small routine associated with the
    current mode and interrupt source then goes back to sleep.
*/

static MachineState state;

#ifdef ENABLE_SERIAL_DGB
    char DBG_BUFFER[50];
#endif

const uint16 tones[5] = {
    1234, // Red
    400, // Green
    600, // Blue
    800, // Yellow
    0    // NoColor
};


void errorExit(uint8 count, Color color)
{
    DBG("EXITING BECAUSE OF ERROR: %d %d\n", count, color); 
    
    // Display error code 
    for (uint32 i = 0; i < count; i++)
    {
        setLed(color, 0);
        CyDelay(400);
        setLed(color, 1);
        CyDelay(200);
    }

    powerOff();
}

int main(void)
{ 
    setupPeripherals();
    
    if (!isBatteryGood())
    {
        //errorExit(Yellow, 2);
    }
    
    DBG("\n\nStarting Serial Debug.\n");

    
    setupInterrupts();
    setMode(Menu);
    setTimeout(1000);
    
    for(;;)
    {
        DBG("Sleeping...\n");
        STATUS(0);
        CySysPmSleep();
    }
    
}

// Executed on every button press by the ISR
void dispatchButtonPressed(Color color)
{
    uint8 s = CyEnterCriticalSection();
    
    STATUS(1);
    
    seedColors(state.nbRounds, state.colors);
    
    if (switchDebounce(color)) {
        DBG("Button press: %d\n", color);
        switch (state.mode)
        {
        case Menu:
            Menu_buttonPressed(color, &state);
            break;
        case Playback:
            Playback_buttonPressed(color, &state);
            break;
        case Listening:
            Listening_buttonPressed(color, &state);
            break;
        case Ending:
            Ending_buttonPressed(color, &state);
            break;
        }
    }
    
    CyExitCriticalSection(s);
}

// Executed on main timer timeout
void dispatchTimeout()
{
    uint8 s = CyEnterCriticalSection();
    
    seedColors(state.nbRounds, state.colors);
    
    STATUS(1);
    DBG("Woken up from main timer.\n");
    
    switch (state.mode)
    {
    case Menu:
        Menu_timeout(&state);
        break;
    case Playback:
        Playback_timeout(&state);
        break;
    case Listening:
        Listening_timeout(&state);
        break;
    case Ending:
        Ending_timeout(&state);
        break;
    }
    
    CyExitCriticalSection(s);
}

void setMode(Mode mode)
{
    DBG("Setting mode to %d\n", mode); 
    
    state.mode = mode;
    state.currentRound = 0;
    setTimeout(1);
}