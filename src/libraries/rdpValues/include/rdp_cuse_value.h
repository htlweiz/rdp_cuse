#pragma once

#include <list>
#include <ctime>

namespace rdp {

  enum RdpValueType {
    RDP_TEMP_VALUE_TYPE,
    RDP_HUM_VALUE_TYPE,
    RDP_PRESS_VALUE_TYPE,
    RDP_NUM_VALUE_TYPES,
  };

  class RdpValue{ 
    public:
      RdpValue(const rdp::RdpValueType value_type, const time_t value_time, const float value);
      RdpValueType get_type() const;
      time_t get_time() const;
      float get_value() const;
    protected:
      const RdpValueType value_type;
      const time_t value_time;
      const float value;
  };

  class RdpValueStore {
    public:
      RdpValueStore(const time_t now=0);
      void add_value(const RdpValueType value_type, const time_t now_offset, float value);
      RdpValue get_value();
      size_t get_value_count() const;
    protected:
      const time_t now;
      std::list<RdpValue> values;
      std::list<RdpValue>::iterator position;
  };

}