

#include "arduPiLoRa.h"
#include <string.h> 
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.1
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana & Victor Boria
 */
 
// Include the SX1272 and SPI library: 
int e;
int i;
//char message1 [] = "Packet 1, wanting to see if received packet is the same as sent packet";
//char message2 [] = "Packet 2, broadcast test";

void setup(int y)
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send packets without ACK\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e = sx1272.setMode(y);
  printf("Setting Mode: state %d\n", e);


//sx1272.setCR(CR_5);
//sx1272.setSF(SF_12);
//sx1272.setBW(BW_500);
  
  // Set header
  e = sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);
  
  // Select frequency channel
  e = sx1272.setChannel(CH_10_900);
  printf("Setting Channel: state %d\n", e);
  

  // Set CRC
  e = sx1272.setCRC_OFF();
  printf("Setting CRC ON: state %d\n", e);
  

  // Select output power (Max, High or Low)
  e = sx1272.setPower('H');
  printf("Setting Power: state %d\n", e);
  
  // Set the node address
  e = sx1272.setNodeAddress(3);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  printf("SX1272 successfully configured\n\n");
  delay(1000);
}

void loop(char message[],int x)
{
	// Send message1 and print the result
    e = sx1272.sendPacketTimeout(8,message);
    i++;
	printf("NO.%d Packet sent, state %d\n",i,e);
	printf("message :%s\n",message);
    	delay(x);
 
    // Send message2 broadcast and print the result
    //e = sx1272.sendPacketTimeout(0, message2);
    //printf("Packet sent, state %d\n",e);
    
    //delay(4000);
}

int main (int argc, const char * argv[]){

FILE *fp;
fp = fopen("logtime.txt","a+");

	setup(atoi(argv[3]));
	//setup();
	i =0 ;
int n,x;
n = atoi(argv[1]);//message len
x = atoi(argv[2]);//delay
fprintf(fp,"====message len:%d    delay:%d ======\n",n,x);

	char message1[n+2];
	//memset(message1,'\0',sizeof(message1));

	memset(message1,'0',sizeof(message1));
	int index = strlen(argv[2]);
int temp;
	//printf("====%d=====",index);
for(temp =0 ;temp < index;temp++)
{
	message1[temp]= argv[2][temp];
}
	
message1[n] = '\0';

struct timeval start;
struct timeval end;
unsigned long timer;
gettimeofday(&start,NULL);


	while(1){
		loop(message1,x);
		if(i==atoi(argv[4]))
       		{ 
				gettimeofday(&end ,NULL);
				message1[n-2]='A';
			for(n=0;n<5;n++)
			{
				
				loop(message1,x);
			}
			i =0;
			break;
        	}
	}
	
	timer = 1000000 *(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	printf("======time :%ld===========\n",timer);

	fprintf(fp,"======time :%ld===========\n\n\n",timer);
	fflush(fp);
	fclose(fp);
	return (0);
}

         
