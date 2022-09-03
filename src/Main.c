#include <stdio.h>
#include <argp.h>
#include <string.h>

#include "sglib.h"

#include "SecretGet.h"

static struct SecretGetState *state;
static int setStateDefaults();
static int parseOptions(int key, char *arg, struct argp_state *state);

int main(int argc, char **argv) {
    state = init();
    if (state == NULL) {
        printf("Error initializing state\n");
        
        return 1;
    }

    int defaultResult = setStateDefaults();
    if (defaultResult != 0) {
        printf("Non-zero status setting default state values\n");
        
        return defaultResult;
    }

    struct argp_option options[] = {
         { "storage", 's', "ENUM", 0, "Choose storage. Supported: [BW, EL]. Default: BW", 0 },
         { 0 }
    };
    struct argp argp = { options, parseOptions, "secretNameRegex", 0, 0, 0, 0 }; 

    int parseResult = argp_parse(&argp, argc, argv, 0, 0, 0);
    
    if (parseResult != 0) {
        printf("Non-zero(%d) parse result. Exiting...\n", parseResult);

        return parseResult;
    }
    
    int searchResult;
    if ((searchResult = findSecret(state)) != 0) {
        if (searchResult == 5) {
            fprintf(stderr, "Secret regex cannot be empty\n");
        } else {
            fprintf(stderr, "Non-zero(%d) search result. Exiting...\n", searchResult);
        }
        
        return searchResult;
    } 

    int copyResult;
    if ((copyResult = copyToClipboard(state)) != 0) {
        printf("Non-zero(%d) coping result. Exiting...\n", copyResult);
        
        return copyResult;
    } else printf("Secret copied to clipboard\n");
    

    cleanUp(state);

    return 0;
}

static int setStateDefaults() {
    return setStorage(state, STORAGE_BITWARDEN);
}

static int parseOptions(int key, char *arg, struct argp_state *argpState) {
    switch (key) {
        case 0: {
            if (setSecretName(state, arg) != 0) {
                printf("Error setting secret name regex\n");
                
                return ARGP_KEY_ERROR;
            }
            break;
        }
        case 's': {
            if (strcmp(arg, "BW") == 0) {
                if (setStorage(state, STORAGE_BITWARDEN) != 0) {
                    printf("Error setting bitwarden as storage");
                    
                    return ARGP_KEY_ERROR;
                }
            } else if (strcmp(arg, "EL") == 0) {
                if (setStorage(state, STORAGE_ELSE) != 0) {
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
