./build/srg : ./build/srg.o ; \
gcc ./build/srg.o -o ./build/srg

./build/srg.o : ./main.c ; \
gcc -c -std=c17 -x c -Wextra -g main.c -o build/srg.o

clean : ; rm -r build/*

