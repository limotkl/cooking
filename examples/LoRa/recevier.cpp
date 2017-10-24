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
#include "arduPiLoRa.h"
#include <stdlib.h>
#include <string.h>

int e;
int re =0;
char my_packet[300];
void setup(int i)
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets without ACK\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e = sx1272.setMode(i);
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
  e = sx1272.setNodeAddress(8);
  printf("Setting Node address: state %d\n", e);
  
  // Print a success message
  printf("SX1272 successfully configured\n\n");
  delay(1000);
}

void loop(void)
{ 
// Receive message
  e = sx1272.receivePacketTimeout(50000);
FILE *fp;
fp = fopen("log0.txt","a+");
if(fp == 0)
{
	printf("can't open file");
}

  if ( e == 0 )
  {
	re++;    
	printf("Receive packet, state %d\n",e);
	printf("NO.%d packets\n",re);
	fprintf(fp,"NO.%d packets\n",re);
	int num;
	num = sx1272.packet_received.length;
	printf("message length:%d\n",num);
	fprintf(fp,"message length:%d\n",num);
    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
	if(my_packet[i]=='A')
	{
		printf("=========received %d packets=========",re);
		fprintf(fp,"====received %d     =====",re);
		fprintf(fp,"====message length:%d====\n",num);
		fprintf(fp,"====Message: %s====\n", my_packet);
		re =0;
	}
    }
	
    printf("Message: %s\n", my_packet);
    fprintf(fp,"Message: %s\n", my_packet);
  }

else {
	printf("Receive packet failed, state %d\n",e);
	//fprintf(fp,"Receive packet failed, state %d\n",e);
  }

fflush(fp);
fclose(fp);

}

int main (int argc,const char* argv[]){
	setup(atoi(argv[1]));
	//setup();
	while(1){
		loop();
	}
	return (0);
}

        
