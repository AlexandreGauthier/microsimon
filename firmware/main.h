#ifndef MAIN_H_USIMON
#define MAIN_H_USIMON

#include <stdbool.h>
#include <stdio.h>
#include "project.h"

/* Debug mode macros
    DBG_MODE_SERIAL : Enables the DBG() macro to transmit 115200-baud 8n1 UART to pin 2.1
    DBG_MODE_STATUS : Pin 2.1 is driven high when MCU is active, low when MCU is sleeping
*/

//#define DBG_MODE_STATUS
#define DBG_MODE_SERIAL
 
#ifdef DBG_MODE_SERIAL
    
    char DBG_BUFFER[50];

    #define DBG(...) {\
        SERIAL_DBG_StartEx(2, 1);\
        sprintf(DBG_BUFFER, __VA_ARGS__); \
        SERIAL_DBG_PutString(DBG_BUFFER); \
    }
#else
    #define DBG(...) {}
#endif // DBG_MODE_SERIAL


#ifdef DBG_MODE_STATUS
    #define STATUS(n) STATUS_DBG_Write(n)
#else
    #define STATUS(n) {}
#endif // DBG_MODE_STATUS

// Number of 32-bit words stored in memory
// Each word contains 16 2-bit colors
#define N_COLOR_WORDS_STORED 256

typedef enum 
{
    Red = 0b00,
    Green = 0b01,
    Blue = 0b10,
    Yellow = 0b11,
    NoColor = 0b100,
} Color;

typedef enum
{
    Menu,
    Playback,   
    Listening,
    Ending
} Mode;

typedef struct
{
    Mode   mode;
    uint32 colors[N_COLOR_WORDS_STORED];
    int    nbRounds;
    int    currentRound;
} MachineState;

// main.c

void dispatchButtonPressed(Color);
void dispatchTimeout();
void setMode(Mode mode);
const uint16 tones[5];

void errorExit(uint8 count, Color color);

// hardware.c
void setupPeripherals();

bool switchDebounce(Color);
bool isBtnPressed(Color color);

void startBuzzer(uint16 freq);
void stopBuzzer();

void setLed(Color color, int state);
void setLeds(Color* colors, uint8 nColors);
void clearLeds();

uint32 getBatteryVoltage();
bool isBatteryGood();

uint32 getClockValue();
void setTimeout(uint16 millis);

void reset();
void powerOff();

// interrupts.c
void setupInterrupts();

// color_generation.c
void seedColors(uint32 currentColor, uint32* colors);
Color getColor(uint32 n, const uint32* colors);

// mode_menu.c
void Menu_buttonPressed(Color color, MachineState* state);
void Menu_timeout(MachineState* state);

// mode_playback.c
void Playback_buttonPressed(Color color, MachineState* state);
void Playback_timeout(MachineState* state);

// mode_listening.c
void Listening_buttonPressed(Color color, MachineState* state);
void Listening_timeout(MachineState* state);

// mode_ending.c
void Ending_buttonPressed(Color color, MachineState* state);
void Ending_timeout(MachineState* state);

#endif // MAIN_H_USIMON