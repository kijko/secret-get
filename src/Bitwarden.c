#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "yajl/yajl_tree.h"

#include "Bitwarden.h"
#include "BitwardenList.h"

#define BW_BUFFER_SIZE 4 * 1024

//#if defined(__linux__)
//
//#elif defined(__WIN32__)
//
//#endif

typedef struct { yajl_val *values; size_t len; } * YajlArray;
typedef struct { const char **keys; yajl_val *values; size_t len;} * YajlObject;

int parseAndAddToState(const char *json, struct SecretGetState *state);

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
                    
                    const char *usernamePath[] = { "login", "username", (const char *) 0 };
                    yajl_val username = yajl_tree_get(item, usernamePath, yajl_t_string);

                    if (name != NULL && pw != NULL) {
                        char *nameStr = YAJL_GET_STRING(name);
                        char *pwStr = YAJL_GET_STRING(pw);
                        char *usernameStr = username != NULL ? YAJL_GET_STRING(username) : NULL;

                        if (nameStr != NULL && pwStr != NULL) {
                            addSecret(state, nameStr, pwStr, usernameStr);
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

