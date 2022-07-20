#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    
    while(1)
    {
        //get input

        // parse input to a format in which execvp needs it to be

        // if no command continue and wait for new input 

        // else child fork
            // if child_pid = 0 execvp
            // else wait
    }

    return 0;
}