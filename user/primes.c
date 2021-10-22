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
main(int argc,char *argv[])
{

    int i,num[50],len=0;
    for(i=2;i<=35;i++){
        num[len++]=i;
    }
    
    while( len>0 )
    {
        pid_t pid;
        int i,fd[2];
        pipe(fd);

        if( ( pid = fork() ) <0)
        {
            fprintf(STDEER_FILENO,"fork error\n");
            exit(1);
        }

        else if(pid >0 )//parent process
        {   
            
            close(fd[READEND]);
            for(i=0;i<len;i++){
                write(fd[WRITEEND],&num[i],sizeof(num[i]));
            }

            close(fd[WRITEEND]);
            wait((int *)0);
            exit(0);
        }

        else if( pid == 0 )//child process
        {     
            
            close(fd[WRITEEND]);
            int prime=0,tmp=0,idx=-1;

            // int tmp;

            while( read( fd[READEND],&tmp,sizeof(tmp) ) !=0 )
            {
                if( idx == -1 ){
                    prime=tmp;
                    idx++;
                }

                if(tmp % prime !=0 )
                {
                    num[idx++]=tmp;
                }
            
            }

            close(fd[READEND]);
            printf("prime %d\n",prime);

            len = idx;
        }
        
    }
    exit(0);
}