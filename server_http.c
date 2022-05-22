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
#include <pthread.h>

#define MYPORT 5000
#define BACKLOG 10


long thread_id;

struct arg_struct{
	int new_fd;
	int t_id;
};


void Kill_process(int sig)  //This function is used to kill the process, in order to free the used socket/port
{
	kill((int)getpid(),SIGKILL);
	(void)signal(SIGINT, Kill_process);
}

char *get_useful_information(char *msg) //This function get the name of the requested HTML page
{
	char *p1 = strstr(msg, "GET /");
	char *p2 = strstr(msg, " HTTP");
	
	char *str = malloc(20*sizeof(char));
	int i = 0;
	p1 = p1+5;
	printf("Lungimea numelui este: %ld\n",p2-p1);
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

void parse_html_file(char *html_name, char* response)
{
	FILE *html_file = fopen(html_name,"r");
	
	if(html_file == NULL)
		html_file = fopen("404.html","r");
		
	char line[100];
	while(fgets(line,100,html_file)!=0)
	{
		strcat(response,line);
	}
}

void send_response(const char *html_name, int new_fd)
{
	char httpHeader[2000]="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
	
	char nume_complet[10]= {}, response[1000]={};
	
	if(html_name == NULL)
		strcat(nume_complet,"home.html");
	else
		strcat(nume_complet,html_name);
		strcat(nume_complet,".html"); // adding .html part at the end of the html_name in order to serch for it

	parse_html_file(nume_complet, response); // parse the html file and put the data in response

	char lenght_of_html_file[10];
	sprintf(lenght_of_html_file,"%ld\n\n",strlen(response));
	strcat(httpHeader,lenght_of_html_file);  // adding the lenght of the response to the header

	strcat(httpHeader,response);
	printf("%s\n", httpHeader); 

	if(send(new_fd, httpHeader, strlen(httpHeader), 0) == -1)
		printf("EROARE: Nu s-a putut trimite informatia catre client\n");
}

void *proccess_new_client(void *arguments)
{
	struct arg_struct *args = (struct arg_struct *)arguments;
	char msg[1000];
	//------------------------------------------Read Data----------------------------------------------
		    
	if((read(args->new_fd, &msg, sizeof(msg)))<0)
	{
		printf("EROARE: Nu s-a putut citi informatia\n");
		exit(1);
	}

	//----------------------------------Process Request and Send Response--------------------------------

	printf("Message:\n\n%s\n",msg);
	char *html_name = get_useful_information(msg);
	if(html_name != NULL)
	{
		if(strcmp(html_name,"favicon.ico"))
		{
			printf("Fisierul HTML dorit este: %s\n\n",html_name);
			send_response(html_name,args->new_fd);
		}
		else
		{
			printf("Nu se va returna nimic\n\n");
		}
	}
	else
	{
		printf("Se va returna pagina principala\n\n");
		send_response("home",args->new_fd);
	}       

	//-----------------------------------------Close Connection------------------------------------------
	close(args->new_fd);
	
	pthread_exit(NULL);
	
}
int main(void)
{
	signal(SIGINT, Kill_process);
	pthread_t client_threads[20];
	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size, rc;
	

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
	    printf("EROARE: Nu s-a initializat socketul\n");
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
            
            //---------------------------Create a thread for managing every client----------------------------
            
            struct arg_struct args;
            args.new_fd = new_fd;
            args.t_id = thread_id;
            rc = pthread_create(&client_threads[thread_id++], NULL, proccess_new_client, (void *)&args);
            
            printf("Acest client va fi gestionat de threadul nr: %ld\n",thread_id);
            
            if (rc)
		{
			printf("Codul erorii este: %d\n", rc);
			exit(-1);
		}
	    if(thread_id == 18)
	    	thread_id = 0;
            
        }
	pthread_exit(NULL);
        return 0;
}
    
    
  //fuser -k 5000/tcp
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
