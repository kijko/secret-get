#include <stdio.h>
#include <argp.h>
#include <string.h>

#include "State.h"
#include "SecretGet.h"

static int setStateDefaults();
static int parseOptions(int key, char *arg, struct argp_state *state);

int main(int argc, char **argv) {
    int isSetup = init() + setStateDefaults();

    if (isSetup != 0) {
        printf("Non-zero status settings things up");
        
        return isSetup;
    }

    struct argp_option options[] = {
         { "storage", 's', "ENUM", 0, "Choose storage. Supported: [BW, EL]. Default: BW", 0 },
         { 0 }
    };
    struct argp argp = { options, parseOptions, 0, 0, 0, 0, 0 }; 

    int parseResult = argp_parse(&argp, argc, argv, 0, 0, 0);
    
    if (parseResult != 0) {
        printf("Non-zero(%d) parse result. Exiting...\n", parseResult);

        return parseResult;
    }
        
    findSecretAndAddToClipboard();

    cleanUp();

    return 0;
}

static int setStateDefaults() {
    return setStorage(STORAGE_BITWARDEN);
}

static int parseOptions(int key, char *arg, struct argp_state *state) {
    switch (key) {
        case 's': {
            if (strcmp(arg, "BW") == 0) {
                if (setStorage(STORAGE_BITWARDEN) != 0) {
                    printf("Error setting bitwarden as storage");
                    
                    return ARGP_KEY_ERROR;
                }
            } else if (strcmp(arg, "EL") == 0) {
                if (setStorage(STORAGE_ELSE) != 0) {
                    printf("Error setting something else as storage");
                    
                    return ARGP_KEY_ERROR;
                }
            } else {
                printf("Unknown storage: %s\n", arg);
                
                return ARGP_KEY_ERROR;
            }
            
            break;
        }

        default: return ARGP_ERR_UNKNOWN;
    }

    return 0;
}




//    char *cmd = "bw list items --search google";    
//    
//    char buf[128];
//    FILE *fp;
//
//    if ((fp = popen(cmd, "r")) == NULL) {
//        printf("Error opening pipe!\n");
//        return -1;
//    }
//
//    while (fgets(buf, 128, fp) != NULL) {
//        // Do whatever you want here...
//        printf("OUTPUT: %s", buf);
//    }
//
//    if (pclose(fp)) {
//        printf("Command not found or exited with error status\n");
//        return -1;
//    }

//    return 0;
