#include <stdio.h>
#include <string.h>

#include "yajl/yajl_tree.h"

#include "Bitwarden.h"

typedef struct { yajl_val *values; size_t len; } * YajlArray;
typedef struct { const char **keys; yajl_val *values; size_t len;} * YajlObject;

int searchInBitwarden(struct SecretGetState *state) {
    const char *mock= "[{\"object\":\"item\",\"id\":\"31da44d1-5ef5-4540-841e-add801602f76\",\"organizationId\":null,\"folderId\":null,\"type\":1,\"reprompt\":0,\"name\":\"Google official\",\"notes\":null,\"favorite\":false,\"login\":{\"username\":\"hello1\",\"password\":\"tomato1\",\"totp\":null,\"passwordRevisionDate\":null},\"collectionIds\":[],\"revisionDate\":\"2021-11-06T21:22:16.066Z\",\"deletedDate\":null},{\"object\":\"item\",\"id\":\"c3bed220-2de0-4367-8c1a-ae4c015a2e4c\",\"organizationId\":null,\"folderId\":null,\"type\":1,\"reprompt\":0,\"name\":\"WikiGoogle\",\"notes\":null,\"favorite\":false,\"login\":{\"username\":\"hello2\",\"password\":\"tomato2\",\"totp\":null,\"passwordRevisionDate\":null},\"collectionIds\":[],\"revisionDate\":\"2022-03-02T21:00:24.350Z\",\"deletedDate\":null}]";

    char errorBuffer[1024];
    yajl_val json = yajl_tree_parse(mock, errorBuffer, sizeof (errorBuffer)); // size of is 1024?
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
            // todo print error
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
//    yajl_val json;
//    char errorBuffer[1024];
//
//    const char *mock = "{    \"BuildType\": \"ephemeral\",    \"DistServer\": \"http://browserplus.yahoo.com\",    \"SecondaryDistServers\": [      \"http://first.fictional.server\",      \"http://second.fictional.server\"    ],    \"Logging\" :    {        \"level\": \"BP_LOG_LEVEL\",        \"dest\": \"BP_LOG_DEST\",        \"layout\": \"standard\",        \"timeFormat\": \"utc\",        \"fileRolloverKB\": 2048,        \"serviceLogMode\": \"combined\"    },    \"Options\":\"\",    \"MaxIdleSecs\": 5,    \"UsageReporting\":    {       \"enabled\": true,       \"url\": false,       \"id\": true    },\"ServiceUpdatePollPeriod\": 86400} ";
//    const char *mock2 = "{    \"secrets\": [ { \"name\": \"first\" }, { \"name\": \"second\" }] } ";
//
//    json = yajl_tree_parse((const char *) mock2, errorBuffer, sizeof(errorBuffer));
//    if (json == NULL) {
//        fprintf(stderr, "parse_error: ");
//        if (strlen(errorBuffer)) fprintf(stderr, " %s", errorBuffer);
//        else fprintf(stderr, "unknown error");
//        fprintf(stderr, "\n");
//        return 1;
//    }

//    {
//        const char * path[] = { "secrets", (const char *) 0 };
//        struct yajl_val_s *v = yajl_tree_get(json, path, yajl_t_array);
//        //printf("isarray: %d\n", YAJL_IS_ARRAY(v));
//        if (v) {
//            struct { yajl_val *values; size_t len; } *arr;
//            arr = YAJL_GET_ARRAY(v); 
//            
//            struct { 
//                 const char **keys;
//                 yajl_val *values;
//                 size_t len;
//             } *object;
//            char *str;
//            char secretName[2];
//
//            for (size_t i = 0; i < arr->len; i++) {
//                object = YAJL_GET_OBJECT(arr->values[i]);
//                str = YAJL_GET_STRING(object->values[0]);
//                secretName[0] = i + '0';
//                secretName[1] = '\0';
//
//                addSecret(state, secretName, str);
//            }
//
//        } else {
//            printf("no such node: %s/%s\n", path[0], path[1]);
//
//        }
//    }


//    return 0;
}

