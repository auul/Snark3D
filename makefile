BIN=snark
LINKS=-lm -lSDL2 -lSDL2_image
SRC_DIRS=. src
INC_DIRS=. ./include

CC=gcc
OPT=-O0
DEP_FLAGS=-MP -MD
INC_FLAGS=$(foreach D,$(INC_DIRS),-I$(D))
C_FLAGS=-Wall -Wextra -g $(INC_FLAGS) $(OPT) $(DEP_FLAGS)

SRC_FILES=$(foreach D,$(SRC_DIRS),$(wildcard $(D)/*.c))
OBJ_FILES=$(patsubst %.c,%.o,$(SRC_FILES))
DEP_FILES=$(patsubst %.c,%.d,$(SRC_FILES))

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $(C_FLAGS) $^ $(LINKS) -o $@

%.o:%.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ_FILES) $(DEP_FILES)

.PHONY:
	all clean
