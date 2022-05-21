#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 5000
#define BACKLOG 10

void Kill_process(int sig)  //This function is used to kill the process, in order to free the used socket/port
{
	kill((int)getpid(),SIGKILL);
	(void)signal(SIGINT, Kill_process);
}

char *get_useful_information(char *msg) //This function get the name of the requested HTML page
{
	char *p1 = strstr(msg, "GET /");
	char *p2 = strstr(msg, " HTTP");
	
	char *str = malloc(10*sizeof(char));
	int i = 0;
	p1 = p1+5;
	printf("Lungimea numelui este: %ld\n",p2-p2);
	if(p2-p1 != 0)
	{
		while(p1!=p2)
		{
			str[i]= *p1;
			i++;
			p1++;
		}
		return str;
	}
	else 
		return NULL;

}

int main(void)
{
	signal(SIGINT, Kill_process);
	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	char msg[1000];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    printf("EROARE: Nu s-a putut creea socketul\n");
	    exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	{
	    printf("EROARE: Nu s-a initializa socketul\n");
	    exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) 
	{
	    printf("EROARE: Nu s-a putut incepe procesul de ascultare al serverului\n");
	    exit(1);
        }

        while(1) 
        {  
            //-----------------------------------Establish Connection----------------------------------------
            sin_size = sizeof(struct sockaddr_in);
            
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) 
            {
                printf("EROARE: Nu s-a putut accepta un request nou de la un client\n");
                continue;
            }
            
            printf("---------------------------NEW CONNECTION---------------------------\n");
            printf("server: conexiune de la: %s\n\n", inet_ntoa(their_addr.sin_addr));
            
            //------------------------------------------Read Data----------------------------------------------
            
            if((read(new_fd, &msg, sizeof(msg)))<0)
            {
            	printf("EROARE: Nu s-a putut citi informatia\n");
            	continue;
            }
            
            //------------------------------------------Process Request----------------------------------------
            
	    printf("Message:\n\n%s\n",msg);
            char *html_name = get_useful_information(msg);
            if(html_name != NULL)
            {
            	if(strcmp(html_name,"favicon.ico"))
            	{
            		printf("Fisierul HTML dorit este: %s\n\n",html_name);
            	}
            	else
            	{
            		printf("Nu se va returna nimic\n\n");
            	}
            }
            else
            	printf("Se va returna pagina principala\n\n");
            
            
            //------------------------------------Send Data and Close Connection-------------------------------
	    if (send(new_fd, "Salutare Vere!\n", 14, 0) == -1)
		    printf("EROARE: Nu s-a putut trimite informatia catre client\n");
 
            close(new_fd);
           
        }

        return 0;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
