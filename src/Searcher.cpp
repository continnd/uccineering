#include "Searcher.h"

/* Constructors, destructor, and assignment operator {{{ */
Searcher::Searcher() {
}

Searcher::Searcher(std::ifstream& ifs) {
    tp_table.populate(ifs);
}

Searcher::Searcher(const Searcher& other)
    : root{other.root}
    , best_moves{other.best_moves}
    , ordered_moves{other.ordered_moves}
    , tp_table{other.tp_table}
{
}

Searcher::Searcher(Searcher&& other)
    : root{std::move(other.root)}
    , best_moves{std::move(other.best_moves)}
    , ordered_moves{std::move(other.ordered_moves)}
    , tp_table{std::move(other.tp_table)}
{
}

Searcher::~Searcher() {
}

Searcher& Searcher::operator=(const Searcher& other) {
    root = other.root;
    best_moves = other.best_moves;
    ordered_moves = other.ordered_moves;
    tp_table = other.tp_table;

    return *this;
}

Searcher& Searcher::operator=(Searcher&& other) {
    root = std::move(other.root);
    best_moves = std::move(other.best_moves);
    ordered_moves = std::move(other.ordered_moves);
    tp_table = std::move(other.tp_table);

    return *this;
}
/* }}} */

Node Searcher::search(const DomineeringState& state,
        const unsigned depth_limit) {
    // Initialize best moves
    best_moves.resize(depth_limit + 1);
    std::fill(best_moves.begin(), best_moves.end(), Node());

    AlphaBeta ab(AlphaBeta::NEG_INF, AlphaBeta::POS_INF);
    search_under(root, ab, state, depth_limit);

    return best_moves.front();
}

Evaluator::score_t Searcher::search_under(const Node& parent, AlphaBeta ab,
        const DomineeringState& current_state, const unsigned depth_limit) {
    // Base case
    if (parent.depth >= depth_limit) {
        return evaluate(current_state);
    }

    std::vector<Node> children;
    auto ordered = ordered_moves.find(current_state);
    // Use move-ordered children if possible
    if (parent.depth == 0 && ordered != ordered_moves.end()) {
        children = ordered->second;
    }
    else {
        children = expand(parent, current_state);
    }

    Node& current_best = best_moves[parent.depth];

    // `parent' is a terminal node
    if (children.empty()) {
        current_best.set_as_terminal();
        // POS_INF or NEG_INF
        return current_best.score();
    }

    DomineeringState next_state(current_state);
    /*
     * Reset the score to POS_INF or NEG_INF depending on which team this node
     * belongs to.
     */
    current_best.set_score(parent.team == Who::HOME
                           ? AlphaBeta::NEG_INF
                           : AlphaBeta::POS_INF);

    next_state.togglePlayer();

    // create a branch queue vector for every possible future root node
    for (const Node& child : children) {
        // Update board to simulate placing the child.
        // Done so that we don't need to make a copy of state for each child.
        tap(child, next_state);

        Evaluator::score_t result;
        bool found;
        // Check for transpositions that were already explored
        std::tie(result, found) = tp_table.check(next_state);
        if (!found) {
            // Recursive call
            result = search_under(child, ab, next_state, depth_limit);
            // Add result to transposition table
            tp_table.insert(next_state, result);
        }

        // Rewind to board before placing the child
        untap(child, next_state);

        // Found path to a terminal state
        if (result == AlphaBeta::POS_INF || result == AlphaBeta::NEG_INF) {
            current_best = std::move(child);
            current_best.set_as_terminal();
            // POS_INF or NEG_INF
            return current_best.score();
        }

        bool result_better = parent.team == Who::HOME
            ? result > current_best.score()
            : result < current_best.score();
        if (result_better || current_best.is_unset) {
            current_best = std::move(child);
            current_best.set_score(result);

            ab.update_if_needed(result, parent.team);
            if (ab.can_prune(result, parent.team)) {
                break;
            }
        }
    }

    // The move that our opponent made is at depth 0. We make the best move at
    // depth 1. Our opponent will make one of the moves children at depth 2.
    // Thus, we want to store the children children at depth 2, which will be
    // our next moves, and move order them so that we maximize pruning.
    if (parent.depth == 2) {
        ordered_moves[current_state] = children;
    }

    return current_best.score();
}

Evaluator::score_t Searcher::evaluate(const DomineeringState& state) {
    // A copy of the state so that we can mark places temporarily and pass
    // that around to various evaluators
    DomineeringState state_copy{state};
    Evaluator::score_t total = 0;
    for (auto&& p : evaluators) {
        auto&& eval_func = p.first;
        auto&& factor_func = p.second;
        total += factor_func(state) * eval_func(&state_copy);
    }

    return total;
}

/* Private methods */

std::vector<Node> Searcher::expand(const Node& parent,
        const DomineeringState& state) {
    // Player has been toggled already
    Who my_team = state.getWho();
    unsigned my_depth = parent.depth + 1;
    std::vector<Node> children;

    // Create one instance and modify the values so that we don't have to
    // instantiate a ton of vectors in GameMove constructor.
    DomineeringMove my_move(0, 0, 0, 0);
    for (unsigned r1 = 0; r1 < state.ROWS; r1++) {
        for (unsigned c1 = 0; c1 < state.COLS; c1++) {
            // Home places horizontally, Away places vertically
            unsigned r2 = my_team == Who::HOME ? r1 : r1 + 1;
            unsigned c2 = my_team == Who::HOME ? c1 + 1 : c1;

            my_move.setMv(r1, c1, r2, c2);

            if (state.moveOK(my_move)) {
                children.push_back(Node(my_team, my_depth, Location(my_move)));
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
    }
}

void Searcher::tap(const Node& node, DomineeringState& state) {
    char c = node.team == Who::HOME ? state.HOMESYM : state.AWAYSYM;
    state.setCell(node.location.r1, node.location.c1, c);
    state.setCell(node.location.r2, node.location.c2, c);
}

void Searcher::untap(const Node& node, DomineeringState& state) {
    char c = state.EMPTYSYM;
    state.setCell(node.location.r1, node.location.c1, c);
    state.setCell(node.location.r2, node.location.c2, c);
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
