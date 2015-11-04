#include "Node.h"

/* Constructors, destructor, and assignment operator {{{ */
Node::Node()
    : score_{0}
    , is_unset{true}
    , is_terminal_{true}
{
}

Node::Node(Who team, unsigned depth)
    : team{team}
    , depth{depth}
    , score_{0}
    , is_unset{true}
    , is_terminal_{false}
{
}

Node::Node(Who team, unsigned depth, Location parent_move)
    : team{team}
    , depth{depth}
    , score_{0}
    , parent_move{parent_move}
    , is_unset{true}
    , is_terminal_{false}
{
}

Node::Node(const Node& other)
    : team{other.team}
    , depth{other.depth}
    , score_{other.score_}
    , parent_move{other.parent_move}
    , is_unset{other.is_unset}
    , is_terminal_{other.is_terminal_}
{
}

Node::Node(Node&& other)
    : team{other.team}
    , depth{other.depth}
    , score_{other.score_}
    , parent_move{std::move(other.parent_move)}
    , is_unset{other.is_unset}
    , is_terminal_{other.is_terminal_}
{
}

Node::~Node() {
}

Node& Node::operator=(const Node& other) {
    team = other.team;
    depth = other.depth;
    score_ = other.score_;
    parent_move = other.parent_move;
    is_unset = other.is_unset;
    is_terminal_ = other.is_terminal_;

    return *this;
}
/* }}} */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
