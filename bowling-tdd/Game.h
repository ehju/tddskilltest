#pragma once

#include <vector>

// Bowling game score keeper.
//
// Supports open frames, spares, and strikes (including consecutive
// strikes). 10th-frame bonus rolls are handled in a later phase.
class Game {
public:
    void roll(int pins);
    int score();

private:
    int rollAt(size_t index) const;

    std::vector<int> rolls_;
};
