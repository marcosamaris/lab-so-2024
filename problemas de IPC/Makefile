OUTPUT=filosofos

CC=gcc
CC_OPT=-std=c11 -w

CC_PTH=-pthread

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OUTPUT).c
	$(CC) -o $(OUTPUT) $(CC_OPT) $(CC_PTH) $(OUTPUT).c

.PHONY: clean
clean:
	rm $(OUTPUT)

.PHONY: run
run:
	./$(OUTPUT)

