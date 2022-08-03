#include <stdio.h>
#include "State.h"

int findSecretAndAddToClipboard() {
    char *name = "unknown";

    switch (getStorage()) {
        case STORAGE_BITWARDEN: { name = "Bitwarden"; break; }
        case STORAGE_ELSE: { name = "Something Else"; break; }
    }

    printf("find secret and add to clipboard for: %s\n", name);
}

