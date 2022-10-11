.RECIPEPREFIX = _

# todo pass version using some gcc param
version=1.1.0


#ifeq ($(OS),Windows_NT)
#  ifeq ($(shell uname -s),) # not in a bash-like shell
#	CLEANUP = del /F /Q
#	MKDIR = mkdir
#    PRINT = dont know
#  else # in a bash-like shell, like msys
#	CLEANUP = rm -f
#	MKDIR = mkdir -p
#    PRINT = echo
#  endif
#	TARGET_EXTENSION=exe
#else
#	CLEANUP = rm -f
#	MKDIR = mkdir -p
#	TARGET_EXTENSION=out
#    PRINT = echo
#endif


build_dir=build
src_dir=src
get_obj_path = $(build_dir)/$(1).o
get_src_path = $(src_dir)/$(1).c
compile = chmod +x ./compile.sh && ./compile.sh $(src_dir) $(build_dir) $(1)

main_module=Main
$(call get_obj_path,$(main_module)) : $(call get_src_path,$(main_module))
_ $(call compile,$(main_module))

state_module=State
$(call get_obj_path,$(state_module)) : $(call get_src_path,$(state_module))
_ $(call compile,$(state_module))

secret_get_module=SecretGet
$(call get_obj_path,$(secret_get_module)) : $(call get_src_path,$(secret_get_module))
_ $(call compile,$(secret_get_module))

bitwarden_module=Bitwarden
$(call get_obj_path,$(bitwarden_module)) : $(call get_src_path,$(bitwarden_module))
_ $(call compile,$(bitwarden_module))

bitwarden_list_module=BitwardenList_linux
$(call get_obj_path,$(bitwarden_list_module)) : $(call get_src_path,$(bitwarden_list_module))
_ $(call compile,$(bitwarden_list_module))

clipboard_module=Clipboard_linux
$(call get_obj_path,$(clipboard_module)) : $(call get_src_path,$(clipboard_module))
_ $(call compile,$(clipboard_module))

all_modules=$(main_module) \
            $(state_module) \
            $(secret_get_module) \
            $(bitwarden_module) \
            $(bitwarden_list_module) \
            $(clipboard_module)

objects=$(foreach module,$(all_modules),$(call get_obj_path,$(module)))
target=$(build_dir)/secret-get.$(version).out

link=chmod +x ./link.sh && ./link.sh $(1) $(2)

$(target) : $(objects)
_ $(call link,$(target),$(objects))

.DEFAULT_GOAL = $(target)

.PHONY: install
.PHONY: uninstall
.PHONY: clean


install_dir=/opt/secret-get
install: $(target)
_ chmod +x ./install.sh && ./install.sh $(target) $(version) $(install_dir)

uninstall: 
_ chmod +x ./uninstall.sh && ./uninstall.sh $(install_dir)

clean:
_ rm -fr build

.PHONY: build-win
build-win:
_ powershell -noprofile .\build.ps1

