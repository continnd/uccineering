#include "Timer.h"

void Timer::click() {
    if (t_start == -1 || (t_start != -1 && t_end != -1)) {
        gettimeofday(&t_dummy, NULL);
        t_start = t_dummy.tv_sec + (t_dummy.tv_usec / 1000000.0);
        t_end = -1;
        diff = -1;
    } else {
        gettimeofday(&t_dummy, NULL);
        t_end = t_dummy.tv_sec + (t_dummy.tv_usec / 1000000.0);
        diff = t_end - t_start;
        time_left -= diff;
        moves_left--;
    }
}

float Timer::get_time() {
    return diff;
}

float Timer::get_time_left() {
    return time_left;
}

int Timer::get_moves_left() {
    return moves_left;
}

float Timer::get_move_time() {
    return time_left / moves_left;
}

int Timer::get_suggested_depth(const int b) {
    int i;
    for (i = 0; i < predicted_times[b].size(); i++) {
        if (predicted_times[b][i] >= get_move_time()) {
            break;
        }
    }
    return i < predicted_times[b].size() ? i - 1 : predicted_times[b].back();
}
