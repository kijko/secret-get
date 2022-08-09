#include <stdio.h>

#include "State.h"

#include "SecretProvider.h"
#include "Bitwarden.h"


int findSecret(struct SecretGetState *state) {
    char *name = "unknown";

    switch (getStorage(state)) {
        case STORAGE_BITWARDEN: { name = "Bitwarden"; break; }
        case STORAGE_ELSE: { name = "Something Else"; break; }
    }

    printf("find secret and add to clipboard for: %s\n", name);

    return 0;
}

int copyToClipboard(struct SecretGetState *state) {
    printf("Coping to clipboard...\n");

    return 0;
}


