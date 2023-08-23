#include <rdp_values.h>
#include <stdexcept>
#include <iostream>

namespace rdp {
  /// RdpValue

  /// @brief  RdpValues constructor.
  /// @param now_time 
  RdpValues::RdpValues(const time_t now_time) {
    struct tm end_struct;
    end_struct=*std::localtime(&now_time);
    end_struct.tm_sec=0;
    end_struct.tm_min=0;
    end_struct.tm_hour=0;
    end=std::mktime(&end_struct);
    start=end-(24*60*60);
    time_t hour = 60*60;
    hum_values[start+0*hour]=0.8;
    hum_values[start+1*hour]=0.85;
    hum_values[start+2*hour]=0.86;
    hum_values[start+3*hour]=0.88;
    hum_values[start+4*hour]=0.84;
    hum_values[start+5*hour]=0.88;
    hum_values[start+6*hour]=0.91;
    hum_values[start+7*hour]=0.95;
    hum_values[start+8*hour]=0.99;
    hum_values[start+9*hour]=0.93;
    hum_values[start+10*hour]=0.85;
    hum_values[start+11*hour]=0.65;
    hum_values[start+12*hour]=0.62;
    hum_values[start+13*hour]=0.58;
    hum_values[start+14*hour]=0.54;
    hum_values[start+15*hour]=0.53;
    hum_values[start+16*hour]=0.49;
    hum_values[start+17*hour]=0.45;
    hum_values[start+18*hour]=0.47;
    hum_values[start+19*hour]=0.51;
    hum_values[start+20*hour]=0.55;
    hum_values[start+21*hour]=0.58;
    hum_values[start+22*hour]=0.57;
    hum_values[start+23*hour]=0.55;
    hum_values[start+24*hour]=0.51;
  }
  time_t RdpValues::start_time() {
    return start;
  }
  time_t RdpValues::end_time() {
    return end;
  }
  float RdpValues::get_hum(const time_t time_point) {
    if (time_point < start || time_point > end) {
      throw std::out_of_range("Index out of bounds");
    }
    return 0.0;
  }

}
