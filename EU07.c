#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

struct EU07Values {
    int steeringStates;
};

struct EU07Values EU07DataInit() {
    return (struct EU07Values) {
        .steeringStates = 43 
        };
}

int EU07() {
    struct EU07Values locoData = EU07DataInit();
    steeringWheelCalc(locoData.steeringStates);
    return 0;
}