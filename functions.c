#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int buttons[4] = {
    0,
    0,
    0,
    0
};

void simulateKeyPress(int keycode) {
#ifdef _WIN32
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = keycode;
    input.ki.dwFlags = 0;
    if(SendInput(1, &input, sizeof(INPUT)) != 1)
        printf(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
#else
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display.\n");
        return;
    }
    KeyCode kc = XKeysymToKeycode(display, keycode);
    XTestFakeKeyEvent(display, kc, True, 0);
    XTestFakeKeyEvent(display, kc, False, 0);
    XFlush(display);
    XCloseDisplay(display);
#endif
}

void loadButtonSettings() {
    FILE *file;
    file = fopen("GeneralSettings.txt", "r");
    if (file == NULL) {
        printf("Failed to open file");
        return;
    }

    int i = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {// Don't overload, please :3
        i++;
        printf("DEBUG: %s", line);
        if (i == 1)
            continue;
        char *data = strtok(line, "=");
        data = strtok(NULL, "=");
        int dataInt = atoi(data);

        switch (i) {
        case 2:
            buttons[0] = dataInt;
            break;

        case 3:
            buttons[1] = dataInt;
            break;

        case 4: 
            buttons[2] = dataInt;
            break;

        case 5: 
            buttons[3] = dataInt;
            break;
        
        default:
            break;
        }
    }
}

void steeringWheelCalc(int steeringStates) {
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
            return;
    }

    int lowestValue = 0;
    int highestValue = 0;
    int shuntUp = -1;
    int shuntDown = -1;
    int i = 0;
    SDL_Event event;
    printf("If you want a deadzone hold only to where you want it. If it does not update for a while just move the wheel a tiny bit\n");
    printf("Hold the wheel as left as you can\n");
    while (i < 500) {
        i++;
        SDL_PollEvent(&event);
        lowestValue = SDL_JoystickGetAxis(joystick, 0);
        SDL_Delay(10);
    }
    i = 0;
    printf("Hold the wheel as right as you can\n");
    while (i < 500) {
        i++;
        SDL_PollEvent(&event);
        highestValue = SDL_JoystickGetAxis(joystick, 0);
        SDL_Delay(10);
    }

    printf("Please click a button for shunt down\n");
    while (shuntDown == -1) {
        for (int i = 0; i < SDL_JoystickNumButtons(joystick); i++) {
            SDL_PollEvent(&event);
            if (SDL_JoystickGetButton(joystick, i) == 1)
                shuntDown = i;
        }
    }

    for (int i = 0; i < 25; i++) { // clear event input
        SDL_PollEvent(&event);
    }
     SDL_Delay(1000);

    printf("Please click a button for shunt up\n");
    while (shuntUp == -1) {
        for (int i = 0; i < SDL_JoystickNumButtons(joystick); i++) {
            SDL_PollEvent(&event);
            if (SDL_JoystickGetButton(joystick, i) == 1)
                shuntUp = i;
        }
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
    int valuePerState = range / steeringStates;
    int lastState = 0;

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

            if (event.type == SDL_JOYBUTTONDOWN) {
                if (SDL_JoystickGetButton(joystick, shuntUp))
                    simulateKeyPress(buttons[2]);
                if (SDL_JoystickGetButton(joystick, shuntDown))
                    simulateKeyPress(buttons[3]);
            }
        }
    }

    SDL_JoystickClose(joystick);
    SDL_Quit();
    return ;
}