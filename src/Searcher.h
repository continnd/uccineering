#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "AlphaBeta.h"
#include "DomineeringState.h"
#include "Evaluators.h"
#include "Location.h"
#include "Node.h"
#include "TranspositionTable.h"
#include "Timer.h"

#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_map>
#include <thread>

/**
 * A class that performs alpha-beta search in the game of Domineering to find
 * the best possible move for the current turn.
 */

class Searcher {
public:
    // Default constructor
    Searcher();

    /**
     * Instantiates a searcher and read the transposition table from the given
     * ifstream.
     *
     * \param[in] ifs the ifstream to read the transposition table from.
     */
    Searcher(std::ifstream& ifs);

    // Copy constructor
    Searcher(const Searcher& other);

    // Move constructor
    Searcher(Searcher&& other);

    // Destructor
    ~Searcher();

    // Assignment operator
    Searcher& operator=(const Searcher& other);

    Searcher& operator=(Searcher&& other);

    /**
     * Gives a starting point to this searcher.
     *
     * \param[in] root the root of the search tree.
     */
    void set_root(const Node& root);

    /**
     * Does housekeeping stuff such as clearing the transposition table.
     */
    void reset();

    /**
     * Searches for moves until it reaches the given depth.
     *
     * \param[in] state current state of the game configuration.
     *
     * \param[in] depth_limit the maximum depth to search.
     *
     * \return the node that represents the best move to make.
     */
    Node search(const DomineeringState& state, const unsigned depth_limit);

    /**
     * Searches under the given node.
     * This method populates the `best_moves' vector, so that the calling
     * method can look that up to find what the best move is for a certain
     * depth.
     *
     * \param[in] base the node to search under.
     *
     * \param[in] ab the alpha and beta values. Passed by value.
     *
     * \param[in] state current state of the game.
     *
     * \param[in] depth_limit the maximum depth to go down.
     */
    void search_under(const Node& base,
                      AlphaBeta ab,
                      const DomineeringState& state,
                      const unsigned depth_limit);

    /**
     * Given a state (i.e. the current board), this method evaluates and gives
     * a score to it.
     * TODO: pass in node also?
     *
     * \param[in] state the state to be evaluated.
     *
     * \return the score.
     */
    Evaluator::score_t evaluate(const DomineeringState& state);

    /**
     * Does cleanup before the program exits.
     * For example, it joins the threads that it spawned.
     */
    void cleanup();

    float get_time_left() const { return timer.get_time_left(); }

private:
    Timer timer;
    /**
     * The root of the search tree.
     */
    Node root;

    /**
     * A vector that contains the best moves for a certain depth.
     * For example, best_moves[1] contains the best move that can be executed
     * at depth 1.
     */
    std::vector<Node> best_moves;

    /**
     * A vectors of Nodes that will potentially be our next move.
     * The idea is to reorder the Nodes in each vector so that the Nodes that
     * can potentially maximize pruning will come towards the front of the
     * list.
     *
     * Key: the state. Compared against the actual move that our opponent has
     *      made, and use the appropriate set of children.
     * Val: the possible children nodes, ordered by preference.
     */
    std::unordered_map<DomineeringState, std::vector<Node>> ordered_moves;

    /**
     * Thread that is spawned to do move ordering during the opponents turn.
     */
    std::thread move_thread;

    /**
     * Transposition table that is used to find duplicates in board
     * configurations.
     */
    TranspositionTable tp_table;

    Who last_team = Who::HOME;

    /**
     * Expands the given node for the next possible placement.
     *
     * \param[in] base the node to expand.
     *
     * \param[in] current_state the state of the current game. Children are
     *                          expanded by placing the base's team's
     *                          dominoes onto current_state and checking if
     *                          that is a valid move or not.
     *
     * \return a vector of the expanded nodes. Note that the team of the nodes
     *         is the opposite of the base.
     */
    std::vector<Node> expand(const Node& base,
                             const DomineeringState& current_state);

    /**
     * Simulates the placing of a domino (i.e. move).
     * This is done by placing changing the grids on the board pointed by the
     * node to whatever team the node belongs to. Searcher::untap should be
     * called to undo this action.
     *
     * \param[in] node the node that modified the state.
     *
     * \param[out] state the state to be undone.
     */
    void tap(const Node& node, DomineeringState& state);

    /**
     * Rewinds the state to before tapping by clearing the place in the board
     * where the domino was placed.
     *
     * \param[in] node the node that modified the state.
     *
     * \param[out] state the state to be undone.
     */
    void untap(const Node& node, DomineeringState& state);
};

inline void Searcher::set_root(const Node& root) {
    this->root = root;
}

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
