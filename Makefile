.RECIPEPREFIX = _

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


APP_VER = 1-1-0

PATHTARGET = build/target
OUT_NAME = SecretGet_$(APP_VER).$(TARGET_EXTENSION)
OUT = $(PATHTARGET)/$(OUT_NAME)

.DEFAULT_GOAL = $(OUT)


.PHONY: clean
.PHONY: test
.PHONY: install
.PHONY: uninstall
.PHONY: build


install: $(OUT)
_ sudo mkdir -p /opt/secret-get/$(APP_VER)
_ sudo cp $(OUT) /opt/secret-get/$(APP_VER)/exec
_ sudo rm -f /usr/bin/srg
_ sudo ln -s /opt/secret-get/$(APP_VER)/exec /usr/bin/srg

uninstall: 
_ sudo rm /usr/bin/srg
_ sudo rm -fr /opt/secret-get

build: $(OUT)

MAIN_MODULE = Main
MAIN_MODULE_SRC = src/$(MAIN_MODULE).c
MAIN_MODULE_OBJ = $(PATHTARGET)/$(MAIN_MODULE).o

$(MAIN_MODULE_OBJ) : $(MAIN_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(MAIN_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(MAIN_MODULE_SRC) -o $(MAIN_MODULE_OBJ)

STATE_MODULE = State
STATE_MODULE_SRC = src/$(STATE_MODULE).c
STATE_MODULE_OBJ = $(PATHTARGET)/$(STATE_MODULE).o

$(STATE_MODULE_OBJ) : $(STATE_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(STATE_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(STATE_MODULE_SRC) -o $(STATE_MODULE_OBJ)

SECRET_GET_MODULE = SecretGet
SECRET_GET_MODULE_SRC = src/$(SECRET_GET_MODULE).c
SECRET_GET_MODULE_OBJ = $(PATHTARGET)/$(SECRET_GET_MODULE).o

$(SECRET_GET_MODULE_OBJ) : $(SECRET_GET_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(SECRET_GET_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(SECRET_GET_MODULE_SRC) -o $(SECRET_GET_MODULE_OBJ)

BITWARDEN_MODULE = Bitwarden
BITWARDEN_MODULE_SRC = src/$(BITWARDEN_MODULE).c
BITWARDEN_MODULE_OBJ = $(PATHTARGET)/$(BITWARDEN_MODULE).o

$(BITWARDEN_MODULE_OBJ) : $(BITWARDEN_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(BITWARDEN_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(BITWARDEN_MODULE_SRC) -o $(BITWARDEN_MODULE_OBJ)

BITWARDEN_LIST_MODULE = BitwardenList
BITWARDEN_LIST_MODULE_PLATFORM = linux
BITWARDEN_LIST_MODULE_SRC = src/$(BITWARDEN_LIST_MODULE)_$(BITWARDEN_LIST_MODULE_PLATFORM).c
BITWARDEN_LIST_MODULE_OBJ = $(PATHTARGET)/$(BITWARDEN_LIST_MODULE).o

$(BITWARDEN_LIST_MODULE_OBJ) : $(BITWARDEN_LIST_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(BITWARDEN_LIST_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(BITWARDEN_LIST_MODULE_SRC) -o $(BITWARDEN_LIST_MODULE_OBJ)

CLIPBOARD_MODULE = Clipboard
CLIPBOARD_MODULE_PLATFORM = linux
CLIPBOARD_MODULE_SRC = src/$(CLIPBOARD_MODULE)_$(CLIPBOARD_MODULE_PLATFORM).c
CLIPBOARD_MODULE_OBJ = $(PATHTARGET)/$(CLIPBOARD_MODULE).o

$(CLIPBOARD_MODULE_OBJ) : $(CLIPBOARD_MODULE_SRC)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Compiling $(CLIPBOARD_MODULE)...\n\n"
_ gcc -c -std=gnu17 -x c -Wextra -g $(CLIPBOARD_MODULE_SRC) -o $(CLIPBOARD_MODULE_OBJ)

PROJECT_OBJ = $(MAIN_MODULE_OBJ) \
    $(STATE_MODULE_OBJ) \
    $(SECRET_GET_MODULE_OBJ) \
    $(BITWARDEN_MODULE_OBJ) \
    $(BITWARDEN_LIST_MODULE_OBJ) \
    $(CLIPBOARD_MODULE_OBJ)

$(OUT) : $(PROJECT_OBJ)
_ $(MKDIR) $(PATHTARGET)
_ $(PRINT) "Linking...\n\n"
_ gcc $(PROJECT_OBJ) -lyajl -o $(OUT)







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
