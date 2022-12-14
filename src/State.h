#define STORAGE_BITWARDEN 1
#define STORAGE_ELSE 2

struct Secret {
    char *name;
    char *value;
    char *username;
    struct Secret *next;
};

struct SecretGetState {
    int storageKey;
    char *secretName;
    struct Secret *first;
    int choosenIndex;
};

struct SecretGetState * init();
int cleanUp(struct SecretGetState *state);
int getStorage(struct SecretGetState *state);
int setStorage(struct SecretGetState *state, int storageKey);

// copies string to state
int setSecretName(struct SecretGetState *state, char *secretName);

char * getSecretName(struct SecretGetState *state);

// copies name, value strings
int addSecret(
        struct SecretGetState *state,
        char *name,
        char *value,
        char *username // NULLABLE
);

int choose(struct SecretGetState *state, int index);

struct Secret *getChoosenSecret(struct SecretGetState *state);

