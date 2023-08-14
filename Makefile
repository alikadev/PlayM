D_BUILD		?=	build
D_SRC		:=	src
D_INCLUDE 	:=	include
OUT			?=	playm

CC 			:=	gcc
LD 			:=	gcc

SRC_C		:=	$(shell find $(D_SRC) -name '*.c')
OBJ_C		:=	$(subst $(D_SRC), $(D_BUILD), $(SRC_C:%.c=%.c.o))

CFLAGS	 	:=	-Werror -Wall -Wextra -c -std=c11 -I$(D_INCLUDE) $(shell pkg-config --cflags sdl2 SDL2_mixer)
LDFLAGS		:=	$(shell pkg-config --libs sdl2 SDL2_mixer) -lpthread

.PHONY: all clean

all: always clean $(OUT)

$(OUT) : $(OBJ_C)
	@printf "\e[1;32m  Building\e[0m $(notdir $@)\n"
	@$(LD) $(OBJ_C) -o $@ $(LDFLAGS)

$(D_BUILD)/%.c.o: $(D_SRC)/%.c
	@mkdir -p $(dir $@)
	@printf "\e[1;32m  Compiling\e[0m $(notdir $<)\n"
	@$(CC) -o $@ $(CFLAGS) $<

clean:
	@printf "\e[1;32m  Cleaning\e[0m\n"
	@rm -f $(D_BUILD)/*

always:
	@mkdir -p $(D_BUILD)