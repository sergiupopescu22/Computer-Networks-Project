#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MY_PORT_ID 6666 /* numar > 5000 */
#define SERV_HOST_ADDR "10.6.14.190"

int main()
{
	int sockid;
	struct sockaddr_in ssock_addr;
	char msg[255];
	
	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Eroare creare socket");
		return -1;
	}
	
	bzero((char *) &ssock_addr, sizeof(ssock_addr));
	ssock_addr.sin_family = AF_INET;
	ssock_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	ssock_addr.sin_port = htons(MY_PORT_ID);
	
	if (connect(sockid, (struct sockaddr *) &ssock_addr,sizeof(ssock_addr)) < 0)
	{
		perror("Eroare conectare la server");
		return -1;
	}
	
	sprintf(msg, "==> mesaj de la client <==");
	
	if ( (write(sockid, &msg, sizeof(msg))) < 0)
	{
		perror(“Eroare scriere in socket");
		return -1;
	}
	
	bzero((char *) &msg, sizeof(msg));
	
	if ( (read(sockid, &msg, sizeof(msg))) < 0)
	{
		perror("Eroare citire din socket");
		return -1;
	}
	
	printf("Serverul spune: %s\n", msg);
	close(sockid);
	
	return 0;
}