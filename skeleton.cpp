#include <iostream>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>


using namespace std;

int main() {

    char *user=getenv("USER");
    char* hostname;
    struct utsname uname_data;
    pid_t child_pid;
    int stat_loc;



	uname(&uname_data);
	hostname = uname_data.nodename;
    if(user==NULL) return EXIT_FAILURE;
    // cout << hostname;
    while(1)
    {
        string input;
        string command[100];
        string prompt;
        prompt.append(user);
        prompt.append("@");
        prompt.append(hostname);
        prompt.append(":~$");
        input = readline(prompt.c_str());
        // getline(cin,input);
        int counter = 0;
        int flag = 0;
        for (short i = 0; i<input.length(); i++){
            if(input[i]==' '&& flag == 0)
                continue;
            else
            {
                flag = 1;
                if (input[i] == ' ')
                    counter++;
                else
                    command[counter] += input[i];
            }
        }

        // for(int i = 0;i<counter+1;i++)
        // {
        //     cout << command[i]<<endl;
        // }
        if (command->empty()) { 
            cout << "I am empty"<<endl;     /* Handle empty commands */
            continue;
        }

        child_pid = fork();
        // cout << "childpid " <<child_pid << endl;
        if (child_pid == 0) {
            /* Never returns if the call is successful */
            char*argv[counter+2];
            for (int i = 0; i < counter+1; i++)
            {
               argv[i] = const_cast<char*>(command[i].c_str());
            }
            argv[counter+1] = NULL;
            execvp(argv[0], argv);
            printf("This won't be printed if execvp is successul\n");
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }


        // if no command continue and wait for new input 

        // else child fork
            // if child_pid = 0 execvp
            // else wait
    }

    return 0;
}