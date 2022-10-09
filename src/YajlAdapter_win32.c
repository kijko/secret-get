#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "yajl/yajl_tree.h"

yajl_val yajl_tree_parse(
    const char *input,
    char *error_buffer,
    size_t error_buffer_size
) {
    
    // Object 1
    const char **login1Keys = (const char **) malloc(sizeof(char *) * 2);
    login1Keys[0] = strcpy((char *) malloc(sizeof(char) * 32), "username");
    login1Keys[1] = strcpy((char *) malloc(sizeof(char) * 32), "password");
    
    
    struct yajl_val_s *username1 = 
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*username1).type = yajl_t_string;
    (*username1).u.string = strcpy((char *) malloc(sizeof(char) * 32), "username1");

    struct yajl_val_s *password1 =
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*password1).type = yajl_t_string;
    (*password1).u.string = strcpy((char *) malloc(sizeof(char) * 32), "password1");


    struct yajl_val_s **login1Values = 
        (struct yajl_val_s **) malloc(sizeof(struct yajl_val_s *) * 2);
    login1Values[0] = username1;
    login1Values[1] = password1;

    size_t login1Len = 2;

    struct yajl_val_s *login1 = 
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*login1).type = yajl_t_object;
    (*login1).u.object.keys = login1Keys;
    (*login1).u.object.values = login1Values;
    (*login1).u.object.len = login1Len;

    const char **object1Keys = (const char **) malloc(sizeof(char *));
    object1Keys[0] = strcpy((char *) malloc(sizeof(char) * 32), "login");

    struct yajl_val_s **object1Values = (struct yajl_val_s **) malloc(sizeof(struct yajl_val_s *));
    object1Values[0] = login1;
    size_t object1Len = 1;

    struct yajl_val_s *object1 = (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*object1).type = yajl_t_object;
    (*object1).u.object.keys = object1Keys;
    (*object1).u.object.values = object1Values;
    (*object1).u.object.len = object1Len;

    // object 2
    const char **login2Keys = (const char **) malloc(sizeof(char *) * 2);
    login2Keys[0] = strcpy((char *) malloc(sizeof(char) * 32), "username");
    login2Keys[1] = strcpy((char *) malloc(sizeof(char) * 32), "password");
    
    
    struct yajl_val_s *username2 = 
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*username2).type = yajl_t_string;
    (*username2).u.string = strcpy((char *) malloc(sizeof(char) * 32), "username2");

    struct yajl_val_s *password2 =
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*password2).type = yajl_t_string;
    (*password2).u.string = strcpy((char *) malloc(sizeof(char) * 32), "password2");


    struct yajl_val_s **login2Values = 
        (struct yajl_val_s **) malloc(sizeof(struct yajl_val_s *) * 2);
    login2Values[0] = username2;
    login2Values[1] = password2;

    size_t login2Len = 2;

    struct yajl_val_s *login2 = 
        (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*login2).type = yajl_t_object;
    (*login2).u.object.keys = login2Keys;
    (*login2).u.object.values = login2Values;
    (*login2).u.object.len = login2Len;

    const char **object2Keys = (const char **) malloc(sizeof(char *));
    object2Keys[0] = strcpy((char *) malloc(sizeof(char) * 32), "login");

    struct yajl_val_s **object2Values = (struct yajl_val_s **) malloc(sizeof(struct yajl_val_s *));
    object2Values[0] = login2;
    size_t object2Len = 1;

    struct yajl_val_s *object2 = (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*object2).type = yajl_t_object;
    (*object2).u.object.keys = object2Keys;
    (*object2).u.object.values = object2Values;
    (*object2).u.object.len = object2Len;

    // array
    struct yajl_val_s *array = (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    (*array).type = yajl_t_array;
    (*array).u.array.values = (struct yajl_val_s **) malloc(sizeof(struct yajl_val_s *) * 2);
    (*array).u.array.values[0] = object1;
    (*array).u.array.values[1] = object2;
    (*array).u.array.len = 1;

    return array;
}

yajl_val yajl_tree_get(
    yajl_val parent,
    const char ** path,
    yajl_type type    
) {
    char *value = "mock-value";

    struct yajl_val_s *result = (struct yajl_val_s *) malloc(sizeof(struct yajl_val_s));
    result->type = yajl_t_string;
    result->u.string = strcpy((char *) malloc(sizeof(char) * 32), value);
    
    return result;
}

void yajl_tree_free(yajl_val v) {
}

