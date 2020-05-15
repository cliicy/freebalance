### @par Copyright:
### Copyright (c) by ScaleFlux, Inc.
###
### ALL RIGHTS RESERVED. These coded instructions and program statements are
### copyrighted works and confidential proprietary information of ScaleFlux, Inc.
### They may not be modified, copied, reproduced, distributed, or disclosed to
### third parties in any manner, medium, or form, in whole or in part.
###
### @par Description:
### The Description of this file.

TOP_DIR         := ../../..
TOP_DIR_ABS     := $(abspath $(TOP_DIR))
# The following are defined by the top level makefile
EXTRA_CFLAGS    := -g3 -lec -lccsutil -lrt -lpthread -l:libblkid.so.1
EXTRA_LDFLAGS   :=
SUBDIRS         := 
SHARELIB        := 
SHARE_INCLUDE   := 
SRCS            := src/ini.c src/sfxini.c src/fsb_main.c src/vsystem.c
EXECS           := sfx-fsb
TESTS           :=
OBJ_DIR		:=./

$(OBJ_DIR)/sfx-fsb:
	$(CC) -g $(CFLAGS) $(LDFLAGS) $(LIB_DIR) ${SRCS} -o $@ $^

.PHONY: clean_build
clean_build:
	rm -rf $(OBJ_DIR)/sfx-fsb