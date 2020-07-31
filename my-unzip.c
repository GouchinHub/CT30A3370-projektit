/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-unzip
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, char **argv) {
	FILE *zip;
	unsigned char c;
	int i,j;
	if (argc == 1)
		exit (0);
	if ((zip = fopen(argv[1],"rb")) == NULL){
				printf("my-unzip: cannot open file\n");
				exit (1);
		}
	while(1){
		if(feof(zip)){
			break;
		}
		fread(&i, sizeof(i), 1, zip);
		if((c = getc(zip))=='\0')
			continue;
		for(j=0;j<i;j++){
			printf("%c",c);
		}
	}
	fclose(zip);
	return 0;
}

/*******************************************************************/
/* eof */
