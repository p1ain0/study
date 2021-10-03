// peview.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

IMAGE_DOS_HEADER dos_header;

IMAGE_NT_HEADERS32 nt_header;
IMAGE_SECTION_HEADER section_header[0xFF];


int main(int argc, char* argv[])
{
	char *filetype;


	if (argc != 2){
		printf("Usage: %s <filename>\n", argv[0]);
		exit(0);
	}
	
	//Is it a execuable file ?
	filetype = strrchr(argv[1], '.');
	if (!(_stricmp(filetype, ".exe") || _stricmp(filetype, ".dll"))){
		printf("This is not a execuable file\n");
		exit(0);
	}


	FILE *fd; 
	if(!(fd = fopen(argv[1], "rb"))){
		fprintf(stderr, "Openfile error!\n");
		exit(0);
	}


	fread(&dos_header, 1, sizeof(IMAGE_DOS_HEADER), fd);
	if(dos_header.e_magic != 0x5a4d){
		printf("This is  not a DOS file.\n");
		fclose(fd);
		exit(0);
	}

	fseek(fd, dos_header.e_lfanew, 0);
	fread(&nt_header, 1, 0x18, fd);
	if(nt_header.Signature != 0x4550){
		printf("This is not a PE file.\n");
		fclose(fd);
		exit(0);
	}

	fread(&nt_header.OptionalHeader, 1, nt_header.FileHeader.SizeOfOptionalHeader, fd);
	
	int i=0;
	for(i=0; i<nt_header.FileHeader.NumberOfSections; i++)
		fread(&section_header[i], 1, sizeof(IMAGE_SECTION_HEADER), fd);
	
	fclose(fd);
	PrintDosHeader();
	PrintNTHeader();
	PrintSextionHeader();
	return 0;
}

