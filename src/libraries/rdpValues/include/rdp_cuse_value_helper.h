#pragma once

#include <rdp_cuse_value.h>

namespace rdp {
    void RdpValue2Buffer(const RdpValue & rdp_value, char *buffer);
}