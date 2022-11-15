/*This is the implementation file for the functions used by the
server in the dining philosophers problem

Author: Stephanie Petrone
Date: November 12, 2022
Last modified: november 12, 2022
.
*/
#include "server.h"

#define NUM_CON 2 //number of connections serviced by this


int runServer(int port)
{
    unsigned short listen_port = port;
	struct sockaddr_in clientAddr; //holds client address of sock
	int addrLen; //holds lenght of client adress
	int count = 0 ; //bytes received wirth recv()
	int connectSock; //new socket with connection
	int listenSock; //socket for listen()
	char buf[MAXLEN]; //buffer for receiving 
	char inputBuf[MAXLEN]; //buffer to sending strings
	int msgLength = 0; //length of message to send
	struct sockaddr_in serverSockAddr;
	char IP_str[16]; //for holding string of IP addr (up tp 16 chars)
	int completeCon = 0; //counter of number of connections hich have been completed
	pid_t pid; //for child processes with




	int status; //holds status of child process when parent waiting
	pid_t pidS; //for slave process pid when testing if they are finished from parent
	int slaveProcCount = 0; //used for counting completed slave processes to close listening socket
	


	//create socket
	listenSock = socket(PF_INET, SOCK_STREAM, 0);

    //set address for the listening
	memset(&serverSockAddr, '0', sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons(listen_port);
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	printf( "Here and pid val is: %d\n", pid);
	//bind socket
	if( bind(listenSock, (struct sockaddr* )&serverSockAddr, sizeof(serverSockAddr)) == -1)
	{
        perror("Error with bind(): ");
		exit(1);
	}
	else
	{


		//ip to string for printing
		memset(IP_str, '\0', sizeof(IP_str));
		strcpy(IP_str, inet_ntoa(serverSockAddr.sin_addr));
		
		printf("\nlistenSock binded to port %u\n", listen_port);
		printf("\nIP address: %s\n\n", IP_str); 
	}


	//listen for connection
	if (listen(listenSock, 10) == -1)
	{
		perror("Error with listen(): ");
		return -1;
	}

	//print status
	printf("\nServer listening on port: %u...\n", listen_port);



	//Close the semaphore not using it in the parent process 
    /*if (sem_close(semaphore) < 0) {
        perror("sem_close(3) failed");
        // We ignore possible sem_unlink(3) errors here 
        sem_unlink(semName);
        exit(EXIT_FAILURE);
    } */

    
	//iteratively accept connection
	while(completeCon < NUM_CON)
	{

		//accept new connection
		if(( connectSock = accept(listenSock,(struct sockaddr *) &clientAddr, &addrLen)) == -1)
		{
			perror("Error with accept(): ");
			return -1;
		}
		else {
		
		
			printf("\nSuccesfully connected to client socketfd: %d,\n\n", connectSock);

			printf("...............Spawning slave process................\n");







			if ((pid = fork()) < 0) {
				perror("Error spawning slave process");
				exit(1);
			}

			if (pid == 0) { //child process



			//receive first message then loop for more receiving
			memset(buf, '\0', sizeof(buf)); //clear buffer
			if((count = recv(connectSock, buf, sizeof(buf), 0)) < 0)
			{
				perror("error with first recv(): ");
				return -1;
			}

			//Run as a server, being the fork serving two philosophers
			while (count > 0) {

				memset(buf, '\0', sizeof(buf)); //clear buffer
				strcpy(inputBuf, "given");
	    		msgLength = strlen(inputBuf);

				if ((send(connectSock, inputBuf, msgLength, 0)) != msgLength)
				{
					perror("Error with send(): ");
				}

				//receive next message
				memset(buf, '\0', sizeof(buf)); //reset buffer
				count = recv(connectSock, buf, sizeof(buf), 0); 

			}
					
			/*//loop for recv() more until nothing more is received
			while (count > 0)
			{
				
				
				//print in server the message received
				printf("\nMessage, size %d received into buf: \n %s\n\n",count, buf);

				//echo messsage
				if ((send(connectSock, buf, count, 0)) != count)
				{
					perror("Error with echo send(): ");
				}
				else
					printf("Sent message of size %d to client\n", count);

				//receive next message
				memset(buf, '\0', sizeof(buf)); //reset buffer
				count = recv(connectSock, buf, sizeof(buf), 0); 
				
			} */

			//close client socket 
			close(connectSock);



			exit(0); //exit child process
				
			}
			else { //in parent
				completeCon++;
				close(connectSock); //doesn't use socket, so close it

				
			}
		
		}	
	}

	/* Wait for children to exit. */
	slaveProcCount = completeCon;
	while (slaveProcCount > 0) {
 	pidS = wait(&status);
  	printf("Slave process with PID %ld exited with status 0x%x.\n", (long)pidS, status);
  	--slaveProcCount; 
	}
	
	//all slave processes must have completed (from while loop above)
	//close the listening socket
	close(listenSock);


}