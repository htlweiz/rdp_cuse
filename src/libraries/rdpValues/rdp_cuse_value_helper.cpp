#include <rdp_cuse_value_helper.h>
#include <cstring>

#include <iostream>

namespace rdp {
    void RdpValue2Buffer(const RdpValue & rdp_value, char* buffer) {        
        float value = rdp_value.get_value();
        size_t value_size = sizeof(float);
        RdpValueType type = rdp_value.get_type();
        size_t type_size = sizeof(RdpValueType);
        time_t time = rdp_value.get_time();
        size_t time_size=sizeof(time_t);
        size_t all_size = value_size+ type_size+ time_size;
        size_t position=0;
        
        /// FIXXME Warning buffer must be big enough, we will not test this.
        std::cerr << "2B ERR Type is: " << type << std::endl;
        std::memcpy(buffer+position,&time,time_size);
        position+=time_size;
        std::memcpy(buffer+position,&type,type_size);
        position+=type_size;
        std::memcpy(buffer+position,&value,value_size);
    }

}
/// fixxme