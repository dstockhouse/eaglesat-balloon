#include <stdio.h>
#include <string.h>

void convertToPgm(char * bin);

int main()
{
	FILE * binfile;

	binfile = fopen("data.txt", "rb");
	if(binfile == NULL) {
		printf("error!\n");
		return(0);
	}
	char bin[160 * 120];
	fread(bin, sizeof(char), (160 * 120), binfile);
	convertToPgm(bin);
	fclose(binfile);
	return(0);
}
void convertToPgm(char * bin) {
	printf("Running!");
	const unsigned int ymax = 160;
	const unsigned int xmax = 120;
	const unsigned char * header = "P5\n# Copied binary data\n120 160\n256\n\0";
	unsigned char data[160 * 121 + sizeof(header)];
	int index = sizeof(header);
	strcpy(data, header);
	for (int y = 0; y < ymax; y++) {
		for (int x = 0; x < xmax; x++) {
			data[index] = bin[index];
			index++;
		}
		// data[index] = "\n";
		index++;
	}
	index--;
	data[index] = "\0";
	FILE * fp;
	fp = fopen("image.pgm", "wb");
	fwrite(data, sizeof(data), 1, fp);
	fclose(fp);
	printf("Success!\n");
}
