#include <SDL2/SDL.h>
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

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
        printf("Failed to initialize SDL_INIT_JOYSTICK | %s", SDL_GetError());
    }

    loadButtonSettings();

    printf("Ensure steering wheel is plugged in!\n");
    SDL_Delay(2500);
    bool correctJoystick = false;
    SDL_Joystick *joystick;
    for (int i = 0; correctJoystick == false; i++) {
        joystick = SDL_JoystickOpen(i);
        if (joystick) {
            printf("Joystick connected: %s\n", SDL_JoystickName(joystick));
            char input = 'n';
            printf("Is this correct? y/n\n");
            scanf(" %c", &input);
            if (input == 'y')
                correctJoystick = true;
        }
        else
            return 1;
    }

    int lowestValue = 0;
    int highestValue = 0;
    int i = 0;
    SDL_Event event;
    printf("Hold the wheel as left as you can\n");
    while (i < 500) {
        i++;
        SDL_PollEvent(&event);
        lowestValue = SDL_JoystickGetAxis(joystick, 0);
        SDL_Delay(10);
    }
    printf("%i", lowestValue);
    i = 0;
    printf("Hold the wheel as right as you can\n");
    while (i < 500) {
        i++;
        SDL_PollEvent(&event);
        highestValue = SDL_JoystickGetAxis(joystick, 0);
        SDL_Delay(10);
    }

    int oldLowest = lowestValue;
    if (lowestValue < 0) {
        highestValue += abs(lowestValue);
        lowestValue += abs(lowestValue);
    }

    if (highestValue < 0) {
        lowestValue += abs(highestValue);
        highestValue += abs(highestValue);
    }

    int range = highestValue - lowestValue;
    int valuePerState = range / locoData.steeringStates;
    int lastState;

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            if (event.type == SDL_JOYAXISMOTION) {
                if (event.jaxis.axis == 0) {
                    int steeringValue = event.jaxis.value;
                    steeringValue += abs(oldLowest);
                    int state = steeringValue / valuePerState;
                    printf("Steering Value: %d\n", steeringValue);
                    printf("State Value: %d\n", state);
                    if (state > lastState) {
                        simulateKeyPress(buttons[0]);
                    }
                    if (state < lastState) {
                        simulateKeyPress(buttons[1]);
                    }
                    if (state == 43) {
                        simulateKeyPress(buttons[0]);
                    }
                    if (state == 0) {
                        simulateKeyPress(buttons[1]);
                    }
                    lastState = state;
                }
            }
        }
    }

    SDL_JoystickClose(joystick);
    SDL_Quit();
    return 0;
}