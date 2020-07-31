/*******************************************************************/
/* CT30A3370_01.06.2020 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-zip
 * Tekijä: Aatu Laitinen & Susanna Lähde
 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i=0,in,comp=0,c=0;
	FILE *tiedosto;
	FILE *zip;
	if (argc == 1)
		exit (0);
	if ((zip = fopen("file.zip","wb")) == NULL){
				printf("my-zip: cannot open file\n");
				exit (1);
		}
	for(in=1;in<argc;in++){
		if ((tiedosto = fopen(argv[in],"r")) == NULL){
				printf("my-zip: cannot open file\n");
				exit (1);
		}
		while(1){
			/*Lähteenä RLE käytetty: 
				https://www.rosettacode.org/wiki/Run-length_encoding/C */
			c = fgetc(tiedosto);
			if (feof(tiedosto))
				break;
			if (c == comp){
				i++;
			}else{
				fwrite(&i,4,1,zip);
				fwrite(&comp,1,1,zip);
				i = 1;
			}
			comp = c;
		}
		fwrite(&i,4,1,zip);
		fwrite(&comp,1,1,zip);
	}
	printf("File compressed to file.zip\n");
	fclose(tiedosto);
	fclose(zip);
	return 0;
}

/*******************************************************************/
/* eof */
