#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

struct ET22Values {
    int steeringStates;
};

struct ET22Values ET22DataInit() {
    return (struct ET22Values) {
        .steeringStates = 49
        };
}

int ET22() {
    struct ET22Values locoData = ET22DataInit();
    steeringWheelCalc(locoData.steeringStates);
    return 0;
}
