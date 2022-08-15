#include <stdio.h>

#include "Bitwarden.h"


int searchInBitwarden(struct SecretGetState *state) {
    addSecret(state, "bw-mock-secret-1", "asdfgh");
    addSecret(state, "a-bw-mock-secret-2", "asdfghxxxxx");

    return 0;
}

