#include <stdio.h>
#include <regex.h>

#include "sglib.h"

#include "Bitwarden.h"

int countSecrets(struct SecretGetState *state);
int chooseFromMany(struct SecretGetState *state);
int chooseFirst(struct SecretGetState *state);

int isBlank(char *str) { // doesnt work
    regex_t rg;
    printf("str: %s\n", str);

    return regcomp(&rg, "/^(?!\\s+$).*/", 0) == 0 ? 1 : 0;
}

int findSecret(struct SecretGetState *state) {
    if (isBlank(getSecretName(state))) {
        return 5;
    }    

    switch (getStorage(state)) {
        case STORAGE_BITWARDEN: {
            int searchResult;
            if ((searchResult = searchInBitwarden(state)) != 0) {
                printf("SecretGet::findSecret - Non-zero(%d) status searching in bitwarden\n", searchResult);

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

    int count = countSecrets(state);

    if (count > 0) {

        if (count > 1) {
           return chooseFromMany(state); 
        } else {
           int chooseResult = choose(state, 0);
           if (chooseResult == 0) {
                printf("Found: '%s'\n", state->first->name);
           }
           
           return chooseResult;
        }

    } else {
        printf("No secrets found\n");
    
        return 0;
    }

}

int chooseFromMany(struct SecretGetState *state) {
    int number = 1;

    struct Secret *each;
    SGLIB_LIST_MAP_ON_ELEMENTS(
        struct Secret,
        state->first,
        each,
        next,
        { 
            printf("%d. %s\n", number, each->name);
            number++;
        }
    );

    int choosen;
    int inputResult = scanf("%d", &choosen);
    if (inputResult == EOF) {
        fprintf(stderr, "EOF scanning number of secret\n");
        return 1;
    }

    if (choosen < 1 || choosen > number) {
        fprintf(stderr, "Incorrect number of secret: %d\n", choosen);
        return 2;
    }

    int chooseResult = 0;
    int counter = 1;    
    SGLIB_LIST_MAP_ON_ELEMENTS(
        struct Secret,
        state->first,
        each,
        next,
        { 
            if (counter == choosen) {

                chooseResult = choose(state, choosen - 1);
                if (chooseResult == 0) printf("Found '%s'\n", each->name);

            }
            counter++;
        }
    );

    return chooseResult;
}

int countSecrets(struct SecretGetState *state) {
    int count = 0;

    struct Secret *each;
    SGLIB_LIST_MAP_ON_ELEMENTS(
        struct Secret,
        state->first,
        each,
        next,
        { 
            count++;
        }
    );

    return count;
}

int copyToClipboard(struct SecretGetState *state) {
    printf("Coping to clipboard...\n");

    return 0;
}


