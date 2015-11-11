#include "Searcher.h"
#include <iostream>

/* Constructors, destructor, and assignment operator {{{ */
Searcher::Searcher() {
    move_thread = std::thread(&Searcher::move_order, this, Who::HOME);
}

Searcher::Searcher(std::ifstream& ifs) {
    move_thread = std::thread(&Searcher::move_order, this, Who::HOME);
}

Searcher::Searcher(const Searcher& other)
    : root{other.root}
    , best_moves{other.best_moves}
    , ordered_moves{other.ordered_moves}
    , tp_table{other.tp_table}
    , timer{other.timer}
{
    move_thread = std::thread(&Searcher::move_order, this, Who::HOME);
}

Searcher::Searcher(Searcher&& other)
    : root{std::move(other.root)}
    , best_moves{std::move(other.best_moves)}
    , ordered_moves{std::move(other.ordered_moves)}
    , tp_table{std::move(other.tp_table)}
    , timer{std::move(other.timer)}
{
    move_thread = std::thread(&Searcher::move_order, this, Who::HOME);
}

Searcher::~Searcher() {
}

Searcher& Searcher::operator=(const Searcher& other) {
    root = other.root;
    best_moves = other.best_moves;
    ordered_moves = other.ordered_moves;
    tp_table = other.tp_table;
    timer = other.timer;

    return *this;
}

Searcher& Searcher::operator=(Searcher&& other) {
    root = std::move(other.root);
    best_moves = std::move(other.best_moves);
    ordered_moves = std::move(other.ordered_moves);
    tp_table = std::move(other.tp_table);
    timer = std::move(other.timer);

    return *this;
}
/* }}} */

void Searcher::reset() {
    tp_table.clear();
}

