#ifndef MODERATOR_H_
#define MODERATOR_H_

#include "Searcher.h"
#include "TranspositionTable.h"

#include "DomineeringMove.h"
#include "GamePlayer.h"

#include <algorithm>
#include <fstream>
#include <string>

/**
 * A class that acts as a supervisor of the searches, thread management, file
 * I/O, and communication with the judge server.
 */

static const std::string GAME_NAME = "Domineering";

class Moderator : public GamePlayer {
public:
    // Default constructor
    Moderator();

    /**
     * Constructs a moderator with the given name.
     *
     * \param[in] team_name the name of the team.
     */
    Moderator(const std::string& team_name);

    // Copy constructor
    Moderator(const Moderator& other);

    // Move constructor
    Moderator(Moderator&& other);

    // Destructor
    ~Moderator();

    // Assignment operator
    Moderator& operator=(const Moderator& other);

    /**
     * Reads in the file that contains the transposition table.
     */
    void init() override;

    /**
     * Does housekeeping stuff like joining threads.
     */
    void done() override;

    /**
     * Uses the searcher to get the next move.
     *
     * \param[in] last_move the last move made by the opponent.
     *
     * \return the optimal next move found.
     */
    DomineeringMove next_move(const DomineeringState& last_move);

    /**
     * Spawns a new searcher that helps search on different threads.
     * TODO
     *
     * \param[in] node the nodes for each thread to search on.
     *
     * \return the best-move node found by the worker searcher.
     */
    Node spawn_searcher(const Node& nodes);

    GameMove* getMove(GameState& state,
            const std::string& last_move) override;

    /**
     * Have fun and learn something new!
     */
    std::string messageForOpponent(const std::string& opponent_name) override;

private:
    /**
     * Determines the depth to search down to.
     *
     * \param[in] state the current state of the game.
     *
     * \return the maximum depth to search.
     */
    unsigned get_search_depth(const DomineeringState& state) const;

    std::string team_name;
    Searcher searcher;
    DomineeringMove next_game_move;
};

inline std::string
Moderator::messageForOpponent(const std::string& opponent_name) {
    return "What's 1 among friends?";
}

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
