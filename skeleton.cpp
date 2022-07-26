#include <iostream>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <setjmp.h>


using namespace std;

void history();
void sigint_handler(int);
int cd(char*);
static sigjmp_buf env;
static volatile sig_atomic_t jump_active = 0;

int main() {

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
    string prompt;
    prompt.append(user);
    prompt.append("@");
    prompt.append(hostname);
    prompt.append(":~$");
    
    while(1)
    {
        if (sigsetjmp(env, 1) == 42) {
            printf("\n");
            continue;
        }
        jump_active = 1;
        string input;
        string command[100];
        
        input = readline(prompt.c_str());
        add_history(input.c_str());
        append_history(1,"ctsh_history");
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
        child_pid = fork();
        // cout << "childpid " <<child_pid << endl;
        if (child_pid == 0) {
            /* Never returns if the call is successful */
            
            if (strcmp(argv[0], "cd") == 0) {
                cd(argv[1]);
            }
            else if(strcmp(argv[0],"history") == 0)
            {
                history();
            }
            else
            {
                execvp(argv[0], argv);
                printf("ctsh: %s: command not found\n", argv[0]);
            }
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

void sigint_handler(int signo) {
    if (!jump_active) {
        return;
    }
    siglongjmp(env, 42);
}

void history()
{
   HISTORY_STATE *myhist = history_get_history_state ();

    /* retrieve the history list */
    HIST_ENTRY **mylist = history_list ();

    for (int i = 0; i < myhist->length; i++) { /* output history list */
        printf (" %8s  %s\n", mylist[i]->line, mylist[i]->timestamp);
        free_history_entry (mylist[i]);     /* free allocated entries */
    }
    putchar ('\n');

    free (myhist);  /* free HIST_ENTRY list */
    free (mylist);  /* free HISTORY_STATE   */
}
int cd(char *path)
{
    return chdir(path);
}