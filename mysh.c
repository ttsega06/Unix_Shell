#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 256
#define MAX_ARGUMENTS 10
//in a shell the text for the path is usually a different color
// so i will  use green
#define GREEN   "\033[32m" 
#define RESET   "\033[0m"


// take in the command and the arguments
// the arguments should be a NULL terminated array
void execCMD(const char *command, char **args)
{
    // Forking a child
    pid_t pid = fork();
    // if fork failed show error
    if (pid == -1)
    {
        fprintf(stderr, "Failed to create process");
        return;
    }
    // if child process created run the command
    else if (pid == 0)
    {
        if (execvp(command, args) < 0)
        {
            fprintf(stderr, "Failed to execute command\n");
            //  In a child process so exit the child process
            exit(1);
        }
        else
        {
            // In a child process so exit the child process
            exit(0);
        }
    }
    else
    {
        // waiting for fork to finish
        wait(NULL);
    }
}

int main()
{
    // save the current shell path to be printed - assumebtion it wont be more than 256 chars
    char path[MAX_CMD_LENGTH];
    getcwd(path, sizeof(path));
    // add the ending to the path
    strcat(path, "$mysh>");
    // command shouldnt be more than 50 chars in this case
    char userInput[MAX_CMD_LENGTH];
    // repeat forever - there is a check for the exit command below
    while (1)
    {
        // show the current path
        printf(GREEN "%s" RESET, path);
        char *res = fgets(userInput, MAX_CMD_LENGTH, stdin);
        // if fgets fails then print error
        if (res == NULL)
        {
            fprintf(stderr, "Failed to read command");
        }
        int numChars = strlen(userInput);
        userInput[numChars - 1] = '\0';
        //  an array needed to hold the arguments - we assume max arguments is 10
        //  created the array to store the arguments
        char **args = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);

        // printf("numArgs: %d  argStr: %s\n", numArgs, argsStr);
        //  use strtokener to go through the arg str and copy each argument into this array
        char *curArg = strtok(userInput, " ");
        int curArgIdx = 0;
        // tokener will give null when it's finished
        while (curArg != NULL)
        {
            // need to allocate space for this char array before assigning it
            args[curArgIdx] = (char *)malloc(sizeof(char) * strlen(curArg));
            // copy the current argument into the array
            strcpy(args[curArgIdx], curArg);
            // move onto the next idex for next time
            curArgIdx++;
            // move onto the next space and get the next argument
            curArg = strtok(NULL, " ");
        }
        // the execvp wants a null terminated 2d array so put null in the last index
        args[curArgIdx] = NULL;
        // test print to make sure the arguments are in the array
        /* for (int i = 0; i <= curArgIdx; i++)
        {
            if (args[i] == NULL)
            {
                printf("NULL\n");
            }
            else
            {
                printf("|%s|\n", args[i]);
            }
        }
        */

        // at this point the command is stored in arg[0]

        if (strcmp(args[0], "pwd") == 0)
        {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            
            if (curArgIdx == 1)
            {
                printf("cd: you must provide a directory to change to\n");
            }
            else
            {
                chdir(args[1]);
                // since I changed directories I should update that path that prints
                getcwd(path, sizeof(path));
                strcat(path, "$mysh>");
            }
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            exit(0);
        }
        else
        {
            execCMD(args[0], args);
        }
        // printf("CMD rec: %s   %d\n", cmd, numChars);
    }
}