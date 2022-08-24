#include <stdio.h>
#include <string.h>

#include "yajl/yajl_tree.h"

#include "Bitwarden.h"


int searchInBitwarden(struct SecretGetState *state) {
    yajl_val json;
    char errorBuffer[1024];

    const char *mock = "{    \"BuildType\": \"ephemeral\",    \"DistServer\": \"http://browserplus.yahoo.com\",    \"SecondaryDistServers\": [      \"http://first.fictional.server\",      \"http://second.fictional.server\"    ],    \"Logging\" :    {        \"level\": \"BP_LOG_LEVEL\",        \"dest\": \"BP_LOG_DEST\",        \"layout\": \"standard\",        \"timeFormat\": \"utc\",        \"fileRolloverKB\": 2048,        \"serviceLogMode\": \"combined\"    },    \"Options\":\"\",    \"MaxIdleSecs\": 5,    \"UsageReporting\":    {       \"enabled\": true,       \"url\": false,       \"id\": true    },\"ServiceUpdatePollPeriod\": 86400} ";
    const char *mock2 = "{    \"secrets\": [ { \"name\": \"first\" }, { \"name\": \"second\" }] } ";

    json = yajl_tree_parse((const char *) mock2, errorBuffer, sizeof(errorBuffer));
    if (json == NULL) {
        fprintf(stderr, "parse_error: ");
        if (strlen(errorBuffer)) fprintf(stderr, " %s", errorBuffer);
        else fprintf(stderr, "unknown error");
        fprintf(stderr, "\n");
        return 1;
    }

    {
        const char * path[] = { "secrets", (const char *) 0 };
        struct yajl_val_s *v = yajl_tree_get(json, path, yajl_t_array);
        //printf("isarray: %d\n", YAJL_IS_ARRAY(v));
        if (v) {
//            struct { yajl_val *values; size_t len; } *arr;
//            arr = &(v)->u.array; 
//            
//            struct { 
//                 const char **keys; /*< Array of keys */ 
//                 yajl_val *values; /*< Array of values. */ 
//                 size_t len; /*< Number of key-value-pairs. */ 
//             } *object;
//            char * str;
//            for (size_t i = 0; i < arr->len; i++) {
//                object = &(arr->values[i])->u.object;
//                str = (object->values[0])->u.string;
//                printf("secret %ld - name: %s\n", i, YAJL_GET_STRING(object->values[0]));
//            }

            struct { yajl_val *values; size_t len; } *arr;
            arr = YAJL_GET_ARRAY(v); 
            
            struct { 
                 const char **keys;
                 yajl_val *values;
                 size_t len;
             } *object;
            char *str;
            char secretName[2];

            for (size_t i = 0; i < arr->len; i++) {
                object = YAJL_GET_OBJECT(arr->values[i]);
                str = YAJL_GET_STRING(object->values[0]);
                secretName[0] = i + '0';
                secretName[1] = '\0';

                addSecret(state, secretName, str);
            }

        } else {
            printf("no such node: %s/%s\n", path[0], path[1]);

        }
    }

    yajl_tree_free(json);

    return 0;
}

