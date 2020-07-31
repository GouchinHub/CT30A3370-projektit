/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: wish
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#define KOKO 100
#define BUFSIZE 4096

char error_message[30] = "An error has occurred\n";

int pmount = 1;

void freememos(char *array[], int amount){
	for(int i=0;i<amount;i++)
		free(array[i]);
}

void process(char * * argv,char *path[BUFSIZE], int pmount,int cmdmount,int NTHREAD){
	int p[2],i,flag = 0; 
	char buffer[BUFSIZE];
	char usepath[BUFSIZE];
	char dest[BUFSIZE];
	for(i=0;i<pmount;i++){
		if(access(path[i],X_OK)==0){
			strcpy(usepath,path[i]);
			break;
		}
	}
	if (strcmp(argv[cmdmount-2],">") == 0){
		strcpy(dest,argv[cmdmount-1]);
		argv[cmdmount-1] = NULL;
		argv[cmdmount-2] = NULL;
		flag = 1;
	}
	strncat(usepath,argv[0],BUFSIZE);
	/* Lähteenä pipe funktion käyttöön child prosessin kanssa on käytetty:
		http://www.microhowto.info/howto/capture_the_output_of_a_child_process_in_c.html */
	if (pipe(p) == -1) {
		  write(STDERR_FILENO, error_message, strlen(error_message));
		  exit(1);
	}	
	pid_t pid = fork();
	if (pid == 0) {
		while ((dup2(p[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
		close(p[1]);
		close(p[0]);
		if (execv(usepath, argv) == -1) {
	           write(STDERR_FILENO, error_message, strlen(error_message));
	           exit(1);
		}
	}else if(pid>0){
		wait(NULL);
	}else{
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	}

    close(p[1]);  
   	while (1) {
		ssize_t count = read(p[0], buffer, sizeof(buffer));
		if (count == -1) {
			if (errno == EINTR) {
			  	continue;
			} else {
			  perror("read");
			  exit(1);
			}
		} else if (count == 0) {
			break;
		} else {
			read(p[0], buffer, BUFSIZE);
			if(flag==1){
				FILE *tied2;
				if ((tied2 = fopen(dest, "w")) == NULL){
					write(STDERR_FILENO, error_message, strlen(error_message));
					exit(1);
				}
				fprintf(tied2,"%s\n", buffer);
				fclose(tied2);
			}else{
				printf("%s\n", buffer); 
			}
		}
	}
	memset(buffer, 0, BUFSIZE);
	close(p[0]);
}

static char *memory(char *str) {
    char *other = malloc(strlen(str) + 1);
    if (other != NULL)
        strcpy (other, str);
    return other;
}


int cmdexit(char *buff){
	if(strcmp(buff,"exit") == 0){
			free(buff);
			exit(0);
		}
	return 0;
}


int run(int argc, char **argv,char *pbuffs[BUFSIZE],char *path[BUFSIZE],int NTHREAD){
	int i=0,j,x=0;
	char * command[KOKO],*buff;
	size_t bufsize = 32;
  	for(x = 0;x<NTHREAD;x++){
  		/*TOKING*/
  		buff = (char *)malloc(bufsize * sizeof(char)+1);
  		strcpy(buff,pbuffs[x]);
		for(i = 0; buff[i] != '\0';i++) {
			if(buff[i] == '\n'){
				buff[i] = '\0';
			}
		}
		if(strcmp(buff,"exit") == 0){
			free(buff);
			exit(0);
		}
		char *jako = strtok(buff, " ");
		i = 0;
		while(jako != NULL){
			command[i++] = memory(jako);
			jako = strtok(NULL, " ");
		}
		command[i] = NULL;
		
		/*pathing*/
		if(strcmp(command[0],"path")==0){
			for(j=1;j<i;j++){
				path[j-1] = (char *)malloc(KOKO * sizeof(char));
				strcpy(path[j-1],command[j]);
			}
			pmount = i;
			return 1;
		}
		/* cd */
		if(strcmp(command[0],"cd")==0){
			if(i != 2){
				write(STDERR_FILENO, error_message, strlen(error_message));
				return 1;
			}
			chdir(command[1]);
			free(buff);
			freememos(pbuffs,NTHREAD);
			return 1;
		}
		process(command,path,pmount,i,NTHREAD);
	}
	return 0;
}
	

int main(int argc, char **argv) {
	size_t bufsize = 32;
	char *buff;
	char *pbuffs[BUFSIZE];
	char *path[BUFSIZE];
	char *trans;
	FILE *tiedosto;
	path[0] = (char *)malloc(KOKO * sizeof(char));
	strcpy(path[0],"/bin/");
	if(argc==2){
		if ((tiedosto = fopen(argv[1], "r")) == NULL){
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(1);
		}
	}
	if (argc >2){
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	}
	while(1){
		buff = (char *)malloc(bufsize * sizeof(char));
		 if( buff == NULL) {
		    write(STDERR_FILENO, error_message, strlen(error_message));
		    exit(1);
		}
		printf("> ");
		if(argc==1)
			getline(&buff,&bufsize,stdin);
		if(buff[0]=='\n')
			continue;
		if(argc==2){
			if(feof(tiedosto)){
				fclose(tiedosto);
				exit(0);
			}
			getline(&buff,&bufsize,tiedosto);
		}
		int NTHREAD = 0;
		int y = 0;
		int x = 0;
		for(int i = 0; buff[i] != '\0';i++) {
			if(buff[i]=='&'&&buff[i+1]==' '&&buff[i-1]==' ' ){
				pbuffs[NTHREAD] = (char *)malloc(KOKO * sizeof(char));
				trans = (char *)calloc(BUFSIZE,KOKO * sizeof(char));
				y = 0;
				while(x<i-1){
					trans[y]=buff[x];
					y++;
					x++;
				}
				x+=3;
				strcpy(pbuffs[NTHREAD],trans);
				NTHREAD++;
				free(trans);
			}
			if(buff[i] == '\n'){
				pbuffs[NTHREAD] = (char *)malloc(KOKO * sizeof(char));
				trans = (char *)calloc(BUFSIZE,KOKO * sizeof(char));
				y = 0;
				while(x<i){
					trans[y]=buff[x];
					y++;
					x++;
				}
				x+=2;
				strcpy(pbuffs[NTHREAD],trans);
				NTHREAD++;
				free(trans);
			}
		}
		if (run(argc,argv,pbuffs,path,NTHREAD) == 1)
			continue;
		
	}

	return 0;
}

/*******************************************************************/
/* eof */
