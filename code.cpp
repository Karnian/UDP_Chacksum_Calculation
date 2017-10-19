#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct UDP
{
	char SIA[20];
	char DIA[20];
	int SPN;
	int DPN;
	char PL[100];
}UDP;

int SIA[5] = { 0 };
int DIA[5] = { 0 };

void tokking(char SIP[], char DIP[])
{
	char* SIPtok = strtok(SIP, ".");
	
	for (int i = 0; SIPtok != NULL; i++)
	{
		int l = strlen(SIPtok);
		int t = 1;

		for (int j = l - 1; j >= 0; j--)
		{
			SIA[i] += (SIPtok[j] - '0') * t;
			t *= 10;
		}
		SIPtok = strtok(NULL, ".");
	}

	char* DIPtok = strtok(DIP, ".");

	for (int i = 0; DIPtok != NULL; i++)
	{
		int l = strlen(DIPtok);
		int t = 1;

		for (int j = l - 1; j >= 0; j--)
		{
			DIA[i] += (DIPtok[j] - '0') * t;
			t *= 10;
		}
		DIPtok = strtok(NULL, ".");
	}
}

int main()
{
	// input SIA
	printf("source IP address : ");
	scanf("%s", UDP.SIA);
	// input DIA
	printf("destination IP address : ");
	scanf("%s", UDP.DIA);
	// input SPN
//	printf("source port number : ");
//	scanf("%d", UDP.SPN);
	// input DPN
//	printf("destination port number : ");
//	scanf("%d", UDP.DPN);
	// input PL
//	printf("udp payload : ");
//	scanf("%s", UDP.PL);

	tokking(UDP.SIA, UDP.DIA);

}
