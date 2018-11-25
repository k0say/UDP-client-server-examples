#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
//#include <ctype.h>
#include <stdlib.h> // for atoi()

#include <netdb.h> // for DNS

#define PORT 48000
#define ECHOMAX 255


void errorhandler(char *errorMessage) {
	printf ("%s", errorMessage);
}

int check_vowel(char ch)
{
    if (ch == 'a' || ch == 'A' || ch == 'e' || ch == 'E' || ch == 'i' || ch == 'I' || ch =='o' || ch=='O' || ch == 'u' || ch == 'U')
      return 1;
    else
      return 0;
}

int main() {

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	int recvMsgSize;
	
	//CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		errorhandler("socket creation failed.\n");
	}
	else{
		printf("connessione riuscita\n");
	}

	//COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr)); // ensures that extra bytes contain 0
	memset(&echoBuffer, 0, sizeof(echoBuffer));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	echoServAddr.sin_port = htons(PORT);
	//DNS-----------------------------
	char *host_name;
	struct hostent *host;
	//--------------------------------

	//BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr))) < 0) 
		errorhandler("bind() failed.\n");

//DNS
//------------------------
	const char * name = "localhost";
	//struct hostent *host;
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
//---------------


	//RICEZIONE STRINGA
	while(1)	{

//DNS ----------------------
		// Validate the parameters    
		//argv[0] dovrà essere acquisito da tastiera
if (argc!= 2) {        
	printf("usage: %s ipv4 address\n", argv[0]);        
	printf("  to return the host\n");        
	printf("       %s 127.0.0.1\n", argv[0]);        
	return EXIT_FAILURE;    
}    

host_name= argv[1];
if (isalpha(host_name[0])) {        
/* host address is a name  e.g., “www.google.com” */        
	printf("Calling gethostbynamewith %s\n", host_name);        
	remoteHost= gethostbyname(host_name);    
}    
else {                                     
/* host address is IP address  e.g., “127.0.0.1” */ 
	printf("Calling gethostbyaddrwith %s\n", host_name);        
	addr.s_addr= inet_addr(host_name);        
	remoteHost= gethostbyaddr((char *) &addr, 4, AF_INET);    
}

//-------------------------


		cliAddrLen = sizeof(echoClntAddr);
		recvMsgSize = recvfrom(sock, echoBuffer, sizeof(echoBuffer), 0, (struct sockaddr*) &echoClntAddr, &cliAddrLen);
		printf("Handling client %s port %d\n", inet_ntoa(echoClntAddr.sin_addr), ntohs(echoClntAddr.sin_port));
		printf("Initial string: %.*s\n", recvMsgSize, echoBuffer);

		char s[sizeof(echoBuffer)], t[sizeof(echoBuffer)];
		int c, d = 0;
			for(c = 0; echoBuffer[c] != '\0'; c++) {
		    	if(check_vowel(echoBuffer[c]) == 0) {       // If not a vowel
		      		t[d] = echoBuffer[c];
		      		d++;
		    	}
		  	}
		 
		 t[d] = '\0';
 
		strcpy(echoBuffer, t);  // We are changing initial string. This is optional.
		 
		printf("String after deleting vowels: %.*s\n", recvMsgSize, echoBuffer);

		//printf("Receive: %s\n", echoBuffer);
		//echoBuffer[ECHOMAX] = "";
		//RINVIA LA STRINGA
		if(sendto(sock,echoBuffer, recvMsgSize, 0, (struct sockaddr*) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
			errorhandler("sendto() sent different bytes than expected");		
		fflush(stdout);
	}
}

