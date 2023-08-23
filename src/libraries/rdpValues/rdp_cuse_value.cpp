#include <iostream>

#include <rdp_cuse_value.h>


 namespace rdp {
  /// RdpValue

  RdpValue::RdpValue(const RdpValueType value_type, const time_t value_time, const float value) 
  :value_type(value_type), value_time(value_time), value(value)
  {

  }
  RdpValueType RdpValue::get_type() const {
    return value_type;
  }
  time_t RdpValue::get_time() const {
    return value_time;
  }
  float RdpValue::get_value() const {
    return value;
  }

  /// RdpValueStore

  /// @brief RdpValueStore constructor
  /// @param now the starting value from this Store
  RdpValueStore::RdpValueStore(const time_t now) : now(now), position(values.end()) {

  }
  void RdpValueStore::add_value(const rdp::RdpValueType value_type,
      const time_t now_offset,
      float value) {
    std::clog << "time offset is: " << now_offset << std::endl;
    rdp::RdpValue tmp_value(value_type, now+now_offset, value);
    values.push_back(tmp_value);
  }
  RdpValue RdpValueStore::get_value() {
    if (position==values.end()) {
      position=values.begin();
    }
    auto return_value=*position;
    position++;
    return return_value;
  }

 }