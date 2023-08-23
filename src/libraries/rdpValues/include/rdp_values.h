#pragma once

#include <ctime>
#include <exception>
#include <list>
#include <map>

namespace rdp {

  class RdpValues {
    public:
      RdpValues(const time_t now_time=0);
      time_t start_time();
      time_t end_time();
      float get_hum(const time_t time_point);
    protected:
      time_t start;
      time_t end;
      std::map<time_t, float> hum_values;
  };

}
