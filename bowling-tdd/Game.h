#pragma once

#include <vector>

// Bowling game score keeper.
//
// Supports open frames, spares, and strikes (including consecutive
// strikes), as well as the 10th (final) frame's bonus rolls (spare/strike
// bonus rolls and the perfect game).
class Game {
public:
    void roll(int pins);
    int score();

private:
    int rollAt(size_t index) const;

    std::vector<int> rolls_;
};
