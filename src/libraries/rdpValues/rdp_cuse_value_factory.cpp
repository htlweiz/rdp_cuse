#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#
#include <rdp_cuse_value_factory.h>

namespace rdp {
    RdpValueStore RdpValueFactory::create(time_t now, const char * values_file) {
        RdpValueStore the_store = RdpValueStore(now);
        std::ifstream in_stream;
        std::string single_line;
        unsigned int line_count=0;
        in_stream.open(values_file);
        while(true) {
            std::getline(in_stream,single_line);
            if (in_stream.fail())
            {
                break; // FIXXME assume eof reached 
            }
            // read time offset
            std::stringstream line(single_line);
            std::string time_offset_str="";
            std::getline(line, time_offset_str, ',');
            if (line.fail()) {
                throw std::exception();
            }
            std::stringstream time_offset_stream(time_offset_str);
            time_t time_offset;
            time_offset_stream >> time_offset;
            if (time_offset_stream.fail()) {
                throw std::exception();
            }

            // read value type
            std::string value_type_str="";
            std::getline(line, value_type_str, ',');
            if (line.fail()) {
                throw std::exception();
            }
            std::stringstream value_type_stream(value_type_str);
            RdpValueType value_type = RDP_NUM_VALUE_TYPES;
            unsigned int value_type_int;
            value_type_stream >> value_type_int;
            if (value_type_stream.fail()) {
                throw std::exception();
            }             
            
            if (value_type_int==0) {
                value_type=RDP_TEMP_VALUE_TYPE;
            } else if (value_type_int==1) {
                value_type=RDP_HUM_VALUE_TYPE;
            } else if (value_type_int==2) {
                value_type=RDP_PRESS_VALUE_TYPE;
            } else {
                throw std::exception();
            }

            // read value
            std::string value_str="";
            std::getline(line, value_str, ',');
            if (line.fail()) {
                throw std::exception();
            }

            std::stringstream value_stream(value_str);
            float value;
            value_stream >> value;
            if (value_stream.fail()) {
                throw std::exception();
            }

            // std::cout << "Got a value: " << time_offset << " " << value_type << " " << value << std::endl;
            the_store.add_value(value_type, time_offset, value);
        }
        return the_store;
    }
}