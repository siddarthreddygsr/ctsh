#include <iostream> 
#include <sys/utsname.h>

using namespace std;

int main() {

    char *user=getenv("USER");
    char* hostname;
    struct utsname uname_data;
	uname(&uname_data);
	hostname = uname_data.nodename;
    if(user==NULL) return EXIT_FAILURE;
    // cout << hostname;
    while(1)
    {
        string input;
        string command[100];
        cout << user<<"@"<<hostname << ">";
        getline(cin,input);
        int counter = 0;
        for (short i = 0; i<input.length(); i++){
            if (input[i] == ' ')
                counter++;
            else
                command[counter] += input[i];
        }


        // parse input to a format in which execvp needs it to be

        // if no command continue and wait for new input 

        // else child fork
            // if child_pid = 0 execvp
            // else wait
    }

    return 0;
}