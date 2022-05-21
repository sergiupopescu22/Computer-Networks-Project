#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#define MY_PORT_ID 6666 /* a number > 5000 */

int main()
{
	int sockid, newsockid, i, j;
	struct sockaddr_in ssock_addr;
	char msg[255];
	if ((sockid = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(“Eroare creare socket");
		return -1;
	}
	bzero((char *) &ssock_addr, sizeof(ssock_addr));
	ssock_addr.sin_family = AF_INET;
	ssock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ssock_addr.sin_port = htons(MY_PORT_ID);
	if (( bind(sockid, (struct sockaddr *) &ssock_addr,sizeof(ssock_addr)) < 0))
	{
		perror(“Eroare de asociere");
		return -1;
	}
	if ( listen(sockid, 5) < 0)
	{
		perror(“Eroare la ascultare");
		return -1;
	}
	while (1)
	{
		newsockid = accept(sockid, (struct sockaddr *) 0, (int *) 0);
		if (newsockid < 0)
		{
			perror(“Eroare acceptare client");
			return -1;
		}
		if ((read(newsockid, &msg, sizeof(msg))) < 0)
		{
			perror(“Eroare citire din socket");
			return -1;
		}
		printf("Clientul spune: %s\n", msg);
		sprintf(msg, "==> mesaj de la server <==");
		write(newsockid, &msg, sizeof(msg));
		close(newsockid);
	}
	close(sockid);
	return 0;
}