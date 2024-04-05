#include "locos.h"

int main(int argc, char* argv[]) {
    printf("Thank you for using Custom Controller To Simrail, if you have any issue please report it on the issues page on github :)\n");
    printf("Please select loco:\n");
    for (int i = 0; i < sizeof(locosSupported) / sizeof(locosSupported[0]); i++) {
        printf("%s", locosSupported[i]);
    }

    int input = 0;
    scanf("%i", &input, sizeof(input));

    switch (input) {
    case 1:
        EU07();
        break;
    case 2: 
        printf("That is just a test case lol\n");
        break;
    default:
        printf("Invalid input!\n");
        return 0;
    }
}