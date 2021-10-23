#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/param.h"

//define standard I/O code
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDEER_FILENO 2

#define BUFFSIZE 1024

void xargs(int argc,char *argv[])
{
    char *cmd=argv[1];
    char *param[MAXARG];    // sub command parameters
    char buf[BUFFSIZE];

    int n,i,idx=0;

    for(i=1;i<argc;i++)
        param[idx++]=argv[i];
    
    while( (n=read(STDIN_FILENO,buf,BUFFSIZE)) >0 )
    {
        
        if(fork()==0)   //child process
        {
            char *arg=(char *)malloc(sizeof(buf));  //store args in a heap array
            int arg_idx=0;
            for(i=0;i<n;i++)
            {
                if( buf[i]==' ' || buf[i]=='\n' )   //parameter end
                {
                    arg[arg_idx]=0;     //end with 0
                    param[idx++]=arg;
                    arg_idx=0;
                    arg=(char *)malloc(sizeof(buf));
                }
                else
                {
                    arg[arg_idx++]=buf[i];
                }
            }
            arg[arg_idx]=0;     
            param[idx]=0;   //argv[argc]=0
            exec(cmd,param);
        }
        else
            wait((int*)0);
    }
    return;
}

int *
main(int argc,char *argv[])
{
    xargs(argc,argv);

    exit(0);
}