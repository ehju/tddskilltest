#pragma once

#include <vector>

// Bowling game score keeper.
//
// Phase 1 scope: only open frames (no strike/spare bonus handling required).
class Game {
public:
    void roll(int pins);
    int score();

private:
    std::vector<int> rolls_;
};
