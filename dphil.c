#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "server.h"
#include "client.h"

#define NUMPHIL 5


int main (void) {

//int childCount = 2 * NUMPHIL; //used for decrementing number of children as they exit
int childCount = 0;
int status; //holds status of child process when parent waiting
pid_t pidF; //hold pid of finished child processes when parent waiting


pid_t pid;

//create child processes for fork servers
for (int i = 0; i < NUMPHIL; i++) {
     
    if ((pid = fork()) < 0) {
        perror("error creating forkserver child ");
        exit(1);
    }

    if (pid == 0) {
        printf("In fork server child %d %d, parent is: %d\n", i, getpid(), getppid());

        int portNumServ = 9000 + i;
        runServer(portNumServ);


        exit(0);
    }
    else if (pid > 0) {
        printf("in parent, i = %d\n", i);
        childCount++;
    }
}

//create child processes for philosopher clients (one for each hand)
if(pid > 0) { //from parent process
    for (int i = 0; i < NUMPHIL; i++) { //one for each philosopher,
        

            if ((pid = fork()) < 0) {
                perror("error creating philClient child ");
                exit(1);
            }

            if (pid == 0) {
                printf("In philosopher client child %d, %d, parent is: %d\n", i, getpid(), getppid());

                //two ports needed, one for each hand 
                int portnumR = 9000 + i;
                int portnumL = 9000 + ((i+1)%NUMPHIL);
                printf("Client %d connecting to fork server ports: %d (left), and %d (right)\n", i, portnumL, portnumR);
                
                    runClient(portnumL, portnumR);
                
                exit(0); //exit child when runClient() is finished
            }
            else if (pid > 0) {
                printf("in parent making philClient, i = %d,\n", i);
                childCount++;
            }
        

    }
}


/* Wait for children to exit. */
while (childCount > 0) {
  pidF = wait(&status);
  printf("Child with PID %ld exited with status 0x%x.\n", (long)pidF, status);
  --childCount; 
}

return 0;
}
