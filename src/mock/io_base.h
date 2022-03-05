#ifndef IO_BASE_H
#define IO_BASE_H

struct io_base
{
public:
    float current_left_speed;
    float current_right_speed;
    bool current_eyes_state;
    bool current_fan_state;
    bool current_heartbeat_state;
};

#endif