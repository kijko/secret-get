.RECIPEPREFIX = _

version=1.1.1

ifeq ($(OS), Windows_NT)
    platform=win32
else
    platform=linux
endif # other platforms not supported

build_dir=build
src_dir=src

ifeq ($(platform),win32)
    lib_headers=C:\lib\include
    get_obj_path = $(build_dir)\$(1).o
		get_src_path = $(src_dir)\$(1).c
    compile = powershell -noprofile .\compile.ps1 $(src_dir) $(build_dir) $(1) $(version) $(lib_headers)
else
    get_obj_path = $(build_dir)/$(1).o
    get_src_path = $(src_dir)/$(1).c
    compile = chmod +x ./compile.sh && ./compile.sh $(src_dir) $(build_dir) $(1) $(version)
endif

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

bitwarden_list_module=BitwardenList_$(platform)
$(call get_obj_path,$(bitwarden_list_module)) : $(call get_src_path,$(bitwarden_list_module))
_ $(call compile,$(bitwarden_list_module))

clipboard_module=Clipboard_$(platform)
$(call get_obj_path,$(clipboard_module)) : $(call get_src_path,$(clipboard_module))
_ $(call compile,$(clipboard_module))

ifeq ($(platform),win32)
    yajl_adapter_module=YajlAdapter_win32

$(call get_obj_path,$(yajl_adapter_module)) : $(call get_src_path,$(yajl_adapter_module))
_ $(call compile,$(yajl_adapter_module))

    all_modules=$(main_module) \
                $(state_module) \
                $(secret_get_module) \
                $(bitwarden_module) \
                $(bitwarden_list_module) \
                $(clipboard_module) \
								$(yajl_adapter_module)

else
    all_modules=$(main_module) \
                $(state_module) \
                $(secret_get_module) \
                $(bitwarden_module) \
                $(bitwarden_list_module) \
                $(clipboard_module)

endif

objects=$(foreach module,$(all_modules),$(call get_obj_path,$(module)))

ifeq ($(platform),win32)
    target=$(build_dir)\secret-get.$(version).exe
    link=powershell -noprofile .\link.ps1 $(1) $(2)
else
    target=$(build_dir)/secret-get.$(version).out
    link=chmod +x ./link.sh && ./link.sh $(1) $(2)
endif

$(target) : $(objects)
_ $(call link,$(target),$(objects))

.DEFAULT_GOAL = $(target)

.PHONY: clean

ifeq ($(platform),win32)
    rm_build=powershell -noprofile "if (Test-Path $(build_dir)) { Remove-Item -Recurse -Force $(build_dir); }"
else
    rm_build=rm -fr $(build_dir)
endif

clean:
_ $(call rm_build)




.PHONY: install
.PHONY: uninstall

# TODO install/uninstall
install_dir=/opt/secret-get
install: $(target)
_ chmod +x ./install.sh && ./install.sh $(target) $(version) $(install_dir)

uninstall: 
_ chmod +x ./uninstall.sh && ./uninstall.sh $(install_dir)
#


