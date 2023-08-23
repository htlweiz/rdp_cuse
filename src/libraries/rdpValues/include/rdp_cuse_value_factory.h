#pragma once

#include <ctime>

#include <rdp_cuse_value.h>

namespace rdp {
    class RdpValueFactory {
        public:
        static RdpValueStore create(time_t now, const char * values_file);
    };
}