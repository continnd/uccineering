#include "Moderator.h"

/**
 * Overview of TODOs (ordered by importance)
 *  - non-recursive Searcher::search_under
 *  - bug fixes and testing
 *  - evaluation functions
 *  - move ordering
 *  - variable search depths
 *  - speeding up the whole program
 *  - spawning threads
 *  - initial file I/O
 */

int main(int argc, char* argv[]) {
    Moderator mod{"uccineers"};
    mod.compete(argc, argv);

    return 0;
}

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
