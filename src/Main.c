#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "sglib.h"
#include "SecretGet.h"

//#if defined(__linux__)
//
//#elif defined(__WIN32__)
//
//#endif

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#ifndef SRG_VERSION
#   define SRG_VERSION unknown
#endif

#define VER STR(SRG_VERSION)

extern char *optarg;
extern int optind, opterr, optopt;
extern int getopt(int argc, char * const argv[], const char *optstring);

static struct SecretGetState *state;
static int setStateDefaults();

static int interpretArgs(int argc, char **argv);
static void printHelp(char *exeName);

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

    int interpretation = interpretArgs(argc, argv);
    if (interpretation != 0 && interpretation != 112) {
        printf("Non-zero(%d) status interpreting args\n", interpretation);       

        return interpretation;
    } else if (interpretation == 112) {
        return 0;
    }

    int searchResult;
    if ((searchResult = findSecret(state)) != 0) {
        if (searchResult == 5) {
            fprintf(stderr, "Secret regex cannot be empty\n");
        } else {
            fprintf(stderr, "Non-zero(%d) search result. Exiting...\n", searchResult);
        }
        
        return searchResult;
    } else {
        struct Secret *choosenSecret = getChoosenSecret(state);
        printf("Found secret: '%s'\n", choosenSecret->name);

        if (strlen(choosenSecret->username) == 0) {
            printf("No username\n");
        } else {
            printf("Username: %s\n", choosenSecret->username);
        }
    }

    int copyResult;
    if ((copyResult = copyToClipboard(state)) != 0) {
        printf("Non-zero(%d) coping result. Exiting...\n", copyResult);
        
        return copyResult;
    } else printf("Secret copied to clipboard\n");
    

    cleanUp(state);

    return 0;
}


static int interpretArgs(int argc, char **argv) {
    char *storage = "BW";
    char *secretName = NULL;

    int option;
    while ((option = getopt(argc, argv, "s:h")) != -1) {
        switch (option) {
            case 's':
                storage = optarg;

                break;

            case ':':
                printf("Option '-%c' needs a value\n", optopt);
                
                break;

            case 'h':
                printHelp(argv[0]);

                return 112;
            case '?':
                return 2;
      }
   }

   for (; optind < argc; optind++) {
        if (secretName == NULL) {
            secretName = argv[optind];
        }
   }

   if (secretName != NULL) {
        
        if (strcmp(storage, "BW") == 0) {
            if (setStorage(state, STORAGE_BITWARDEN) != 0) {
                printf("Error setting bitwarden as storage\n");
                
                return 10;
            }    
        } else if (strcmp(storage, "ELSE") == 0) {
            if (setStorage(state, STORAGE_ELSE) != 0) {
                printf("Error setting something else as storage\n");
                
                return 10;
            }
        } else {
                fprintf(stderr, "Unknown storage: %s\n", storage);
                
                return 10;
        }

        if (setSecretName(state, secretName) != 0) {
            printf("Error setting secret name regex\n");
            
            return 11;
        }

        return 0;
   } else {
        fprintf(stderr, "No secret name provided\n\n");
        printHelp(argv[0]);

        return 1;
   }
}

static void printHelp(char *exeName) {
    printf("secret-get v. %s\n", VER);
    printf("Usage:\n");
    printf("%s [options...] [secretName]\n\n", exeName);
    printf("Args:\n");
    printf("secretName -> Name of the secret for searching\n\n");
    printf("Available options:\n");
    printf("-s [storage] -> Switch between storages. Default: 'BW'. Available so far: 'BW', 'ELSE'\n");
}

static int setStateDefaults() {
    return setStorage(state, STORAGE_BITWARDEN);
}

