#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#endif

extern int buttons[4];

extern void simulateKeyPress(int keycode);
extern void loadButtonSettings();
extern void steeringWheelCalc(int steeringStates);