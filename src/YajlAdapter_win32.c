#include <stdio.h>

#include "yajl/yajl_tree.h"

yajl_val yajl_tree_parse(
    const char *input,
    char *error_buffer,
    size_t error_buffer_size
) {
    printf("yajl_tree_parse not implemented yet.\n");

    return NULL;
}

yajl_val yajl_tree_get(
    yajl_val parent,
    const char ** path,
    yajl_type type    
) {
    printf("yajl_tree_get not implemented yet.\n");

    return NULL;
}

void yajl_tree_free(yajl_val v) {
    printf("yajl_tree_free not implemented yet.\n");
}

