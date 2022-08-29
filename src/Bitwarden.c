#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "yajl/yajl_tree.h"

#include "Bitwarden.h"

#define BW_BUFFER_SIZE 4 * 1024

typedef struct { yajl_val *values; size_t len; } * YajlArray;
typedef struct { const char **keys; yajl_val *values; size_t len;} * YajlObject;

int parseAndAddToState(const char *json, struct SecretGetState *state);
int bwList(char *buff, int buffSize, char *secretName);

int searchInBitwarden(struct SecretGetState *state) {
    int returnCode = 0;

    char *jsonStr = (char *) malloc(BW_BUFFER_SIZE);
    
    int listResult = bwList(jsonStr, BW_BUFFER_SIZE, getSecretName(state));
    if (listResult == 0) {
        int parseResult = parseAndAddToState(jsonStr, state);
        
        if (parseResult != 0) {
            returnCode = 2;
        }

    } else {
        returnCode = 1;
    }
    
    free(jsonStr);

    return returnCode;
}

int bwList(char *buff, int buffSize, char *secretName) {
    char cmdBuff[160] = "\0";
    
    char *cmd = strncat(strcat(cmdBuff, "bw list items --search "), secretName, 128);
    
    FILE *fp;
    if ((fp = (FILE *) popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return 1;
    }

    while (fgets(buff, buffSize, fp) != NULL) {}

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return 2;
    }

    return 0;
}


int parseAndAddToState(const char *jsonStr, struct SecretGetState *state) {
    char errorBuffer[1024];
    yajl_val json = yajl_tree_parse(jsonStr, errorBuffer, sizeof (errorBuffer));
    int returnCode = 0;
    
    if (json != NULL) {
        YajlArray array = (YajlArray) YAJL_GET_ARRAY(json); 
        
        if (array != NULL && array->len > 0) {
            
            yajl_val item;
            for (size_t i = 0; i < array->len; i++) {
                item = array->values[i];

                if (YAJL_IS_OBJECT(item) && YAJL_GET_OBJECT(item)->len > 0) {

                    const char *namePath[] = { "name", (const char *) 0  };
                    yajl_val name = yajl_tree_get(item, namePath, yajl_t_string); 

                    const char *pwPath[] = { "login", "password", (const char *) 0 };
                    yajl_val pw = yajl_tree_get(item, pwPath, yajl_t_string); 

                    if (name != NULL && pw != NULL) {
                        char *nameStr = YAJL_GET_STRING(name);
                        char *pwStr = YAJL_GET_STRING(pw);

                        if (nameStr != NULL && pwStr != NULL) {
                            addSecret(state, nameStr, pwStr);
                        }
                    }
                }

            }
           
        } else {
            returnCode = 2;
        }
        
    } else {
        fprintf(stderr, "parse_error: ");

        if (strlen(errorBuffer)) {
            fprintf(stderr, " %s", errorBuffer);
        } else {
            fprintf(stderr, "unknown error");
        }

        fprintf(stderr, "\n");
        
        returnCode = 1;
    }

    yajl_tree_free(json);

    return returnCode;
}

