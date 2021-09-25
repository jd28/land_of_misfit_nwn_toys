#ifndef TALIB_CUSTOM_DICE_H
#define TALIB_CUSTOM_DICE_H

#include <ostream>
#include <stdlib.h>

struct DiceRoll {
    int32_t      dice;
    int32_t      sides;
    int32_t      bonus;

    DiceRoll ()
    	: dice(), sides(), bonus() {}
    DiceRoll (int32_t d, int32_t s, int32_t b)
        : dice(d), sides(s), bonus(b) {}

    bool operator==(const DiceRoll& other) const {
        return ( dice  == other.dice  &&
                 sides == other.sides &&
                 bonus == other.bonus );
    }
    bool operator!=(const DiceRoll& other) const {
        return !(*this == other);
    }
};

static inline int32_t rollDice(DiceRoll d, int32_t n = 1) {
    if ( n <= 0 ) { n = 1; }
    int32_t r = 0;
    for ( int i = d.dice * n; i > 0; --i ) {
        r += (rand() % d.sides) + 1;
    }
    return r + d.bonus;
}

static inline bool rollIsValid(DiceRoll d) {
    return (d.dice > 0 && d.sides > 0) || d.bonus > 0;
}


static inline std::ostream &operator<<(std::ostream &out, const DiceRoll &o) {
    out << o.dice << "d" << o.sides << " + " << o.bonus;
    return out;
}

#endif // TALIB_CUSTOM_DICE_H
