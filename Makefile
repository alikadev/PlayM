RM_F := rm -f
CC   := cc

SRC_DIR     := src
INCLUDE_DIR := include
PLAYM       := playm
SRC         := $(shell find $(SRC_DIR) -name '*.c')
MIXERFLAGS  := $(shell pkg-config --cflags --libs SDL2_mixer)
CFLAGS      := -Werror -Wextra -Wall -std=c99 -I$(INCLUDE_DIR) $(MIXERFLAGS)

.PHONY: $(PLAYM) clean

default: clean $(PLAYM)

$(PLAYM) : $(SRC)
	$(CC) $(CFLAGS) -o $(PLAYM) $(SRC) 

clean:
	$(RM_F) $(PLAYM)

