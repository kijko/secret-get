#include <stdio.h>
#include <string.h>

#include "BitwardenList.h"

extern FILE *popen(const char *command, const char *type);
extern int pclose(FILE *stream);

int bwList(char *buff, int buffSize, char *secretName) {
    char cmdBuff[160] = "\0";
        
    char *cmd = strncat(strcat(cmdBuff, "bw list items --search "), secretName, 128);
    
    FILE *fp;
    if ((fp = (FILE *) popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return 1;
    }

    while (fgets(buff, buffSize, fp) != NULL) {}

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return 2;
    }

    return 0;
}

