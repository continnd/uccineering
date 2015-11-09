#include "TranspositionTable.h"

using TPT = TranspositionTable;
using score_t = Evaluator::score_t;

/* Constructors for TranspositionTable::Entry {{{ */
TPT::Entry::Entry()
    : lower_limit{0}
    , upper_limit{0}
    , nodes_searched{0}
{ }

TPT::Entry::Entry(const score_t lower_limit,
                  const score_t upper_limit,
                  const long unsigned nodes_searched)
    : lower_limit{lower_limit}
    , upper_limit{upper_limit}
    , nodes_searched{nodes_searched}
{ }

TPT::Entry::Entry(const TPT::Entry& other)
    : lower_limit{other.lower_limit}
    , upper_limit{other.upper_limit}
    , nodes_searched{other.nodes_searched}
{ }

TPT::Entry::Entry(TPT::Entry&& other)
    : lower_limit{std::move(other.lower_limit)}
    , upper_limit{std::move(other.upper_limit)}
    , nodes_searched{std::move(other.nodes_searched)}
{ }

TPT::Entry& TPT::Entry::operator=(const Entry& other) {
    lower_limit = other.lower_limit;
    upper_limit = other.upper_limit;
    nodes_searched = other.nodes_searched;
    return *this;
}

TPT::Entry& TPT::Entry::operator=(Entry&& other) {
    lower_limit = std::move(other.lower_limit);
    upper_limit = std::move(other.upper_limit);
    nodes_searched = std::move(other.nodes_searched);
    return *this;
}
/* }}} */

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

std::pair<TPT::Entry, bool> TPT::check(const DomineeringState& state) {
    DomineeringState state_copy{state};

    // Original position
    auto it = table.find(state);
    if (it != table.end()) {
        return std::make_pair(it->second, true);
    }

    flip_horizontal(state_copy);

    // Horizontal
    it = table.find(state);
    if (it != table.end()) {
        return std::make_pair(it->second, true);
    }

    // Horizontal AND vertical
    flip_vertical(state_copy);
    it = table.find(state);
    if (it != table.end()) {
        return std::make_pair(it->second, true);
    }

    // Vertical
    // Revert the horizontal flip
    flip_horizontal(state_copy);
    it = table.find(state);
    if (it != table.end()) {
        return std::make_pair(it->second, true);
    }

    // No match
    return std::make_pair(Entry(), false);
}

void TPT::shrink() {
    // Key-Value type
    using k_v_t = std::pair<
        decltype(table)::key_type,
        decltype(table)::mapped_type
    >;
    // Move all entries into a vector and sort
    std::vector<k_v_t> tmp;
    tmp.resize(table.size());
    std::move(table.begin(), table.end(), tmp.begin());
    // Sort so that the largest nodes_searched comes first
    std::sort(tmp.begin(), tmp.end(), [](const k_v_t& a, const k_v_t& b) {
              return a.second.nodes_searched > b.second.nodes_searched;
              });
    // Shrink and destroy entries
    tmp.resize(TP_MAX * SHRINK_RATE);
    // Them move them back to the table
    for (auto&& p : std::move(tmp)) {
        table.insert(p);
    }
}

void TPT::insert(const DomineeringState& state,
                 const score_t lower_limit,
                 const score_t upper_limit,
                 const long unsigned nodes_searched) {
    // Check table size first
    if (table.size() > TPT::TP_MAX && TPT::TP_MAX != 0) {
        shrink();
    }
    else {
        table[state] = Entry(lower_limit, upper_limit, nodes_searched);
    }
}

/* Private methods */

void TPT::flip_horizontal(DomineeringState& state) {
    for (unsigned i = 0; i < state.ROWS; i++) {
        for (unsigned j = 0; j < state.COLS / 2; j++) {
            // Row to swap
            unsigned k = state.ROWS - 1 - i;
            char a = state.getCell(i, j);
            char b = state.getCell(k, j);
            state.setCell(i, j, b);
            state.setCell(k, j, a);
        }
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
