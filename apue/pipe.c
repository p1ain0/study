#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pipe_fd[2];
    int state = pipe(pipe_fd);
    
    pid_t pid = fork();
    if(pid == 0)
    {
        write(pipe_fd[1],"Hello world", 11);
        exit(0);
    }
    else if(pid > 0)
    {
        char buf[256];
        waitpid(pid, NULL, 0);
        read(pipe_fd[0], buf, 256);
        printf("%s", buf);
    }
    
    return 0;
}