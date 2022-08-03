#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "State.h"

struct SecretGetState {
    int storageKey;
    char *secretName;
};

static struct SecretGetState *state;

int init() {
    state = (struct SecretGetState *) malloc(sizeof *state);
    if (state == NULL) {
        printf("State::init - cannot allocate memory");

        return 1;
    }

    return 0;
}

int cleanUp() {
    free(state);

    return 0;
}

int getStorage() {
    return state->storageKey;
}

int setStorage(int storageKey) {
    if (
        storageKey == STORAGE_BITWARDEN
        || storageKey == STORAGE_ELSE
    ) {
        state->storageKey = storageKey;

        return 0;
    }

    return 1;
}

int setSecretName(char *secretName) {
    if (secretName == NULL) return 1;

    state->secretName = secretName;

    return 0;
}

