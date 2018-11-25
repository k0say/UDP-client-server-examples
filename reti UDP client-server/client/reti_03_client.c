#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> // for DNS
#include <limits.h>

#define PORT 48000
#define ECHOMAX 255

void errorhandler(char *error_message) {
	printf("%s", error_message);
}

int main() {

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	unsigned int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)	{
		errorhandler("socket creation failed.\n");
		close(sock);
		return -1;
	}

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(PORT);	
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	
	// DNS PART
	/*
	const char * name = "localhost";
	struct hostent *host;
	host = gethostbyname(name);
	
	if (host == NULL) {
		fprintf(stderr, "gethostbyname() failed.\n");
		exit(EXIT_FAILURE);
	} else {
		struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];
		printf("Risultato di gethostbyname(%s): %s\n", name,inet_ntoa(*ina));
	}
	
	const char* ip = "127.0.0.1";
	struct in_addr addr;
	addr.s_addr = inet_addr(ip);
	host = gethostbyaddr((char *) &addr, 4, AF_INET);
	char* canonical_name = host->h_name;
	fprintf(stdout, "Risultato di gethostbyaddr(%s): %s\n", ip, canonical_name);
	
	//-------------------------------------
	
    char hostname[HOST_NAME_MAX + 1];

    hostname[HOST_NAME_MAX] = 0;
    if (gethostname(hostname, HOST_NAME_MAX) == 0)
        puts(hostname);
    else
        perror("gethostname");

	struct in_addr addr;
	struct hostent *hstnm;
    
        fprintf(stderr, "usage: %s hostname\n");// CHE GLI PASSO??
     
    hstnm = gethostbyname((char*)&addr, 4, AF_INET);
    if (!hstnm)
        return 1;
    printf ("Name: %s\n", hstnm->h_name);
*/
    //----------------------------------

		printf("Inserisci la stringa echo da inviare al server\n");
		scanf("%s", echoString);
		
		if((echoStringLen = strlen(echoString)) > ECHOMAX)	{
			errorhandler("echo word too long\n");
			close(sock);
			return -1;
		}

		if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) != echoStringLen)	{
			errorhandler("sendto() sent different number of bytes than expected\n");
			close(sock);
			return -1;
		}

		// RITORNO DELLA STRINGA ECHO
		fromSize = sizeof(fromAddr);
		respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*) &fromAddr, &fromSize);

		if(echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)	{
			fprintf(stderr, "Error: received a packet from unknown source.\n");
			exit(EXIT_FAILURE);
		}
		char b1[sizeof(echoBuffer)] ="";
		sprintf(b1, "Received: %s\n", echoBuffer);
		//fflush(stdout);

		//printf("Receive: %s\n", echoBuffer);
		close(sock);
		getchar();
		return EXIT_SUCCESS;
}