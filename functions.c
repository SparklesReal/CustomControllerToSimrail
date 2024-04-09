#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    SendInput(1, &input, sizeof(INPUT));
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
        printf("DEBUG: %s\n", line);
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