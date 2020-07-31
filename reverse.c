/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: reverse
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **side(char nimi[],char output[],int valinta){
	FILE *tiedosto;
	if ((tiedosto = fopen(nimi,"r")) == NULL) {
			fprintf(stderr,"error: cannot open file '%s'\n",nimi);			
			exit(1);
		}
	int i,count=0;
	char *line = NULL;
	size_t bufkoko = 0;
	size_t koko;
	char** muisti = NULL;
	koko = getline(&line,&koko,tiedosto);
	while(feof(tiedosto) == 0) {
		koko = getline(&line,&bufkoko,tiedosto);
		count++;
	}
	rewind(tiedosto);
	if ((muisti = malloc((count+1)*sizeof(char))) == NULL){
		fprintf(stderr,"malloc failed");
		exit(1);
	}
	count = 0;
	while(feof(tiedosto) == 0) {
		if ((muisti[count] = malloc((koko +1)*sizeof(char))) == NULL){
			fprintf(stderr,"malloc failed");
			exit(1);
		}
		strcpy(muisti[count],line);
		count++;
		koko = getline(&line,&bufkoko,tiedosto);
	}
	fclose(tiedosto);
	if (valinta==1||valinta==3){
		printf("Content of file '%s' reverted: \n",nimi);
		for(i=count-1;i >= 1;i--){
			printf("%s",muisti[i]);
		}
	}else{
		FILE *tiedosto2;
		if ((tiedosto2 = fopen(output,"w")) == NULL) {
			fprintf(stderr,"error: cannot open file '%s'\n",nimi);			
			exit(1);
		}
		for(i=count-1;i >= 1;i--){
			fprintf(tiedosto2,"%s",muisti[i]);
		}
		printf("Content of file '%s' reverted and imported to '%s'. \n",nimi,output); 
	}
	free(muisti);
	return(0);
}

int main(int argc, char *argv[]) {
	int i;
	char name[999];
	for(i = 1; i < argc; i++)
		i = argc;
	if(i > 4){
		printf("usage: reverse <input> <output>\n");
		exit(0);
	}
	if(i == 1){
		printf("Give name of input file: ");
		scanf("%s",name);
		side(name,NULL,i);
	}
	if(i == 3) {
		side(argv[1],NULL,i);
	}
	if(i == 4) {
		if (strcmp(argv[1],argv[2])==0){
			printf("Input and output file must differ\n");
			exit(1);
		}
		side(argv[1],argv[2],i);
	}
	return (0);
}

/*******************************************************************/
/* eof */
