#include <sys/time.h>
#include <cstddef>
#include <vector>

class Timer {
    timeval t_dummy;
    double t_start;
    double t_end;
    double diff;
    double lapse;
    std::vector<std::vector<float>> predicted_times;

    public:
    Timer() :
        t_start(-1),
        t_end(-1),
        diff(-1),
        lapse(-1)
    {
    }
    Timer(std::vector<std::vector<float>> pt) :
        t_start(-1),
        t_end(-1),
        diff(-1),
        lapse(-1)
    {
        predicted_times = std::move(pt);
    }
    ~Timer() {}
    void click();
    float get_time();
    float get_lapse_time();
    int get_suggested_depth(int b, float time_allotted);
};
