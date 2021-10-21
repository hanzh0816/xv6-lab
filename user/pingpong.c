#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//define standard I/O code
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDEER_FILENO 2

//define read/write end code
#define READEND 0
#define WRITEEND 1

typedef int pid_t;

int
main(int argc, char *argv[])
{
    int pipe1_fd[2];    //parent --> child
    int pipe2_fd[2];    //child --> parent
    char buf[10];       //buffer array

    pid_t pid;
        
    pipe(pipe1_fd);
    pipe(pipe2_fd);

    if((pid=fork()) < 0)
    {
        fprintf(STDEER_FILENO,"fork error\n");
        exit(-1);
    }   

    if(pid == 0)    // child process
    {
        close(pipe1_fd[WRITEEND]);
        close(pipe2_fd[READEND]);
        
        read(pipe1_fd[READEND],buf,4);
        close(pipe1_fd[READEND]);
        printf("%d: received %s\n",getpid(),buf);

        write(pipe2_fd[WRITEEND],"pong",4);
        close(pipe2_fd[WRITEEND]);
        sleep(10);
    }

    else              // parent process
    {
        close(pipe1_fd[READEND]);
        close(pipe2_fd[WRITEEND]);


        write(pipe1_fd[WRITEEND],"ping",4);
        close(pipe1_fd[WRITEEND]);
        
        read(pipe2_fd[READEND],buf,4);
        close(pipe2_fd[READEND]);
        printf("%d: received %s\n",getpid(),buf);

        sleep(10);
    }
    exit(0);
}