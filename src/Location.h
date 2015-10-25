#ifndef LOCATION_H_
#define LOCATION_H_

#include "DomineeringMove.h"

#include <istream>

/**
 * A class that represents a certain location on the board.
 * In the game of Domineering, the location of a domino is described using two
 * coordinates.
 */
struct Location {
    unsigned r1, c1, r2, c2;

    Location()
        : r1{0}
        , c1{0}
        , r2{0}
        , c2{0}
    { };

    Location(const unsigned r1, const unsigned c1,
            const unsigned r2, const unsigned c2)
        : r1{r1}
        , c1{c1}
        , r2{r2}
        , c2{c2}
    { }

    Location(const DomineeringMove& d_move)
        : r1{static_cast<unsigned>(d_move.row1())}
        , c1{static_cast<unsigned>(d_move.col1())}
        , r2{static_cast<unsigned>(d_move.row2())}
        , c2{static_cast<unsigned>(d_move.col2())}
    { }

    Location& operator=(const Location& other);

    bool operator==(const Location& other) const;
    bool operator!=(const Location& other) const;

    /**
     * Converts this location to a DomineeringMove object.
     *
     * \return the DomineeringMove that has this location as the move.
     */
    DomineeringMove to_move() const;
};

inline Location& Location::operator=(const Location& other) {
    r1 = other.r1;
    c1 = other.c1;
    r2 = other.r2;
    c2 = other.c2;
    return *this;
}

inline bool Location::operator==(const Location& other) const {
    return r1 == other.r1 && r2 == other.r2
        && c1 == other.c1 && c2 == other.c2;
}

inline bool Location::operator!=(const Location& other) const {
    return r1 != other.r1 || r2 != other.r2
        || c1 != other.c1 || c2 != other.c2;
}

inline DomineeringMove Location::to_move() const {
    return DomineeringMove(r1, c1, r2, c2);
}

namespace std {
    template<>
    struct hash<Location> {
        size_t operator()(const Location& l) const {
            size_t r1{std::hash<unsigned>()(l.r1)};
            size_t c1{std::hash<unsigned>()(l.c1)};
            size_t r2{std::hash<unsigned>()(l.r2)};
            size_t c2{std::hash<unsigned>()(l.c2)};

            size_t r{((r1 << 1) ^ r2) >> 1};
            return (((r << 1) ^ c1) >> 1) ^ c2;
        }
    };
} // namespace std

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */
