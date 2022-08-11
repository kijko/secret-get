#include <stdio.h>

#include "sglib.h"

#include "Bitwarden.h"

int findSecret(struct SecretGetState *state) {
    switch (getStorage(state)) {
        case STORAGE_BITWARDEN: {
            int searchResult;
            if ((searchResult = searchInBitwarden(state)) != 0) {
                printf("SecretGet::findSecret - Non-zero(%d) status searching in bitwarden", searchResult);

                return searchResult;
            }            

            break; 
        }
        case STORAGE_ELSE: { 
            printf("It's example storage. It doesnt work\n");

            break; 
        }
        default: {
            printf("SecretGet::findSecret - unknown storage\n");

            return 3;
        }
    }

    return 0;
}

int copyToClipboard(struct SecretGetState *state) {
    printf("Coping to clipboard...\n");

    return 0;
}


