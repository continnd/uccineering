#ifndef NODE_H_
#define NODE_H_

#include "AlphaBeta.h"
#include "Evaluators.h"
#include "GameState.h"
#include "Location.h"

#include <algorithm>
#include <istream>

/**
 * A class that represents a node in the search tree.
 */

struct Node {
    // Default constructor
    Node();

    Node(Who team, unsigned depth);

    Node(Who team, unsigned depth, Location parent_move);

    // Copy constructor
    Node(const Node& other);

    // Move constructor
    Node(Node&& other);

    // Destructor
    ~Node();

    // Assignment operator
    Node& operator=(const Node& other);

    /**
     * Compares the scores with another Node.
     *
     * \param[in] other Node to be compared against
     *
     * \return true if other node is more favorable, false otherwise
     */
    bool operator<(const Node& other) const;

    /**
     * Compares the scores with another Node.
     *
     * \param[in] other Node to be compared against
     *
     * \return true if this node is more favorable, false otherwise
     */
    bool operator>(const Node& other) const;

    /**
     * Checks for equality.
     *
     * \param[in] other the node to compare.
     *
     * \return true if the other node is equal to this node, false otherwise.
     */
    bool operator==(const Node& other) const;

    /**
     * Checks for inequality.
     *
     * \param[in] other the node to compare.
     *
     * \return true if the other node is not equal to this node, false
     *         otherwise.
     */
    bool operator!=(const Node& other) const;

    /**
     * \return the score of this node.
     */
    Evaluator::score_t score() const;

    /**
     * Setter for the score.
     * This method also sets the is_unset parameter to false.
     *
     * \param[in] score the new score.
     */
    void set_score(const Evaluator::score_t score);

    /**
     * \return true if this node is a terminal node, false otherwise.
     */
    bool is_terminal() const;

    /**
     * Changes the node to a terminal node.
     * The score is updated depending on the status of the game (POS_INF if
     * status is HOME_WIN, NEG_INF if AWAY_WIN).
     *
     * \param[in] state current state of the board. Passed in by value because
     *                  the `DomineeringState::status' member variable needs
     *                  to be updated first (due to how the library is
     *                  currently implemented) in order for us to be able to
     *                  get the status of the game.
     */
    void set_as_terminal(DomineeringState state);

    void update_ab(AlphaBeta* const ab) const;


    /* Team of this node. Min or Max. */
    Who team;
    /* How deep it is in the search tree */
    unsigned depth;
    Location parent_move;
    /* True if node score is unset */
    bool is_unset;

private:
    Evaluator::score_t score_;
    /* True if node is a terminal node */
    bool is_terminal_;
};

inline Evaluator::score_t Node::score() const {
    return score_;
}

inline void Node::set_score(const Evaluator::score_t score) {
    this->score_ = score;
    is_unset = false;
}

inline bool Node::is_terminal() const {
    return is_terminal_;
}

inline void Node::set_as_terminal(DomineeringState state) {
    is_terminal_ = true;

    // Update the DomineeringState::state variable
    state.checkTerminalUpdateStatus();
    // Now get the updated status
    auto status = state.getStatus();
    if (status == Status::HOME_WIN) {
        set_score(AlphaBeta::POS_INF);
    }
    else if (status == Status::AWAY_WIN) {
        set_score(AlphaBeta::NEG_INF);
    }
}

inline bool Node::operator<(const Node& other) const {
    return score_ < other.score_;
}

inline bool Node::operator>(const Node& other) const {
    return score_ > other.score_;
}

inline bool Node::operator==(const Node& other) const {
    return team == other.team && parent_move == other.parent_move
        && depth == other.depth;
}

inline bool Node::operator!=(const Node& other) const {
    return team != other.team || parent_move != other.parent_move
        || depth != other.depth;
}

namespace std {
    template<>
    struct hash<Node> {
        size_t operator()(const Node& n) const {
            size_t t{std::hash<int>()(static_cast<int>(n.team))};
            size_t l{std::hash<Location>()(n.parent_move)};
            size_t d{std::hash<unsigned>()(n.depth)};

            return (((t << 1) ^ l) >> 1) ^ d;
        }
    };
} // namespace std

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
