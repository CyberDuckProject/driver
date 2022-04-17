#ifndef DEV_BINARY_PIN_H
#define DEV_BINARY_PIN_H

#include "utl/fundamental_types.h"

namespace dev {

class binary_pin
{
public:
    explicit binary_pin(u32 broadcom, bool on = false);
    ~binary_pin();

    bool state() const;
    void set_state(bool on);

private:
    u32 broadcom;
    bool value;
};

} // namespace dev

#endif
