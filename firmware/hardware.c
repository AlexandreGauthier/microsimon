#include "main.h"

#define MAIN_TIMER_FREQ 30000
#define MAIN_TIMER_PERIOD 65535
#define DEBOUNCING_DELAY 300 // Clock cycles
#define VBAT_MIN_VALUE 1800

inline uint32 timerDelta(uint32 last, uint32 now)
{
    // Timer overflow has occured since last
    if (now < last)
    {
        return MAIN_TIMER_PERIOD - last + now;
    }
    
    return now - last;
}

// Returns true if switch is not bouncing
static uint32 switchTimings[4] = {0, 0, 0, 0};
bool switchDebounce(Color color)
{   
    uint32 thisTime = getClockValue();
    uint32 lastTime = switchTimings[color];
    
    if (timerDelta(lastTime, thisTime) < DEBOUNCING_DELAY)
    {
        DBG("BOUNCE ON PRESS\n");
        // Switch is bouncing on button press
        return false;
    }
    
    for (int i = 0; i < 20; i++)
    {
        CyDelayUs(50);
        if (!isBtnPressed(color))
        {   
            DBG("BOUNCE ON RELEASE\n");
            // Switch is bouncing on button release
            return false;
        }
    }
    
    switchTimings[color] = thisTime;
    return true;
}

void setTimeout(uint16 millis)
{   
    uint32 now = MAIN_TIMER_ReadCounter();
    
    // maintain switch debouncing timings through timer reset
    for (Color i = 0; i < NoColor; i++)
    {
        switchTimings[i] = MAIN_TIMER_PERIOD - timerDelta(switchTimings[i], now);
    }
    
    
    
    MAIN_TIMER_WriteCounter(1);
    MAIN_TIMER_WriteCompare(MAIN_TIMER_FREQ*millis/1000);
}

void startBuzzer(uint16 freq)
{
    const uint32 CLOCK = 24000000;
    BZ1_PWM_Start();
    BZ1_PWM_WritePeriod(CLOCK/freq);
    BZ1_PWM_WriteCompare(CLOCK/(freq*500));
}

void stopBuzzer()
{
    BZ1_PWM_Stop();
}

bool isBtnPressed(Color color)
{
    switch (color)
    {
        case Red:
            return BTN_R_Read();
        case Green:
            return BTN_G_Read();
        case Blue:
            return BTN_B_Read();
        case Yellow:
            return BTN_Y_Read();
        default:
            errorExit(3, Red);
            return false;
    }
}

void setLed(Color color, int state)
{
    switch (color)
    {
    case Red:
        LED_R_Write(state);
        break;
    case Blue:
        LED_B_Write(state);
        break;
    case Green:
        LED_G_Write(state);
        break;
    case Yellow:
        LED_Y_Write(state);
        break;
    case NoColor:
        break;
    default:
        errorExit(1, Red);
    }
}

void setLeds(Color* colors, uint8 nColors)
{   
    for (int i = 0; i < nColors; i++)
    {
        setLed(colors[i], 0);
    }
}

void clearLeds()
{   
    LED_R_Write(1);
    LED_G_Write(1);
    LED_B_Write(1);
    LED_Y_Write(1);
}

bool isBatteryGood()
{
    return getBatteryVoltage() > VBAT_MIN_VALUE;
}

uint32 getBatteryVoltage()
{
    VBAT_ADC_StartConvert();
    VBAT_ADC_IsEndConversion(VBAT_ADC_WAIT_FOR_RESULT);
    return VBAT_ADC_GetResult16(0);
}

void setupPeripherals()
{

    PWR_EN_Write(1);
    MAIN_TIMER_Start();
    VBAT_ADC_Start();
}

uint32 getClockValue()
{
    return MAIN_TIMER_ReadCounter() * 1000 / MAIN_TIMER_FREQ;
};

void reset()
{
    CySoftwareReset();
}

void powerOff() {
    PWR_EN_Write(0);
    CyDisableInts();
    while (1)
    {
    }
}