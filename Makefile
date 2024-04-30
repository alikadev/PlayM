RM_F := rm -f
CC   := cc

SRC_DIR     := src
INCLUDE_DIR := include
PLAYM       := playm
PLAYM_MAJOR := 0
PLAYM_MINOR := 1
PLAYM_PATCH := 6
SRC         := $(shell find $(SRC_DIR) -name '*.c')
MIXERFLAGS  := $(shell pkg-config --cflags --libs SDL2_mixer)
CFLAGS      := -Werror -Wextra -Wall -std=c99 
CFLAGS      += -I$(INCLUDE_DIR) $(MIXERFLAGS) 
CFLAGS      += -DVERSION_MAJOR=$(PLAYM_MAJOR) 
CFLAGS      += -DVERSION_MINOR=$(PLAYM_MINOR)
CFLAGS      += -DVERSION_PATCH=$(PLAYM_PATCH)

.PHONY: $(PLAYM) clean

default: clean $(PLAYM)

$(PLAYM) : $(SRC)
	$(CC) $(CFLAGS) -o $(PLAYM) $(SRC) 

clean:
	$(RM_F) $(PLAYM)

