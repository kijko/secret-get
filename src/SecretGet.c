#include <stdio.h>

#include "State.h"

#include "sglib.h"

#include "SecretProvider.h"
#include "Bitwarden.h"

int findSecret(struct SecretGetState *state) {
    char *name = "unknown";

    switch (getStorage(state)) {
        case STORAGE_BITWARDEN: { name = "Bitwarden"; break; }
        case STORAGE_ELSE: { name = "Something Else"; break; }
    }

    printf("find secret '%s' in '%s'\n", state->secretName ,name);

    addSecret(state, "some secret", "asdfasda");
    addSecret(state, "my secret", "asdffff");
    addSecret(state, "aaa", "poqpwe");

    struct Secret *each;
    SGLIB_LIST_MAP_ON_ELEMENTS(struct Secret, state->first, each, next, {
        printf("name: %s value: %s\n", each->name, each->value); 
    });

    return 0;
}

int copyToClipboard(struct SecretGetState *state) {
    printf("Coping to clipboard...\n");

    return 0;
}


