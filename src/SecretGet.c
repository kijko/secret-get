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

            return 4;
        }
        default: {
            printf("SecretGet::findSecret - unknown storage\n");

            return 3;
        }
    }

    struct Secret *each;
    int i = 1;
    SGLIB_LIST_MAP_ON_ELEMENTS(
        struct Secret,
        state->first,
        each,
        next,
        { 
            printf("%d. %s\n", i, each->name); 
            i++;
        }
    );

    int choosen;
    scanf("%d", &choosen);

    if (choosen < 1 || choosen > i) {
        printf("Not found\n");
        
        return 5;
    }

    i = 1;
    SGLIB_LIST_MAP_ON_ELEMENTS(
        struct Secret,
        state->first,
        each,
        next,
        { 
            if (i == choosen) {
               printf("Choosen: %s\n", each->name);
            }

            i++;
        }
    );

    return 0;
}

int copyToClipboard(struct SecretGetState *state) {
    printf("Coping to clipboard...\n");

    return 0;
}


