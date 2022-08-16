#include <iostream>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/wait.h>
#include <editline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <setjmp.h>
#include <fstream>


using namespace std;

void history();
void sigint_handler(int);
int cd(char*);
static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;

int main() {

    char cwd[256];
    getcwd(cwd, 256);
    char tmp[14] = "/ctsh_history";
    strcat(cwd, tmp);

    // cout << "Current working directory: " << tm << endl;
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);

    setenv("SHELL","cybertrauma's shell",1); // does overwrite
    char *user=getenv("USER");
    char* hostname;
    struct utsname uname_data;
    pid_t child_pid;
    int stat_loc;

	uname(&uname_data);
	hostname = uname_data.nodename;
    if(user==NULL) return EXIT_FAILURE;
    using_history(); 
    read_history("ctsh_history");
    // cout << hostname;
    string prompt= "";
    prompt.append(user);
    prompt.append("@");
    prompt.append(hostname);
    prompt.append(":~$");
    
    while(1)
    {
        string input;
        string command[100];
        if (sigsetjmp(env, 1) == 42) {
            printf("\n");
            continue;
        }
        jump_active = 1;
        
        
        input = readline(prompt.c_str());
        if (input.c_str() == NULL) {  /* Exit on Ctrl-D */
            printf("\n");
            exit(0);
        }
        add_history(input.c_str());
        ofstream histfile("ctsh_history", std::ios_base::app);
        histfile << input.c_str()<<endl;
        histfile.close();
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

        if (command->empty()) { 
            // cout << "I am empty"<<endl;     /* Handle empty commands */
            continue;
        }
        char*argv[counter+2];
        for (int i = 0; i < counter+1; i++)
        {
            argv[i] = const_cast<char*>(command[i].c_str());
        }
        argv[counter+1] = NULL;
        if (strcmp(argv[0], "exit") == 0) {
                return 0;
        }
        if (strcmp(argv[0], "cd") == 0)
        {
            if (cd(argv[1]) < 0) 
            {
                perror(argv[1]);
            }
            /* Skip the fork */
            continue;
        }
        // else if(strcmp(argv[0],"history") == 0)
        // {
        //     history();
        //     continue;
        // }
        child_pid = fork();
        // cout << "childpid " <<child_pid << endl;
        if (child_pid == 0) {
            struct sigaction s_child;
            s_child.sa_handler = sigint_handler;
            sigemptyset(&s_child.sa_mask);
            s_child.sa_flags = SA_RESTART;
            sigaction(SIGINT, &s_child, NULL);
            /* Never returns if the call is successful */
            
            // if (strcmp(argv[0], "cd") == 0) {
            //     cd(argv[1]);
            // }
            if(strcmp(argv[0],"history") == 0)
            {
                char *argv[] = {"cat", cwd, 0};
                execvp(argv[0],argv);
                continue;
            }
            execvp(argv[0], argv);
            printf("ctsh: %s: command not found\n", argv[0]);
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

    }

    return 0;
}

void sigint_handler(int signo) {
    if (!jump_active) {
        return;
    }
    siglongjmp(env, 42);
}

void history()
{
    // cout << "this command prints history in linux but dev was lazy to develop it for macOS"<<endl;
    char *argv[] = {"cat", "ctsh_history", 0};
    execvp(argv[0],argv);
}
int cd(char *path)
{
    return chdir(path);
}


