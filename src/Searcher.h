#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "AlphaBeta.h"
#include "DomineeringState.h"
#include "Evaluators.h"
#include "Location.h"
#include "Node.h"

#include <algorithm>
#include <vector>
#include <queue>

/**
 * A class that performs alpha-beta search in the game of Domineering to find
 * the best possible move for the current turn.
 */

template<typename T> using branch_queue = std::priority_queue<Node, std::vector<Node>, T>;

class Searcher {
public:
    // Default constructor
    Searcher();

    // Copy constructor
    Searcher(const Searcher& other);

    // Move constructor
    Searcher(const Searcher&& other);

    // Destructor
    ~Searcher();

    // Assignment operator
    Searcher& operator=(const Searcher& other);

    /**
     * Gives a starting point to this searcher.
     *
     * \param[in] root the root of the search tree.
     */
    void set_root(const Node& root);

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
     * Searches under the given node and returns the best move for that
     * branch.
     * This method populates the `best_moves' vector, so that the calling
     * method can look that up to find what the best move is for a certain
     * depth.
     *
     * \param[in] parent the node to search under.
     *
     * \param[in] ab the alpha and beta values. Passed by value.
     *
     * \param[in] state current state of the game.
     *
     * \param[in] depth_limit the maximum depth to go down.
     *
     * \return the score that results from the best move.
     */
    double search_under(const Node& parent, AlphaBeta ab,
            const DomineeringState& state, const unsigned depth_limit);

    /**
     * Given a state (i.e. the current board), this method evaluates and gives
     * a score to it.
     * TODO: pass in node also?
     *
     * \param[in] state the state to be evaluated.
     *
     * \return the score.
     */
    double evaluate(const DomineeringState& state);

private:
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

    std::unordered_map<size_t, std::vector<Node>> ordered_moves;

    /**
     * Expands the given node for the next possible placement.
     *
     * \param[in] parent the node to expand.
     *
     * \param[in] current_state the state of the current game.
     *
     * \return a vector of the expanded nodes.
     */
    std::vector<Node> expand(const Node& parent,
            const DomineeringState& current_state);

    /**
     * Does move ordering to the given vector of nodes.
     * Modifies the vector given as the argument. This method orders the
     * elements in such a way that the most preferred move comes to the
     * beginning of the vector.
     *
     * \param[out] nodes the nodes to be reordered for optimal search.
     */
    void move_order();

    /**
     * Checks if further children nodes can be pruned or not.
     *
     * \param[in] node the node to be examined. If pruning is valid, all of
     *                 its children are not searched.
     *
     * \param[in] ab the current alpha-beta values for this node.
     *
     * \return true if children nodes can be pruned, false otherwise.
     */
    bool can_prune(const Node& node, const AlphaBeta& ab);

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

/**
 * Combine operation of two hash keys. Based on boost::hash_combine.
 */
namespace {
    void hash_combine(std::size_t& h, const std::size_t& v) {
        h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
}

/**
 * Hash function for DomineeringState.
 * Yes, it is so much more appropriate for this function to reside in
 * common/DomineeringState.h but since this file is part of the distributed code
 * base, I did not want to mess around with it.
 */
namespace std {
    template<>
    struct hash<DomineeringState> {
        size_t operator()(const DomineeringState& ds) const {
            size_t h = 0;
            for (auto&& c : *ds.getBoard1D()) {
                if (c == ds.EMPTYSYM) {
                    hash_combine(h, std::hash<char>()(c));
                }
            }
            return h;
        }
    };
} // namespace std

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
