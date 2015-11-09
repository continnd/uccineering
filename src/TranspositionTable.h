#ifndef TRANSPOSITION_TABLE_H_
#define TRANSPOSITION_TABLE_H_

#include "DomineeringState.h"
#include "Evaluators.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using score_t = Evaluator::score_t;

class TranspositionTable {
public:
    /**
     * A simple struct that represents an entry in the transposition table.
     * The `nodes_searched' member variable is used when deleting old elements
     * when the table gets too large.
     */
    struct Entry {
        Entry();
        Entry(const score_t lower_limit,
              const score_t upper_limit,
              const long unsigned nodes_searched);
        Entry(const Entry& other);
        Entry(Entry&& other);

        Entry& operator=(const Entry& other);
        Entry& operator=(Entry&& other);

        score_t lower_limit, upper_limit;
        long unsigned nodes_searched;
    };

    // Declare type of table here so that key-value size can be calculated
    using table_t = std::unordered_map<DomineeringState, Entry>;

    /**
     * Maximum memory we should use in megabytes.
     */
    static const unsigned MEM_LIMIT = 500;

    static const unsigned BYTES_PER_MEGABYTE = 1024 * 1024;

    /**
     * The amount of memory it takes for one entry in bytes.
     * Estimated by adding the size of hash key and value.
     */
    static constexpr size_t BYTES_PER_ENTRY =
        sizeof(typename table_t::value_type);

    /**
     * Maximum number of entries to store in the transposition table.
     * 0 for unlimited.
     */
    static constexpr unsigned TP_MAX =
        MEM_LIMIT * BYTES_PER_MEGABYTE / BYTES_PER_ENTRY;

    /**
     * How much entries to get rid of in one shrink.
     */
    static constexpr float SHRINK_RATE = 0.8;

    TranspositionTable();

    TranspositionTable(const TranspositionTable& other);

    TranspositionTable(TranspositionTable&& other);

    ~TranspositionTable();

    TranspositionTable& operator=(const TranspositionTable& other);

    TranspositionTable& operator=(TranspositionTable&& other);

    /**
     * Clears the transposition table.
     */
    void clear();

    /**
     * Shrinks the transposition table by removing entries that have smaller
     * searched nodes count.
     */
    void shrink();

    /**
     * Checks for existence in the transposition table.
     * If the given state is not a hit, this method will try to find the same
     * transposition of the state (vertically and horizontally symmetrical).
     *
     * \param[in] state the state to check
     *
     * \return a std::pair<score_t, bool> where the first element is the score
     *         and the second element is true if there was a hit in any of the
     *         transposition, false otherwise.
     */
    std::pair<Entry, bool> check(const DomineeringState& state);

    /**
     * Adds the current state and the resulting score to the transposition
     * table.
     * If the number of entries in the table exceeds a threashold, some
     * entries in the transposition table are deleted.
     *
     * \param[in] state the current state.
     *
     * \param[in] lower_limit the lowest possible score that is guarenteed to
     *                        be found when further searching down the tree.
     *
     * \param[in] upper_limit the highest possible score that is guarenteed to
     *                        be found when further searching down the tree.
     *
     * \param[in] nodes_searched the number of nodes searched up to the point
     *            of insertion. This is used when the table gets too large and
     *            needs to be shrunk.
     */
    void insert(const DomineeringState& state,
                const score_t lower_limit,
                const score_t upper_limit,
                const long unsigned nodes_searched);

private:
    /**
     * The transposition table.
     * This table is used to look up board configurations that have already
     * been explored.
     *
     * Key: the state.
     * Val: a pair of resulting score and the move number this entry was added.
     */
    table_t table;

    /**
     * Flips the board horizontally (along the x-axis).
     *
     * \param[out] state the state to be flipped.
     */
    void flip_horizontal(DomineeringState& state);

    /**
     * Flips the board vertically (along the y-axis).
     *
     * \param[out] state the state to be flipped.
     */
    void flip_vertical(DomineeringState& state);

    /**
     * Rotates the board counter-clockwise 90 degrees.
     *
     * \param[out] state the state to be rotated.
     */
    void rotate_ccw(DomineeringState& state);

    /**
     * Rotates the board clockwise 90 degrees.
     *
     * \param[out] state the state to be rotated.
     */
    void rotate_cw(DomineeringState& state);
};

inline void TranspositionTable::clear() {
    table.clear();
}

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
