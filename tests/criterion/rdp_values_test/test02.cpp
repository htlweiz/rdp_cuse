#include <criterion/criterion.h>

#include <iostream>
#include <iomanip>

#include <rdp_cuse.h>

using std::clog;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;

Test(helper, passing) {
  cr_assert(1);
  rdp::RdpValue t(rdp::RDP_HUM_VALUE_TYPE, 1, 289.3);
  char test_buffer[200];
  memset(test_buffer, 0, 200);
  rdp::RdpValue2Buffer(t, test_buffer);
  
  std::clog << "!!! dumping buffer !!!" << std::endl;
  std::clog << hex << setfill('0');
  for(int i = 0; i<64; i++) {
    if ((i&16)==0) {
      clog << setw(8) << i;
    }
    if ((i&8)==0) {
      clog << ' '; 
    }
    clog << ' ' << setw(2) << (unsigned int)(unsigned char) test_buffer[i];
    if ((i&16)==0) {
      clog << endl;
    }
  }
  clog << endl;


  clog << "Sizes :" << sizeof(time_t) << " " << sizeof(rdp::RdpValueType) << " " << sizeof(float) << endl; 
  size_t position=0;
  time_t * time = (time_t *) &test_buffer[position];
  position+=sizeof(time_t);
  rdp::RdpValueType * type = (rdp::RdpValueType *) &test_buffer[8];
  position+=sizeof(rdp::RdpValueType);
  float *value = (float *) &test_buffer[position];
  
  clog << "Time : " << *time << endl;
  clog << "Type : " << *type << endl;
  clog << "Value: " << *value << endl;
  
  cr_assert(*time == 1);
  cr_assert(*type==rdp::RDP_HUM_VALUE_TYPE);
  cr_assert(*value=289.3);
}
