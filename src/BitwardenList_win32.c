#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BitwardenList.h"

static long slurp(char const *path, char **buf, bool add_nul);

int bwList(char *buff, int buffSize, char *secretName) {
    long file_size;

    file_size = slurp("./resources/test-bw.json", &buff, true);

    if (file_size < 0L) {
        return 1;
    }

    return 0;
}

long slurp(char const* path, char **buf, bool add_nul) {
    FILE  *fp;
    size_t fsz;
    long   off_end;
    int    rc;

    /* Open the file */
    fp = fopen(path, "rb");
    if( NULL == fp ) {
        return -1L;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if( 0 != rc ) {
        return -1L;
    }

    /* Byte offset to the end of the file (size) */
    if( 0 > (off_end = ftell(fp)) ) {
        return -1L;
    }
    fsz = (size_t)off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = malloc( fsz+(int)add_nul );
    if( NULL == *buf ) {
        return -1L;
    }

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* Slurp file into buffer */
    if( fsz != fread(*buf, 1, fsz, fp) ) {
        free(*buf);
        return -1L;
    }

    /* Close the file */
    if( EOF == fclose(fp) ) {
        free(*buf);
        return -1L;
    }

    if( add_nul ) {
        /* Make sure the buffer is NUL-terminated, just in case */
        (*buf)[fsz] = '\0';
    }

    /* Return the file size */
    return (long)fsz;
}


