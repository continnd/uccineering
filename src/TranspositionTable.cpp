#include "TranspositionTable.h"

using TPT = TranspositionTable;
using score_t = Evaluator::score_t;

const std::string TPT::TP_FILE_NAME = "tp_table.bin";

/* Constructors, Destructor, and Assignment operator {{{ */
// Default constructor
TPT::TranspositionTable()
{ }

// Copy constructor
TPT::TranspositionTable(const TPT& other)
    : table{other.table}
{ }

// Move constructor
TPT::TranspositionTable(TPT&& other)
    : table{std::move(other.table)}
{ }

// Destructor
TPT::~TranspositionTable()
{ }

// Assignment operator
TPT& TPT::operator=(const TPT& other) {
    table = other.table;
    return *this;
}

TPT& TPT::operator=(TPT&& other) {
    table = std::move(other.table);
    return *this;
}
/* }}} */

void TPT::populate(std::ifstream& ifs) {
    // TODO: read from file the initial transposition table to use
}

std::pair<score_t, bool> TPT::check(const DomineeringState& state) {
#if 0
    auto it = table.find(state);
    if (it != table.end()) {
        score_t score = it->second;
        return std::make_pair<score_t, bool>(std::move(score), true);
    }
    return std::make_pair<score_t, bool>(0, false);
#else
    DomineeringState state_copy{state};

    auto p = check_four_directions(state_copy);
    // Found?
    if (p.second) {
        return p;
    }

    // Now flip and do the same thing
    flip_vertical(state_copy);

    // Whatever we get is the result
    return check_four_directions(state_copy);
#endif
}

void TPT::insert(DomineeringState& state, const score_t score) {
    if (table.size() > TPT::TP_MAX && TPT::TP_MAX != 0) {
        // TODO: erase
    }
    else {
#if 1
        table[state] = score;
#else
        add_four_directions(state, score);
        flip_vertical(state);
        add_four_directions(state, score);
        // Back to the original position
        flip_vertical(state);
#endif
    }
}

/* Private methods */

std::pair<score_t, bool> TPT::check_four_directions(DomineeringState& state) {
    // Rotate and check
    for (unsigned i = 0; i < 4; i++) {
        auto it = table.find(state);
        if (it != table.end()) {
            score_t score = it->second;
            return std::make_pair<score_t, bool>(std::move(score), true);
        }
        rotate_cw(state);
    }

    // No match
    return std::make_pair<score_t, bool>(0, false);
}

void TPT::add_four_directions(DomineeringState& state, const score_t score) {
    // Rotate and check
    for (unsigned i = 0; i < 4; i++) {
        table[state] = score;
        rotate_cw(state);
    }
}

void TPT::flip_vertical(DomineeringState& state) {
    for (unsigned i = 0; i < state.ROWS; i++) {
        for (unsigned j = 0; j < state.COLS / 2; j++) {
            // Column to swap
            unsigned k = state.COLS - 1 - j;
            char a = state.getCell(i, j);
            char b = state.getCell(i, k);
            state.setCell(i, j, b);
            state.setCell(i, k, a);
        }
    }
}

void TPT::rotate_ccw(DomineeringState& state) {
    unsigned f = std::floor(state.ROWS / 2);
    unsigned c = std::ceil(state.ROWS / 2);
    for (unsigned i = 0; i < f; i++) {
        for (unsigned j = 0; j < c; j++) {
            unsigned k = state.COLS - 1 - i;
            unsigned l = state.COLS - 1 - j;
            char a = state.getCell(i, j);
            char b = state.getCell(j, k);
            char c = state.getCell(k, l);
            char d = state.getCell(l, i);
            state.setCell(i, j, b);
            state.setCell(j, k, c);
            state.setCell(k, l, d);
            state.setCell(l, i, a);
        }
    }
}

void TPT::rotate_cw(DomineeringState& state) {
    unsigned f = std::floor(state.ROWS / 2);
    unsigned c = std::ceil(state.ROWS / 2);
    for (unsigned i = 0; i < f; i++) {
        for (unsigned j = 0; j < c; j++) {
            unsigned k = state.COLS - 1 - i;
            unsigned l = state.COLS - 1 - j;
            char a = state.getCell(i, j);
            char b = state.getCell(j, k);
            char c = state.getCell(k, l);
            char d = state.getCell(l, i);
            state.setCell(i, j, d);
            state.setCell(j, k, a);
            state.setCell(k, l, b);
            state.setCell(l, i, c);
        }
    }
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
