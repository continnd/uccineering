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

DomineeringMove Moderator::next_move(const DomineeringState& state) {
    // Set the starting node
    searcher.set_root(Node(state.getWho(), 0));

    Node next_move = searcher.search(state, get_search_depth(state));
    return next_move.location.to_move();
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
    // TODO: Variable depth
    return 4;
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
