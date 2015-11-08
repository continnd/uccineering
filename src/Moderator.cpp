#include "Moderator.h"

/* Constructors, destructor, and assignment operator {{{ */
Moderator::Moderator()
    : GamePlayer("anonymous", GAME_NAME)
{
}

Moderator::Moderator(const std::string& team_name)
    : team_name{team_name}
    , GamePlayer(team_name, GAME_NAME)
{
}

Moderator::Moderator(const Moderator& other)
    : team_name{other.team_name}
    , searcher{other.searcher}
    , GamePlayer(other.team_name, GAME_NAME)
{
}

Moderator::Moderator(Moderator&& other)
    : team_name{std::move(other.team_name)}
    , searcher{std::move(other.searcher)}
    , GamePlayer(other.team_name, GAME_NAME)
{
}

Moderator::~Moderator() {
}

Moderator& Moderator::operator=(const Moderator& other) {
    team_name = other.team_name;
    searcher = other.searcher;

    return *this;
}
/* }}} */

void Moderator::init() {
    // Do nothing
}

void Moderator::done() {
    searcher.cleanup();
}

DomineeringMove Moderator::next_move(const DomineeringState& state) {
    // Set the starting node
    searcher.set_root(Node(state.getWho(), 0));

    Node best_child = searcher.search(state, get_search_depth(state));
    return best_child.parent_move.to_move();
}

Node Moderator::spawn_searcher(const Node& nodes) {
    // TODO: Implement me!
    return Node();
}

GameMove* Moderator::getMove(GameState& state, const std::string& last_move) {
    const auto&& m = next_move(static_cast<DomineeringState&>(state));
    return new DomineeringMove(m);
}

/* Private methods */

unsigned Moderator::get_search_depth(const DomineeringState& state) const {
    unsigned game_moves = state.getNumMoves();
    unsigned depth;

    // The first four moves are not worth searching deep
    if (game_moves <= 4) {
        depth = 4;
    }
    else if (game_moves > 13) {
        depth = 8;
    }
    else {
        depth = 6;
    }
    return depth;
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
