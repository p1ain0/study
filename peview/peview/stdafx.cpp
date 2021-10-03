// stdafx.cpp : 只包括标准包含文件的源文件
// peview.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include "stdafx.h"

extern IMAGE_DOS_HEADER dos_header;

extern IMAGE_NT_HEADERS32 nt_header;
extern IMAGE_SECTION_HEADER section_header[0xFF];
// TODO: ÔÚ STDAFX.H ÖÐ
// ÒýÓÃÈÎºÎËùÐèµÄ¸½¼ÓÍ·ÎÄ¼þ£¬¶ø²»ÊÇÔÚ´ËÎÄ¼þÖÐÒýÓÃ
void PrintDosHeader(){
	char *header[]={"e_magic", "e_cblp", "e_cp", "e_crlc", 
					"e_parhdr", "e_minalloc", "e_maxalloc", "e_ss", 
					"e_sp", "e_csum", "e_ip", "e_cs", 
					"e_lfarlc", "e_ovno", "e_res[0]}", "e_res[1]",
					"e_res[2]", "e_res[3]", "e_oemid", "e_oeminfo",
					"e_res2[0]", "e_res2[1]", "e_res2[2]", "e_res2[3]", 
					"e_res2[4]", "e_res2[5]", "e_res2[6]", "e_res2[7]",
					"e_res2[8]", "e_res2[9]", "e_lfanew"};
	int i=0;
	printf("IMAGE_DOS_HEADER:\n");
	for(i=0; i<30; i++){
		printf("\t%-20s:    %04x\n", header[i], *(&(dos_header.e_magic)+i));
	}
	printf("\t%-20s:%08x\n", header[i], dos_header.e_lfanew);


}

void PrintNTHeader(){
	char *Signature = "Signature";
	char *FileHeader = "FileHeader";
	char *OptionalHeader_name = "OptionalHeader";
	char *FHeader[] = {"Machine", "NumberOfSection", "TimeDataStamp", "PointerToSymbolTable", "NumberOfSymbols", "Size0fOptionalHeader", "Characteristics"};
	char *OptionalHeader[20] = {"Magic", "SizeOfCode", "AddressOfEntryPoint", "BaseOfCode", "ImageBase", "SectionAlignment", "FileAlignment", "SizeOfImage", "SizeOfHeaders", "Subsystem", "NumberOfRvaAndSizes"};
	char *DataDirectory[] = {"EXPORT Table", "IMPORT Table", "RESOURCE Table", "EXCEPTION Table", "SECURITY Table", "BASERELOC Table", "DEBUG Diretory", "COPYRIGHT", "GLOBALPTR", "TLS Directory", "LOAD_CONFIG", 
							 "BOUND_IMPORT", "IAT", "DELAY_IMPORT", "COM_DESCRIPTOR", "Reserved Table"};
	printf("IMAGE_NT_HEADER:\n");
	printf("\t%-20s:%08x\n", Signature, nt_header.Signature);
	printf("\t%-20s:\n", FileHeader);
	printf("\t\t%-20s:    %04x\n", FHeader[0], nt_header.FileHeader.Machine);
	printf("\t\t%-20s:    %04x\n", FHeader[1], nt_header.FileHeader.NumberOfSections);
	printf("\t\t%-20s:%08x\n", FHeader[2], nt_header.FileHeader.TimeDateStamp);
	printf("\t\t%-20s:%08x\n", FHeader[3], nt_header.FileHeader.PointerToSymbolTable);
	printf("\t\t%-20s:%08x\n", FHeader[4], nt_header.FileHeader.NumberOfSymbols);
	printf("\t\t%-20s:    %04x\n", FHeader[5], nt_header.FileHeader.SizeOfOptionalHeader);
	printf("\t\t%-20s:    %04x\n", FHeader[6], nt_header.FileHeader.Characteristics);
	
	printf("\t%-20s:\n", OptionalHeader_name);
	printf("\t\t%-20s:    %04x\n", OptionalHeader[0], nt_header.OptionalHeader.Magic);
	printf("\t\t%-20s:%08x\n", OptionalHeader[1], nt_header.OptionalHeader.SizeOfCode);
	printf("\t\t%-20s:%08x\n", OptionalHeader[2], nt_header.OptionalHeader.AddressOfEntryPoint);
	printf("\t\t%-20s:%08x\n", OptionalHeader[3], nt_header.OptionalHeader.BaseOfCode);
	printf("\t\t%-20s:%08x\n", OptionalHeader[4], nt_header.OptionalHeader.ImageBase);
	printf("\t\t%-20s:%08x\n", OptionalHeader[5], nt_header.OptionalHeader.SectionAlignment);
	printf("\t\t%-20s:%08x\n", OptionalHeader[6], nt_header.OptionalHeader.FileAlignment);
	printf("\t\t%-20s:%08x\n", OptionalHeader[7], nt_header.OptionalHeader.SizeOfImage);
	printf("\t\t%-20s:%08x\n", OptionalHeader[8], nt_header.OptionalHeader.SizeOfHeaders);
	printf("\t\t%-20s:    %04x\n", OptionalHeader[9], nt_header.OptionalHeader.Subsystem);
	printf("\t\t%-20s:%08x\n", OptionalHeader[10], nt_header.OptionalHeader.NumberOfRvaAndSizes);


	printf("\t\tDataDirectory       :\n");
	int i=0;
	for(i=0; i<16; i++){
		printf("\t\t\t%-20s:\n", DataDirectory[i]);
		printf("\t\t\t  VA                :%08x\n",  nt_header.OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("\t\t\tSize                :%08x\n",  nt_header.OptionalHeader.DataDirectory[i].Size);
	}


}

void PrintSextionHeader(){
	char *section[]={"VirtureSize", "VirtualAddress", "SizeOfRawData", "PointerToRawData", "PointerToRelocations", "PointerToLinenumbers", "NumberOfRelocations","NumberOfLinumber", "Characteristics"};
	printf("IMAGE_SECTION_HEADER:\n");
	int i=0;
	for(i=0; i<nt_header.FileHeader.NumberOfSections; i++){
		printf("\t%-20s:\n", section_header[i].Name);
		printf("\t\t%-20s:%08x\n", section[0], section_header[i].Misc);
		printf("\t\t%-20s:%08x\n", section[1], section_header[i].VirtualAddress);
		printf("\t\t%-20s:%08x\n", section[2], section_header[i].SizeOfRawData);
		printf("\t\t%-20s:%08x\n", section[3], section_header[i].PointerToRawData);
		printf("\t\t%-20s:%08x\n", section[4], section_header[i].PointerToRelocations);
		printf("\t\t%-20s:%08x\n", section[5], section_header[i].PointerToLinenumbers);
		printf("\t\t%-20s:    %04x\n", section[6], section_header[i].NumberOfRelocations);
		printf("\t\t%-20s:    %04x\n", section[7], section_header[i].NumberOfLinenumbers);
		printf("\t\t%-20s:%08x\n", section[8], section_header[i].Characteristics);
	}
}