Node Searcher::search(const DomineeringState& state,
        const unsigned depth_limit) {
    if (root.team != last_team) {
        last_team = root.team;
        timer = Timer(240);
    }

    timer.click();

    if (move_thread.joinable()) {
        move_thread.join();
    // }
    // Initialize best moves
    best_moves.resize(depth_limit + 1);
    std::fill(best_moves.begin(), best_moves.end(), Node());

    AlphaBeta ab(AlphaBeta::NEG_INF, AlphaBeta::POS_INF);
    // Remove all the useless information currently stored in the table
    tp_table.clear();
    search_under(root, ab, state, depth_limit);

    timer.click();

    return best_moves.front();
}

void Searcher::search_under(const Node& base,
                            AlphaBeta ab,
                            const DomineeringState& current_state,
                            const unsigned depth_limit) {
    Node& current_best = best_moves[base.depth];

    // Base case
    if (base.depth >= depth_limit) {
        current_best = base;
        current_best.set_score(evaluate(current_state));
        current_best.lower_limit = current_best.score();
        current_best.upper_limit = current_best.score();
        return;
    }

    // Check for transpositions that were already explored
    bool found;
    TranspositionTable::Entry entry;
    std::tie(entry, found) = tp_table.check(current_state);
    if (found && ab.can_prune(entry, base.team)) {
        // Set score to the best value possible in our sub tree so that we get
        // chosen by the parent, but that won't happen because there already
        // is a better value somewhere in another sub tree.
        current_best.set_score(base.team == Who::HOME
                               ? entry.lower_limit
                               : entry.upper_limit);
        current_best.lower_limit = entry.lower_limit;
        current_best.upper_limit = entry.upper_limit;
        return;
    }

    std::vector<Node> children;
    auto ordered = ordered_moves.find(current_state);
    // Use move-ordered children if possible
    if (base.depth == 0 && ordered != ordered_moves.end()) {
        children = ordered->second;
        ordered_moves.clear();
    }
    else {
        children = expand(base, current_state);
    }

    // `base' is a terminal node
    if (children.empty()) {
        current_best = base;
        current_best.set_as_terminal(current_state);
        return;
    }

    /*
     * Reset the score to POS_INF or NEG_INF depending on which team this node
     * belongs to.
     */
    current_best.set_score(base.team == Who::HOME
            ? AlphaBeta::NEG_INF
            : AlphaBeta::POS_INF);

    DomineeringState next_state{current_state};
    next_state.togglePlayer();

    for (Node& child : children) {
        // Update board to simulate placing the child.
        // Done so that we don't need to make a copy of state for each child.
        tap(child, next_state);

        // Recursive call
        search_under(child, ab, next_state, depth_limit);

        // Rewind to board before placing the child
        untap(child, next_state);

        const Node& next_move{best_moves[base.depth + 1]};

        if (next_move.is_terminal()) {
            child.set_as_terminal(next_state);
        }
        else {
            child.set_score(next_move.score());
        }

        current_best.update_limits(next_move);
        current_best.descentdants_searched += next_move.descentdants_searched;

        bool result_better = base.team == Who::HOME
            ? child.score() > current_best.score()
            : child.score() < current_best.score();
        if (result_better || current_best.is_unset) {
            current_best = child;

            ab.update_if_needed(child.score(), base.team);
            if (ab.can_prune(child.score(), base.team)) {
                // Add result to transposition table
                tp_table.insert(current_state,
                        current_best.lower_limit,
                        current_best.upper_limit,
                        current_best.descentdants_searched);
                return;
            }
        }
    }

    // Set true score since there was no pruning
    current_best.lower_limit = current_best.score();
    current_best.upper_limit = current_best.score();
    // Add result to transposition table
    tp_table.insert(current_state,
            current_best.lower_limit,
            current_best.upper_limit,
            current_best.descentdants_searched);

    return;
}

Evaluator::score_t Searcher::evaluate(const DomineeringState& state) {
    // A copy of the state so that we can mark places temporarily and pass
    // that around to various evaluators
    DomineeringState state_copy{state};
    using score_t = Evaluator::score_t;

    score_t home_score = RESERVED_FACTOR * home_reserved(&state_copy)
        + OPEN_FACTOR * home_open(&state_copy);

    clear_marks(&state_copy);

    score_t away_score = RESERVED_FACTOR * away_reserved(&state_copy)
        + OPEN_FACTOR * away_open(&state_copy);

    return home_score - away_score;
}

void Searcher::cleanup() {
    if (move_thread.joinable()) {
        move_thread.join();
    }
}

/* Private methods */

std::vector<Node> Searcher::expand(const Node& base,
        const DomineeringState& current_state) {
    // Toggle player
    Who child_team = base.team == Who::HOME ? Who::AWAY : Who::HOME;
    unsigned child_depth = base.depth + 1;
    std::vector<Node> children;

    // Create one instance and modify the values so that we don't have to
    // instantiate a ton of vectors in GameMove constructor.
    DomineeringMove parent_move{0, 0, 0, 0};
    for (unsigned r1 = 0; r1 < current_state.ROWS; r1++) {
        for (unsigned c1 = 0; c1 < current_state.COLS; c1++) {
            // Home places horizontally, Away places vertically
            unsigned r2 = base.team == Who::HOME ? r1 : r1 + 1;
            unsigned c2 = base.team == Who::HOME ? c1 + 1 : c1;

            parent_move.setMv(r1, c1, r2, c2);

            if (current_state.moveOK(parent_move)) {
                // Note: my_move is HOW I got to this state i.e. base's move
                children.push_back(Node(child_team,
                            child_depth,
                            Location(parent_move)));
            }
        }
    }
    return children;
}

void Searcher::move_order(Who team) {
    for (auto& p : ordered_moves) {
        auto& moves = p.second;
        if (team == Who::HOME) {
            std::sort(moves.begin(), moves.end(), std::greater<Node>());
        }
        else {
            std::sort(moves.begin(), moves.end(), std::less<Node>());
        }

        for (Node s : moves) std::cout << s.score();
        std::cout << std::endl;
    }
}

void Searcher::tap(const Node& node, DomineeringState& state) {
    // To reflect the base's team, flip the symbols
    char c = node.team == Who::HOME ? state.AWAYSYM : state.HOMESYM;
    state.setCell(node.parent_move.r1, node.parent_move.c1, c);
    state.setCell(node.parent_move.r2, node.parent_move.c2, c);
}

void Searcher::untap(const Node& node, DomineeringState& state) {
    char c = state.EMPTYSYM;
    state.setCell(node.parent_move.r1, node.parent_move.c1, c);
    state.setCell(node.parent_move.r2, node.parent_move.c2, c);
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
