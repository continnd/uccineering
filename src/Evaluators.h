#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>
#include <vector>
// TODO: Better evaluation
#include <random>

static std::random_device rd;
static std::uniform_real_distribution<double> u_dist;

using DS = DomineeringState;

struct EvalTakeAway {
    double operator()(const DS& state) const {
        return u_dist(rd);
    }
};

struct EvalReserve {
    double operator()(const DS& state) const {
        return u_dist(rd);
    }
};

using EvalScore = std::function<double(const DS&)>;
using EvalFactor = std::function<double(const DS&)>;
// vector of pairs because std::map requires operator< and std::unordered_map
// requires operator== and hash function for DomineeringState
static const std::vector<std::pair<EvalScore, EvalFactor>> evaluators = {
    { std::make_pair(EvalTakeAway(), [](const DS& state) { return 1; }) },
    { std::make_pair(EvalReserve(), [](const DS& state) { return 1; }) },
};

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
