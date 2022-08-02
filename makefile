OS := $(shell uname)
all:
ifeq ($(OS),Darwin)
	g++ -g -lreadline macos.cpp -o ctsh
else ifeq ($(OS),Linux)
    g++ -g -lreadline skeleton.cpp -o ctsh
else
	echo "OS not detected. Just hope this works."
	g++ -g -lreadline skeleton.cpp -o ctsh
endif