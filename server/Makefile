.PHONY: clear, list, all, building

C_OBJECTS = $(wildcard src/*.c)
O_OBJECTS = $(C_OBJECTS:src/%.c=target/%.o)

BIN_PATH = "bin"
TARGET_PATH = "target"
APP_NAME = "server"

TARGET = building

all: $(TARGET)

building: $(O_OBJECTS)
	gcc -g $(O_OBJECTS) -o $(BIN_PATH)/$(APP_NAME)

target/%.o: src/%.c
	gcc -g -c $< -Ihdr -o $@

#$< == таргет
#$@ == пререквезит первый

clear:
	rm $(BIN_PATH)/$(APP_NAME)
	rm $(TARGET_PATH)/*.o

list:
	echo $(C_OBJECTS)
	echo $(H_OBJECTS)
	echo $(O_OBJECTS)

