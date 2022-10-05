#include <stdio.h>
#include <string.h>

#include "Clipboard.h"

extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream);

int copyToPlatformClipboard(char *text) {
    char cmdBuff[160] = "\0";
        
    char *cmd = strcat(strncat(strcat(cmdBuff, "echo \'"), text, 128), "\' | xsel --clipboard --input");

    FILE *fp;
    if ((fp = (FILE *) popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return 1;
    }

    char xselOutputBuffer[128];
    while (fgets(xselOutputBuffer, 128, fp) != NULL) {}
    
    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return 2;
    }

    return 0;
}

