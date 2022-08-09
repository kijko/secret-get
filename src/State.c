#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "State.h"

#define SECRET_NAME_BUFFER_SIZE 130

struct SecretGetState * init() {
    struct SecretGetState *state = 
                (struct SecretGetState *) malloc(sizeof (struct SecretGetState));
    state->secretName = (char *) malloc((sizeof (char)) * SECRET_NAME_BUFFER_SIZE);
    if (state == NULL) {
        printf("State::init - cannot allocate memory");

        return NULL;
    }

    return state;
}

int cleanUp(struct SecretGetState *state) {
    free(state->secretName);
    free(state);

    return 0;
}

int getStorage(struct SecretGetState *state) {
    return state->storageKey;
}

int setStorage(struct SecretGetState *state, int storageKey) {
    if (
        storageKey == STORAGE_BITWARDEN
        || storageKey == STORAGE_ELSE
    ) {
        state->storageKey = storageKey;

        return 0;
    }

    return 1;
}

int setSecretName(struct SecretGetState *state, char *secretName) {
    if (secretName == NULL) return 1;

    // todo err on empty of blank
    char * result = strncpy(state->secretName, secretName, SECRET_NAME_BUFFER_SIZE - 1);
    if (result == NULL) {
        printf("State::setSecretName - cannot copy secret name");
        
        return 1;
    }

    return 0;
}

char * getSecretName(struct SecretGetState *state) {
    return state->secretName;
}


