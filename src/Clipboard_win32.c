#include <windows.h>
#include <stdio.h>
#include <string.h>

int copyToPlatformClipboard(char *text) {
    size_t len = strlen(text) + 1;

    if (len == 0) return 4;

    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(char));
    if (mem == NULL) {
        printf("Failed to allocate global memory [err='%d']", GetLastError());

        return 5;
    }

    memcpy(GlobalLock(mem), text, len);
    GlobalUnlock(mem);

    if (!OpenClipboard(0)) {
        printf("Failed to open clipboard [err='%d']", GetLastError());

        return 1;
    }

    EmptyClipboard();
    HANDLE clipboard = SetClipboardData(CF_TEXT, mem);

    if (clipboard == NULL) {
        printf("Failed to set clipboard data [err='%d']", GetLastError());

        CloseClipboard();

        return 2;
    }


    if (!CloseClipboard()) {
        printf("Failed to close clipboard [err='%d']", GetLastError());

        return 3;
    }

    return 0;
}

