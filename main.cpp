#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short *resimdizi_org = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i*N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu){
		case 1:
			Dilation(N*M, filter_size, resimdizi_org);
			resimYaz(dil_resim, resimdizi_org, N, M, Q);
			break;
		case 2:
			Erosion(N*M, filter_size, resimdizi_org);
			resimYaz(ero_resim, resimdizi_org, N, M, Q);
			break;
		case 3:
			system("EXIT");
			break;
		default:
			system("EXIT");
			break;
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* resim_org) {
	
	//for (int i = 0; i < 100; i++)resim_kopya[i] = resim_org[i];
	/*for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			resim_org[i][j] = 255;*/
	__asm {

		MOV ESI, resim_org
		XOR EAX, EAX // i
		DIS :
		CMP EAX , n
		JNB DIS_SON
		XOR EBX ,EBX // j
		IC:
		CMP EBX , 512
		JNB IC_SON
		XOR DI, DI // max
		XOR ECX , ECX  // K
		PUSH EAX 
		PUSH EDX
		PUSH EBX
		MOV EAX , filter_size
		SHR EAX , 1
		XOR EDX , EDX
		MOV EBX , 512
		MUL EBX
		MOV ECX , EAX
		NEG ECX
		POP EBX
		POP EDX
		POP EAX
		ADD ECX , EAX  // (i - (filter_size / 2) * 512)
		SATIR :
		MOV EDX, 3 
		SHR EDX, 1
		NEG EDX
		ADD EDX, EBX // (j - filter_size / 2)
		SUTUN :
		PUSH EBX
		PUSH AX
		XOR EBX , EBX
		ADD EBX , ECX 
		ADD EBX , EDX
		CMP EBX , 0
		JL SUTUN_SON 
		CMP EBX , n
		JG SUTUN_SON
		ADD EBX, EBX
		MOV AX ,WORD PTR  [ESI+EBX]
		CMP DI , AX
		JA SUTUN_SON
		MOV DI , AX
		SUTUN_SON :
		POP AX
		POP EBX
		INC EDX 
		PUSH EAX 
		MOV EAX ,filter_size
		SHR EAX , 1
		ADD EAX , EBX 
		CMP EDX , EAX
		POP EAX 
		JNG SUTUN
		ADD ECX , 512
		PUSH EBX 
		PUSH EDX 
		PUSH EAX 
		MOV EAX , filter_size
		SHR EAX , 1
		XOR EDX , EDX
		MOV EBX , 512 
		MUL EBX
		MOV EBX , EAX
		POP EAX 
		POP EDX 
		ADD EBX , EAX
		CMP ECX , EBX
		POP EBX 
		JNG SATIR
		PUSH DI 
		INC EBX 		
		JMP IC
		IC_SON :
		ADD EAX , 512
		JMP DIS 		
		DIS_SON :
		MOV ECX , n
		MOV EDI , n
		ADD EDI , EDI
		SUB EDI , 2


		L1 :
		POP AX 
		MOV WORD PTR [ESI + EDI], AX
		SUB EDI , 2
		LOOP L1
			


	} 


	printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
}

void Erosion(int n, int filter_size, short* resim_org) {

	__asm {
		MOV ESI, resim_org

		XOR EAX, EAX // i
		DIS :
		CMP EAX, n
		JNB DIS_SONß
		XOR EBX, EBX // j
		IC :
		CMP EBX, 512
		JNB IC_SON
		MOV DI, 255 // min
		XOR ECX, ECX  // K
		PUSH EAX
		PUSH EDX
		PUSH EBX
		MOV EAX, filter_size
		SHR EAX, 1
		XOR EDX, EDX
		MOV EBX, 512
		MUL EBX
		MOV ECX, EAX
		NEG ECX
		POP EBX
		POP EDX
		POP EAX
		ADD ECX, EAX  // (i - (filter_size / 2) * 512)
		SATIR :
		MOV EDX, 3
		SHR EDX, 1
		NEG EDX
		ADD EDX, EBX // (j - filter_size / 2)
		SUTUN :
		PUSH EBX
		PUSH AX
		XOR EBX, EBX
		ADD EBX, ECX
		ADD EBX, EDX
		CMP EBX, 0
		JL SUTUN_SON
		CMP EBX, n
		JG SUTUN_SON
		ADD EBX, EBX
		MOV AX, WORD PTR[ESI + EBX]
		CMP DI, AX
		JB SUTUN_SON
		MOV DI, AX
		SUTUN_SON :
		POP AX
		POP EBX
		INC EDX
		PUSH EAX
		MOV EAX, filter_size
		SHR EAX, 1
		ADD EAX, EBX
		CMP EDX, EAX
		POP EAX
		JNG SUTUN
		ADD ECX, 512
		PUSH EBX
		PUSH EDX
		PUSH EAX
		MOV EAX, filter_size
		SHR EAX, 1
		XOR EDX, EDX
		MOV EBX, 512
		MUL EBX
		MOV EBX, EAX
		POP EAX
		POP EDX
		ADD EBX, EAX
		CMP ECX, EBX
		POP EBX
		JNG SATIR
		PUSH DI
		INC EBX
		JMP IC
		IC_SON :
		ADD EAX, 512
		JMP DIS
		DIS_SON :
		MOV ECX, n
		MOV EDI, n
		ADD EDI, EDI
		SUB EDI, 2

		L1 :
		POP AX
		MOV WORD PTR[ESI + EDI], AX
		SUB EDI, 2
		LOOP L1
		
	} 

	

	printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
}
