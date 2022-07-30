.RECIPEPREFIX = _
.DEFAULT_GOAL = build/target/SecretGet

ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
    PRINT = dont know
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
    PRINT = echo
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
    PRINT = echo
endif

.PHONY: clean
.PHONY: test

PATHTARGET = build/target

$(PATHTARGET)/SecretGet : $(PATHTARGET)/SecretGet.o
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Linking...\n\n"
_ gcc $(PATHTARGET)/SecretGet.o -o $(PATHTARGET)/SecretGet

$(PATHTARGET)/SecretGet.o : src/Main.c
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g src/Main.c -o $(PATHTARGET)/SecretGet.o


PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)*.c)

COMPILE=gcc -c
LINK=gcc
DEPEND=gcc -MM -MG -MF
CFLAGS=-I. -I$(PATHU) -I$(PATHS) -DTEST

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT) )

PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

test: $(BUILD_PATHS) $(RESULTS)
_ @echo "-----------------------\nIGNORES:\n-----------------------"
_ @echo "$(IGNORE)"
_ @echo "-----------------------\nFAILURES:\n-----------------------"
_ @echo "$(FAIL)"
_ @echo "-----------------------\nPASSED:\n-----------------------"
_ @echo "$(PASSED)"
_ @echo "\nDONE"

$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
_ -./$< > $@ 2>&1

$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o $(PATHO)unity.o #$(PATHD)Test%.d
_ $(LINK) -o $@ $^

$(PATHO)%.o:: $(PATHT)%.c
_ $(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHS)%.c
_ $(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
_ $(COMPILE) $(CFLAGS) $< -o $@

$(PATHD)%.d:: $(PATHT)%.c
_ $(DEPEND) $@ $<

$(PATHB):
_ $(MKDIR) $(PATHB)

$(PATHD):
_ $(MKDIR) $(PATHD)

$(PATHO):
_ $(MKDIR) $(PATHO)

$(PATHR):
_ $(MKDIR) $(PATHR)

clean:
_ $(CLEANUP) $(PATHO)*.o
_ $(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
_ $(CLEANUP) $(PATHR)*.txt
_ $(CLEANUP) $(PATHTARGET)/*

.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt
