
gcc -std=c17 -x c -Wextra -g src\Main.c src\State.c src\SecretGet.c src\Bitwarden.c src\YajlAdapter_win32.c src\BitwardenList_win32.c src\Clipboard_win32.c -I "C:\lib\include" -o .\srg.exe

