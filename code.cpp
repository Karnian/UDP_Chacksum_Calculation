#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned short u16;
typedef unsigned long u32;

struct UDP
{
	char SIA[20] = "172.30.1.17";
	char DIA[20] = "210.107.197.252";
	int SPN = 2443;
	int DPN = 80;
	char PL[100] = "MMcN Lab.";
}UDP;

u16 SIA[5] = { 0 };
u16 DIA[5] = { 0 };

// tokking IP address
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


u16 udp_sum_calc(u16 len_udp, u16 src_addr[], u16 dest_addr[], int padding, u16 buff[])
{
	u16 prot_udp = 17;
	u16 padd = 0;
	u16 word16;
	u32 sum;

	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	if (padding) {
		padd = 1;
		buff[len_udp] = 0;
	}

	//initialize sum to zero
	sum = 0;

	// make 16 bit words out of every two adjacent 8 bit words and 
	// calculate the sum of all 16 vit words
	for (int i = 0; i < len_udp + padd; i = i + 2) {
		word16 = ((buff[i] << 8) & 0xFF00) + (buff[i + 1] & 0xFF);
		sum = sum + (unsigned long)word16;
	}
	// add the UDP pseudo header which contains the IP source and destinationn addresses
	for (int i = 0; i<4; i = i + 2) {
		word16 = ((src_addr[i] << 8) & 0xFF00) + (src_addr[i + 1] & 0xFF);
		sum = sum + word16;
	}
	for (int i = 0; i<4; i = i + 2) {
		word16 = ((dest_addr[i] << 8) & 0xFF00) + (dest_addr[i + 1] & 0xFF);
		sum = sum + word16;
	}
	// the protocol number and the length of the UDP packet
	sum = sum + prot_udp + len_udp;

	// keep only the last 16 bits of the 32 bit calculated sum and add the carries
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	// Take the one's complement of sum
	sum = ~sum;

	return ((u16)sum);
}

int main()
{
	// input SIA
	printf("source IP address : %s\n", UDP.SIA);
//	scanf("%s", UDP.SIA);
	// input DIA
	printf("destination IP address : %s\n", UDP.DIA);
//	scanf("%s", UDP.DIA);
	// input SPN
	printf("source port number : %d\n", UDP.SPN);
//	scanf("%d", &UDP.SPN);
	// input DPN
	printf("destination port number : %d\n", UDP.DPN);
//	scanf("%d", &UDP.DPN);

	char nullbuff;
//	scanf("%c", &nullbuff);

	// input PL
	printf("udp payload : %s\n", UDP.PL);
//	fgets(UDP.PL, 100, stdin);

	tokking(UDP.SIA, UDP.DIA);
/*
	for (int i = 0; i < 4; i++)
		printf("%d ", SIA[i]);
	printf("\n");

	for (int i = 0; i < 4; i++)
		printf("%d ", DIA[i]);
	printf("\n");
	*/
	u16 buff[200];


	buff[0] = (UDP.SPN >> 8) & 0xFF;
	buff[1] = (UDP.SPN) & 0x00FF;
	buff[2] = (UDP.DPN >> 8) & 0xFF;
	buff[3] = (UDP.DPN) & 0x00FF;
	/*
	for (int i = 0; i < 4; i++)
		printf("%hd ", buff[i]);
	printf("\n");
	*/
	buff[6] = 0;
	buff[7] = 0;

	for (int i = 0; i < strlen(UDP.PL); i++)
	{
		buff[8 + i] = UDP.PL[i];
	}

	u16 len_udp = 8 + strlen(UDP.PL);

	buff[4] = (len_udp >> 8) & 0xFF;
	buff[5] = (len_udp) & 0x00FF;

	/*
	for (int i = 0; i < len_udp; i++)
	{
		printf("%hd ", buff[i]);
		if ((i % 2))
			printf("\n");
	}
	*/
	u16 checksum = udp_sum_calc(len_udp, SIA, DIA, len_udp & 0x01, buff);

	buff[6] = (checksum >> 8) & 0xFF;
	buff[7] = (checksum) & 0x00FF;

	u16 result = udp_sum_calc(len_udp, SIA, DIA, 0, buff);

	printf("checksum : %hd\nresult : %hd\n", checksum, result);
}
