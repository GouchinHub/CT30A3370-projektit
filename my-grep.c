/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-grep
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KOKO 4096

int main(int argc, char **argv) {
	size_t bufsize = 32;
	char *buff;
	int i;
	FILE *tiedosto;
	if (argc == 1){
		printf("my-grep: searchterm [file ...]\n");
		exit (0);
	}
	if(argc == 2){
		while(1) {
			buff = (char *)malloc(bufsize * sizeof(char));
			if( buff == NULL) {
				perror("Error allocating memory");
				exit(1);
			}
			getline(&buff,&bufsize,stdin);
			if (strcmp(buff,"\n")==0)
				break;
			if (strstr(buff,argv[1])){
				printf("%s",buff);
			}
		}
		free(buff);
		return 0;
	}
	for(i=2;i<argc;i++){
		if ((tiedosto = fopen(argv[i],"r")) == NULL){
				perror("my-grep: cannot open file");
				exit (1);
		}
		while(1) {
			buff = (char *)malloc(bufsize * sizeof(char));
			if( buff == NULL) {
				perror("Error allocating memory");
				exit(1);
			}
			if(feof(tiedosto)){
				fclose(tiedosto);
				break;
			}
			getline(&buff,&bufsize,tiedosto);
			if (strstr(buff,argv[1])){
				printf("%s",buff);
			}
		}
		free(buff);
	}
	return 0;
}

/*******************************************************************/
/* eof */
