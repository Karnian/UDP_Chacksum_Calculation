#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned short u16;
typedef unsigned long u32;

struct UDP
{
	char SIA[20];
	char DIA[20];
	int SPN;
	int DPN;
	char PL[100];
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


u16 udp_sum_calc(u16 len_udp, u16 src_addr[], u16 dest_addr[], bool padding, u16 buff[])
{
	u16 prot_udp = 17;
	u16 padd = 0;
	u16 word16;
	u32 sum;

	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	if (padding & 1 == 1) {
		padd = 1;
		buff[len_udp] = 0;
	}

	//initialize sum to zero
	sum = 0;

	// make 16 bit words out of every two adjacent 8 bit words and 
	// calculate the sum of all 16 vit words
	for (int i = 0; i<len_udp + padd; i = i + 2) {
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
	printf("source IP address : ");
	scanf("%s", UDP.SIA);
	// input DIA
	printf("destination IP address : ");
	scanf("%s", UDP.DIA);
	// input SPN
	printf("source port number : ");
	scanf("%d", UDP.SPN);
	// input DPN
	printf("destination port number : ");
	scanf("%d", UDP.DPN);
	// input PL
	printf("udp payload : ");
	scanf("%s", UDP.PL);

	tokking(UDP.SIA, UDP.DIA);

	u16 buff[200];

	for (int i = 0; i < strlen(UDP.PL); i++)
	{
		buff[8 + i] = UDP.PL[i];
	}

	u16 len_udp = 8 + strlen(UDP.PL);

	printf("%d\n", udp_sum_calc(len_udp, SIA, DIA, strlen(UDP.PL) % 2, buff));
}
