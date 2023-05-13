Compiler=gcc

Wkeys= -static -Werror -Wall -Wextra -pedantic
OptKeys= -O0
DbgKeys= -g3

SrcDir= Src
ExecDir= Exec

TargetName=mirror

.PHONY: build
build:
	$(Compiler) $(wildcard $(SrcDir)/*.c) $(Wkeys) $(OptKeys) $(DbgKeys) -o $(ExecDir)/$(TargetName)
	cd $(ExecDir)/
	ln app mirror

