#include "client.h"

int runClient(int portL, int portR)
{
	
	int sockidL; //socket to connect to server for left hand
	int sockidR; //socket to connect to server for right hand
    struct sockaddr_in servAddrL; //server address for left hand
	struct sockaddr_in servAddrR; //server address for right hand
	char inputBuf[MAXLEN]; //buffer to sending strings
	char recBuf[MAXLEN]; //buffer for receiving echo
	int msgLength = 0; //length of message to send
	int count = 0; //number of bytes with send() and recv()
	int philEatCount = 0; //calculating how much philsosopher has eaten
	int leftFork = 0; //determines if the philospher has the left fork
	int rightFork = 0; //determines if the philosopher has the right fork


 	//set server address information for left hand
	memset(&servAddrL, '\0', sizeof(servAddrL));//initialize struct
	servAddrL.sin_family = AF_INET;
	servAddrL.sin_port = htons(portL);
	servAddrL.sin_addr.s_addr = inet_addr("127.0.0.1");

	//set server address information for right hand
	memset(&servAddrR, '\0', sizeof(servAddrR));//initialize struct
	servAddrR.sin_family = AF_INET;
	servAddrR.sin_port = htons(portR);
	servAddrR.sin_addr.s_addr = inet_addr("127.0.0.1");

	//create socket for left hand
	if((sockidL = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error with socket(): \n");
		return -1;
	}

	//create socket for right hand
	if((sockidR = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error with socket(): \n");
		return -1;
	}

	//connect to server for left hand
	if(connect(sockidL, (struct sockaddr *)&servAddrL, sizeof(servAddrL)) != 0)
	{
		perror("Error with connect(): \n");
		return -1;
	}

	//connect to server for right hand
	if(connect(sockidR, (struct sockaddr *)&servAddrR, sizeof(servAddrR)) != 0)
	{
		perror("Error with connect(): \n");
		return -1;
	}



	//dining philosophers problem, this is a philosopher's hand

	//loop until eaten enought or so much time has passed (deadlock?)
	while (philEatCount < PHIL_EAT_REQ) {

		//wait for forks
		//left fork first, request from the fork server
		while(!leftFork) {
			memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
	    	strcpy(inputBuf, "request");
	    	msgLength = strlen(inputBuf); 

		if((count = send(sockidL, inputBuf, msgLength, 0)) != msgLength)
		{
			perror("Error with send(): \n");
			return -1;
		}
		else
			printf("requesting left fork ***\n");

		//receive echo
		memset(recBuf, '\0', sizeof(recBuf)); //reset buffer
		count = recv(sockidL, recBuf, sizeof(recBuf), 0);

		if (strcmp(recBuf, "given") == 0 ) {
			leftFork = 1;
		} //wil exit loop, otherwise will keep looping until it is given

		}
		//then picks up the right fork

		while(!rightFork) {
			memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
	    	strcpy(inputBuf, "request");
	    	msgLength = strlen(inputBuf); 

			if((count = send(sockidR, inputBuf, msgLength, 0)) != msgLength)
			{
				perror("Error with send(): \n");
				return -1;
			}
			else
				printf("requesting right fork ***\n");

			//receive echo
			memset(recBuf, '\0', sizeof(recBuf)); //reset buffer
			count = recv(sockidR, recBuf, sizeof(recBuf), 0);

			if (strcmp(recBuf, "given") == 0 ) {
				rightFork = 1;
			} //will exit loop, otherwise will keep looping until it is given

		}

		printf("\n\nPHILOSOPHER HAS BOTH FORKS\n\n");
		philEatCount = PHIL_EAT_REQ;

		//eat when they have both forks, each time they eat they have the forks for 2 second and 
		//increment eating amount by 2

		//think for 3 seconds
	}
	


	/*//send message/echo with server LEFT
        //send message
	    memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
	    strcpy(inputBuf, "...in a LEFT HAND client process sending message to server...\n");
	    msgLength = strlen(inputBuf); 

		if((count = send(sockidL, inputBuf, msgLength, 0)) != msgLength)
		{
			perror("Error with send(): \n");
			return -1;
		}
		else
			printf("Sent message of length %d to server ***FROM LEFT ***\n", count);

		//receive echo
		memset(recBuf, '\0', sizeof(recBuf)); //reset buffer
		count = recv(sockidL, recBuf, sizeof(recBuf), 0);

		//print echo from server
		printf("\n\nECHO from server: \n");
		printf("length in bytes: %d\n", count);
		printf("%s\n", recBuf);		

	

		//send message/echo with server RIGHT ***********************
        //send message
	    memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
	    strcpy(inputBuf, "...in a client RIGHT HAND process sending message to server...\n");
	    msgLength = strlen(inputBuf); 

		if((count = send(sockidR, inputBuf, msgLength, 0)) != msgLength)
		{
			perror("Error with send(): \n");
			return -1;
		}
		else
			printf("Sent message of length %d to server ***FROM RIGHT***\n", count);

		//receive echo
		memset(recBuf, '\0', sizeof(recBuf)); //reset buffer
		count = recv(sockidR, recBuf, sizeof(recBuf), 0);

		//print echo from server
		printf("\n\nECHO from server: \n");
		printf("length in bytes: %d\n", count);
		printf("%s\n", recBuf);	
*/

	//close left
 	close(sockidL); 

	//close right
	close(sockidR);

    return 0;

}