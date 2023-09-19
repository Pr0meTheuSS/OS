Compiler=gcc

Wkeys= -static -Werror -Wall -Wextra -pedantic
OptKeys= -O0
DbgKeys= -g3

SrcDir= Src
ExecDir= Exec

TargetName=app

.PHONY: build
build:
	make clean
	mkdir $(ExecDir)
	$(Compiler) $(wildcard $(SrcDir)/*.c) $(Wkeys) $(OptKeys) $(DbgKeys) -o $(ExecDir)/$(TargetName)
	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_dir
	ln $(ExecDir)/$(TargetName) $(ExecDir)/lookup_dir
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_dir

	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_file
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_file
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_file
	
	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_symlink
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_symlink
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_symlink_content
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_symlink

	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_hardlink
	
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_hardlink

.PHONY: clean
clean:
	rm -rf $(ExecDir)ln $(ExecDir)/$(TargetName) $(ExecDir)/create_dir
	ln $(ExecDir)/$(TargetName) $(ExecDir)/lookup_dir
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_dir

	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_file
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_file
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_file
	
	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_symlink
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_symlink
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_symlink_content
	ln $(ExecDir)/$(TargetName) $(ExecDir)/cat_symlink

	ln $(ExecDir)/$(TargetName) $(ExecDir)/create_hardlink
	
	ln $(ExecDir)/$(TargetName) $(ExecDir)/rm_hardlink

.PHONY: clean
clean:
	rm -rf $(ExecDir)
