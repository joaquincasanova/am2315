#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <bcm2835.h>

using namespace std;
 
int address = 0x5c;//92
int func=3;
int start=0;
int num=4;
int i, j;
double rhtfactor = 0.1;
double t, rh;

int rhtread() {

  char writebuf[3] = {func, start, num};
  char readbuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned short c, crc;
  int delay[2]={158000, 1000000};

  for(i=0;i<2;i++){
    bcm2835_i2c_write(&writebuf[0], 3);
  };
  usleep(delay[0]);
  bcm2835_i2c_read(&readbuf[0],8);
  usleep(delay[1]);

  crc = (readbuf[7]<<8)+readbuf[6];

  if(readbuf[1]!=writebuf[2] || readbuf[0]!=writebuf[0]){
    cout<<"Failed AM2315 read"<<std::endl;
    return -1;
  }else{
    rh = rhtfactor * (double)(readbuf[3]+(readbuf[2]<<8));
    t = rhtfactor * (double)(readbuf[5]+(readbuf[4]<<8));
    cout<<t<<" "<<rh<<std::endl;
    c = 0xFFFF;
    for(int l=0;l<6;l++){
      c^=readbuf[l];
	for(int i=0;i<8;i++){
	  if(c & 0x01){
	    c>>=1;
	    c^=0xA001;
	  }else{
	    c>>=1;
	  };
	};
    };
    if(crc!=c){cout<<"CRC check failed"<<std::endl; return -1;};
    /*
    cout<<"CRC check "<<crc<<" "<<c<<std::endl;
    for(i=0;i<3;i++){
      cout<<"writebuf "<<i<<" "<<int (writebuf[i])<<std::endl;
    };
    for(i=0;i<8;i++){
      cout<<"readbuf "<<i<<" "<<int (readbuf[i])<<std::endl;
      };*/ 
  };
  return 0;
}

int main(void){

  if(!bcm2835_init()){
    cout<<"Failed bcm2835 init"<<std::endl;
    return -1;
  };

  bcm2835_i2c_set_baudrate(100000);
   
  bcm2835_i2c_begin();
  bcm2835_i2c_setSlaveAddress(address);
  
  for(j=0;j<100;j++){
    rhtread();
  };
  
  bcm2835_i2c_end();
  bcm2835_close();
  
  return 0;

};
