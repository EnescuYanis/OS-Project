#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main()
{

    int fd[2];
    int PID = fork();
    char buffer[100];
   


    if(pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }


    if(PID < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    if(PID > 0)
    {
        close(fd[0]);
        char message[] = "Hello from the parent process!";
        write(fd[1], message, sizeof(message));
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Child process received message: %s\n", buffer);
        close(fd[0]);
    }

    return 0;
}

