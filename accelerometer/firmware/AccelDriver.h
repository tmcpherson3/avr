// AccelDriver.h

#ifndef AccelDriverHeader
#define AccelDriverHeader

#include <stdint.h>

struct AccelData
{
  double Ax;
  double Ay;
  double Az;
  double Temp;
  double Gx;
  double Gy;
  double Gz;
  
};

class AccelDriver
{
 public:
  AccelDriver();
  ~AccelDriver();
  AccelData readData();

 private:
  void initialize();
  void error();
  int16_t readNextByte(); 
  
  
};

#endif
