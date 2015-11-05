#ifndef TRANSPOSITION_TABLE_H_
#define TRANSPOSITION_TABLE_H_

#include "DomineeringState.h"
#include "Evaluators.h"

#include <cmath>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

using score_t = Evaluator::score_t;

class TranspositionTable {
public:
    /* File that contains the initial transposition table */
    static const std::string TP_FILE_NAME;

    /**
     * Maximum number of entries to store in the transposition table.
     * 0 for unlimited.
     */
    static const unsigned TP_MAX = 0;

    TranspositionTable();

    TranspositionTable(const TranspositionTable& other);

    TranspositionTable(TranspositionTable&& other);

    ~TranspositionTable();

    TranspositionTable& operator=(const TranspositionTable& other);

    TranspositionTable& operator=(TranspositionTable&& other);

    /**
     * Populates the transposition table from the given file stream.
     *
     * \param[in] ifs the input file stream to read from.
     */
    void populate(std::ifstream& ifs);

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
    std::pair<score_t, bool> check(const DomineeringState& state);

    /**
     * Adds the current state and the resulting score to the transposition
     * table.
     * If the number of entries in the table exceeds a threashold, some
     * entries in the transposition table are deleted.
     * This method modifies the state, but modifies in such a way that the
     * board configuration is the same at the beginning and the end of the
     * method.
     *
     * \param[in] state the current state.
     *
     * \param[in] score the best score that results from the given state.
     */
    void insert(DomineeringState& state, const score_t score);

private:
    /**
     * The transposition table.
     * This table is used to look up board configurations that have already
     * been explored.
     *
     * Key: the state.
     * Val: the resulting score.
     *
     * TODO: score or Node for the value?
     */
    std::unordered_map<DomineeringState, score_t> table;

    /**
     * Checks if state is in the transposition table for all four direction by
     * rotating.
     */
    std::pair<score_t, bool> check_four_directions(DomineeringState& state);

    /**
     * Rotates the board and adds them to the transposition table.
     */
    void add_four_directions(DomineeringState& state, const score_t score);

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

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
