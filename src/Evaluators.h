#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>

#include <iostream>

using DS = DomineeringState;

struct Evaluator {
        using score_t = int;

        /**
         * Checks if there is space to put a `self' domino when r1 and c1 are the
         * given.
         * Depending on `self', bottom tile or the right tile is checked.
         */
        bool has_space_for(const Who self, const int r1, const int c1,
                        const DS& state) const {
                if (state.getCell(r1, c1) != state.EMPTYSYM) {
                        return false;
                }

                auto r2 = self == Who::HOME ? r1 : r1 + 1;
                auto c2 = self == Who::HOME ? c1 + 1 : c1;

                if ((r1 < 0 || r1 >= state.ROWS || c1 < 0 || c1 >= state.COLS)
                                || (r2 < 0 || r2 >= state.ROWS || c2 < 0 || c2 >= state.COLS)) {
                        return false;
                }

                return state.getCell(r2, c2) == state.EMPTYSYM;
        }

        bool home_edge(int r1, int c1, int r2, int c2, const DS& state) const {
                return (r1 > 0 && r2 == state.ROWS
                                && c1 < state.COLS
                                && state.getCell(r1, c2) != state.EMPTYSYM
                                && state.getCell(r1, c1) != state.EMPTYSYM)

                        || (r1 < 0 && r2 < state.ROWS
                                        && c1 < state.COLS
                                        && state.getCell(r2, c2) != state.EMPTYSYM
                                        && state.getCell(r2, c1) != state.EMPTYSYM);
        }

        bool home_mid(int r1, int c1, int r2, int c2, const DS& state) const {
                return (r1 > 0 && c1 > 0
                                && state.getCell(r1, c2) != state.EMPTYSYM
                                && state.getCell(r1, c1) != state.EMPTYSYM)

                        && (r2 < state.ROWS && c1 < state.COLS
                                        && state.getCell(r2, c2) != state.EMPTYSYM
                                        && state.getCell(r2, c1) != state.EMPTYSYM);
        }

        bool away_edge(int r1, int c1, int r2, int c2, const DS& state) const {
                return (c1 > 0 && c2 == state.COLS && r1 < state.ROWS
                                && state.getCell(r2, c1) != state.EMPTYSYM
                                && state.getCell(r1, c1) != state.EMPTYSYM)

                        || (c1 < 0 && c2 < state.COLS && r1 < state.ROWS
                                        && state.getCell(r2, c2) != state.EMPTYSYM
                                        && state.getCell(r1, c2) != state.EMPTYSYM);
        }

        bool away_mid(int r1, int c1, int r2, int c2, const DS& state) const {
                return (r1 > 0 && c1 > 0
                                && r1 < state.ROWS
                                && c2 < state.COLS
                                && state.getCell(r2, c1) != state.EMPTYSYM
                                && state.getCell(r1, c1) != state.EMPTYSYM
                                && state.getCell(r2, c2) != state.EMPTYSYM
                                && state.getCell(r1, c2) != state.EMPTYSYM);
        }
};

struct EvalReserve : public Evaluator {
        score_t operator()(const DS& state) const {
                score_t home_count = 0;
                score_t away_count = 0;

                for (int i = 0; i < state.ROWS; i++) {
                        for (int j = 0; j < state.COLS; j++) {
                                // Check if HOME has reserved spot here
                                if (has_space_for(Who::HOME, i, j, state)
                                                && (home_edge(i - 1, j + 1, i + 1, j, state)
                                                        || home_mid(i - 1, j + 1, i + 1, j, state))) {
                                        home_count++;
                                }

                                // Check if AWAY has reserved spot here
                                if (has_space_for(Who::AWAY, i, j, state)
                                                && (away_edge(i + 1, j - 1, i, j + 1, state)
                                                        || away_mid(i + 1, j - 1, i, j + 1, state))) {
                                        away_count++;
                                }
                        }
                }

                //	 for (int i = 0; i < state.ROWS; i++) {
                //           for (int j = 0; j < state.COLS; j++) {
                //             std::cout << state.getCell(i,j);

                //        }
                //           std::cout << std::endl;
                //  }


                //        std::cout << "HOME: " << home_count << std::endl;

                //      std::cout << "AWAY: " << away_count << std::endl;
                //	std::cout << "BOTH: " << home_count - away_count << std::endl;

                // if we are home return this.
                return home_count - away_count;
                // if we are away return this
                //      return away_count - home_count;

        }
};

using EvalScore = std::function<Evaluator::score_t(const DS&)>;
using EvalFactor = std::function<Evaluator::score_t(const DS&)>;
// vector of pairs because std::map requires operator< and std::unordered_map
// requires operator== and hash function for DomineeringState
static const std::vector<std::pair<EvalScore, EvalFactor>> evaluators = {
        {
                std::make_pair(EvalReserve(), [](const DS& state) {
                                return 1;
                                })
        },
};

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */

