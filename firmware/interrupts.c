#include "main.h"

CY_ISR(BTN_R_Handler)
{
    dispatchButtonPressed(Red);
    BTN_R_INTR_ClearPending();
}

CY_ISR(BTN_G_Handler)
{
    dispatchButtonPressed(Green);
    BTN_G_INTR_ClearPending();
}

CY_ISR(BTN_B_Handler)
{
    dispatchButtonPressed(Blue);
    BTN_B_INTR_ClearPending();
}

CY_ISR(BTN_Y_Handler)
{
    dispatchButtonPressed(Yellow);
    BTN_Y_INTR_ClearPending();
}

CY_ISR(TIMEOUT_Handler)
{
    MAIN_TIMER_Stop();
    dispatchTimeout();
    MAIN_TIMER_ClearInterrupt(MAIN_TIMER_INTR_MASK_CC_MATCH);
    MAIN_TIMER_Start();
}


void setupInterrupts()
{
    BTN_R_INTR_StartEx(BTN_R_Handler);
    BTN_G_INTR_StartEx(BTN_G_Handler);
    BTN_B_INTR_StartEx(BTN_B_Handler);
    BTN_Y_INTR_StartEx(BTN_Y_Handler);
    
    TIMEOUT_INTR_StartEx(TIMEOUT_Handler);
    
    CyGlobalIntEnable;
}