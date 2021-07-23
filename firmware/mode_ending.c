#include "main.h"

void Ending_buttonPressed(Color color, MachineState* state) {
    setLed(color, 1);
    setTimeout(color*1000);
}
void Ending_timeout(MachineState* state) {
    setLeds(NULL, 0);
    setTimeout(5000);
}