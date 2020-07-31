/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-cat
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	size_t bufsize = 32;
	char *buff;
	int i;
	FILE *tiedosto;
	if (argc == 1)
		exit (0);
	for(i=1;i<argc;i++){
		if ((tiedosto = fopen(argv[i],"r")) == NULL){
				printf("my-cat: cannot open file\n");
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
			printf("%s",buff);
		}
		printf("\n");
		free(buff);
	}
	return 0;
}

/*******************************************************************/
/* eof */
