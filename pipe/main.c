#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "debug.h"

#define MAXLINE 256

int main(void)
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
	int status;

    if (pipe(fd) < 0)
    {
        debug_error("pipe error");
        exit(-1);
    }
    if ((pid = fork()) < 0) 
    {
        debug_error("fork error");
        exit(-1);
    } 
    else if (pid > 0) 
    { /* parent */
        close(fd[0]);
        write(fd[1], "hello pipe\n", 12);

		close(1);
		dup2( fd[1], 1 );
		execlp( "ls", "ls", "-l", NULL );
		
		wait(&status);
    }
    else { /* child */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);

		close(0);
		dup2( fd[0], 0 );
		execlp( "wc", "wc", "-l", NULL );
    }

    exit(0);
}



