#include <stdio.h>

int copyToPlatformClipboard(char *text) {
    printf("win mock copying to clipboard: '%s'", text);

    return 0;
}

