#include <stdio.h>
#include <windows.h>

#include "yajl/yajl_tree.h"

typedef yajl_val (*TreeParse)(const char *, char *, size_t);
typedef yajl_val (*TreeGet)(yajl_val, const char **, yajl_type);  
typedef void (*TreeFree)(yajl_val);

static HMODULE dll = NULL;

static int loadLib();

yajl_val yajl_tree_parse(
    const char *input,
    char *error_buffer,
    size_t error_buffer_size
) {
    if (loadLib()) {
        printf("Cannot load libyajl.\n");
        return NULL;
    }

    TreeParse parse = (TreeParse) GetProcAddress(dll, "yajl_tree_parse");

    return parse(input, error_buffer, error_buffer_size);
}

yajl_val yajl_tree_get(
    yajl_val parent,
    const char ** path,
    yajl_type type    
) {
    if (loadLib()) {
        printf("Cannot load libyajl.\n");
        return NULL;
    }

    TreeGet get = (TreeGet) GetProcAddress(dll, "yajl_tree_get");

    return get(parent, path, type);
}

void yajl_tree_free(yajl_val v) {
    if (loadLib()) {
        printf("Cannot load libyajl.\n");

        return;
    }

    TreeFree free = (TreeFree) GetProcAddress(dll, "yajl_tree_free");

    free(v);
}

int loadLib() {
    char *p = "C:\\lib\\libyajl.dll";
    dll = LoadLibrary(p);

    if (dll == NULL) {
        printf("Failed to load libyajl.dll [error='%d', path='%s']", GetLastError(), p);

        return 1;
    }

    return 0;
}

