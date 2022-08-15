#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sglib.h"

#include "State.h"


#define COMMON_STRING_BUFFER_SIZE 130
#define SECRET_LIST_CMP(s1, s2) (strcmp(s1->name, s2->name))

struct SecretGetState * init() {
    struct SecretGetState *state = 
                (struct SecretGetState *) malloc(sizeof (struct SecretGetState));
    state->storageKey = -1;
    state->secretName = (char *) malloc((sizeof (char)) * COMMON_STRING_BUFFER_SIZE);
    if (state == NULL) {
        printf("State::init - cannot allocate memory");

        return NULL;
    }
    state->first = NULL;

    return state;
}

int cleanUp(struct SecretGetState *state) {
    free(state->secretName);
    
    struct Secret *each;
    SGLIB_LIST_MAP_ON_ELEMENTS(struct Secret, state->first, each, next, { 
        free(each->name);
        free(each->value);
        free(each);
    });

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
    char *result = strncpy(state->secretName, secretName, COMMON_STRING_BUFFER_SIZE - 1);
    if (result == NULL) {
        printf("State::setSecretName - cannot copy secret name");
        
        return 1;
    }

    return 0;
}

char * getSecretName(struct SecretGetState *state) {
    return state->secretName;
}

int addSecret(
    struct SecretGetState *state,
    char *name,
    char *value) {

    struct Secret *elem = (struct Secret *) malloc(sizeof (struct Secret));
    if (elem == NULL) {
        printf("State::addSecret - cannot allocate memory for secret");
        
        return 1;
    }

    elem->name = (char *) malloc(sizeof(char) * COMMON_STRING_BUFFER_SIZE);
    if ((elem->name) == NULL) {
        printf("State::addSecret - cannot allocate memory for secret name");
        
        return 2;
    }

    elem->value = (char *) malloc(sizeof(char) * COMMON_STRING_BUFFER_SIZE);
    if ((elem->value) == NULL) {
        printf("State::addSecret - cannot allocate memory for secret value");
        
        return 3;
    }
    
    char *copyNameResult = strncpy(elem->name, name, COMMON_STRING_BUFFER_SIZE - 1);
    if (copyNameResult == NULL) {
        printf("State::addSecret - Cannot copy name");

        return 4;
    }

    char *copyValueResult = strncpy(elem->value, value, COMMON_STRING_BUFFER_SIZE - 1);
    if (copyValueResult == NULL) {
        printf("State::addSecret - Cannot copy value");

        return 5;
    }


    SGLIB_SORTED_LIST_ADD(struct Secret, state->first, elem, SECRET_LIST_CMP, next);
 
    return 0;
}


