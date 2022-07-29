export LDFLAGS="-L /opt/homebrew/Cellar/readline/8.1.2/include/readline"
all: skeleton.cpp
	g++ -L/usr/include -g -lreadline -ledit skeleton.cpp -o ctsh

