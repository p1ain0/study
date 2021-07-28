#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <strings.h>
#include <fcntl.h>

#define MAXLINE 256

#ifdef __APPLE__
#define DEF_PAGER "/usr/bin/more"
#elif defined __LINUX__
#define DEF_PAGER "/bin/more"
#endif

void err_quit(const char *err_msg, ...)
{
    va_list va;
    va_start(va, err_msg);
    vfprintf(stderr, err_msg, va);
    va_end(va);
    exit(-1);
}

int main(int argc, char* argv[])
{
    int n;
    int pipe_fd[2];
    pid_t pid;
    char *pager, *argv0;
    char line[MAXLINE];
    FILE *fp;

    if(argc != 2)
        err_quit("Usage : pager <pathname>\n");
    if((fp = fopen(argv[1], "r")) == NULL)
        err_quit("Can't open %s\n", argv[1]);
    if(pipe(pipe_fd) < 0)
        err_quit("pipe error\n");

    pid = fork();
    if(pid < 0)
        err_quit("fork error\n");
    else if(pid == 0){
        close(pipe_fd[1]);
        if(pipe_fd[0] != STDIN_FILENO){
            if(dup2(pipe_fd[0], STDIN_FILENO) != STDIN_FILENO)
                err_quit("dup2 error\n");
            //printf("%d\n", pipe_fd[0]);
            close(pipe_fd[0]);
        }
        pager = getenv("PAGER");
        if(pager == NULL)
            pager = DEF_PAGER;
        argv0 = strrchr(pager, '/');
        if(argv0 != NULL)
            argv0 ++;
        else
            argv0 = pager;
        fcntl(STDIN_FILENO, F_DUPFD);
        
        if(execl(pager, argv0, (char* )0) < 0)        
            err_quit("execl error for %s", pager);

    }
    else{
        close(pipe_fd[0]);
        while(fgets(line, MAXLINE, fp) != NULL){
            n = strlen(line);
            if(write(pipe_fd[1], line, n) != n)
                err_quit("write error to pipe\n");
        }
        if(ferror(fp) != 0)
            err_quit("fgets error\n");

        close(pipe_fd[1]);

        if(waitpid(pid, NULL, 0) < 0)
            err_quit("waitpid error\n");
        exit(0);
    }
    return 0;
}