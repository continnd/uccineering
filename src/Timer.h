#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <cstddef>
#include <vector>

class Timer {
    timeval t_dummy;
    double t_start;
    double t_end;
    double diff;
    double time_left;
    int moves_left;
    std::vector<std::vector<float>> predicted_times;

    public:
    Timer() :
        t_start(-1),
        t_end(-1),
        diff(-1),
        time_left(-1),
        moves_left(14)
    {
    }
    Timer(float tl) :
        t_start(-1),
        t_end(-1),
        diff(-1),
        time_left(tl),
        moves_left(14)
    {
    }
    Timer(float tl, std::vector<std::vector<float>> pt) :
        t_start(-1),
        t_end(-1),
        diff(-1),
        time_left(tl),
        moves_left(14)
    {
        predicted_times = std::move(pt);
    }
    ~Timer() {}
    void click();
    float get_time();
    float get_time_left();
    int get_moves_left();
    float get_move_time();
    int get_suggested_depth(int b);
};

#endif
