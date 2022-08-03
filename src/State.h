#define STORAGE_BITWARDEN 1
#define STORAGE_ELSE 2

int init();
int cleanUp();

int getStorage();
int setStorage(int storageKey);
int setSecretName(char *secretName);

