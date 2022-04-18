#ifndef DEV_ESC_H
#define DEV_ESC_H

#include "utl/fundamental_types.h"

class esc
{
public:
    explicit esc(u32 broadcom, f64 speed = 0.0);
    ~esc();

    f64 speed() const;
    void set_speed(f64 speed);

private:
    u32 broadcom;
};

#endif
