#include <time.h>
#include <vector>

class Timer {
    time_t t_start;
    time_t t_end;
    time_t diff;
    std::vector<std::vector<float>> predicted_times;

    public:
    Timer() : t_start(-1), t_end(-1), diff(-1) {}
    Timer(std::vector<std::vector<float>> pt) : t_start(-1), t_end(-1),
    diff(-1), predicted_times(pt) {}
    ~Timer();
    click();
    get_time();
    get_suggested_depth(int b, float time_alloted);
};
