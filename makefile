OS := $(shell uname)
all:
ifeq ($(OS),Darwin)
	@echo "MacOS not supported yet"
else ifeq ($(OS),Linux)
    g++ -g -lreadline linux.cpp -o ctsh
endif