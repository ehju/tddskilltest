#include "Game.h"

void Game::roll(int pins) {
    rolls_.push_back(pins);
}

int Game::score() {
    int total = 0;
    size_t rollIndex = 0;
    for (int frame = 0; frame < 10 && rollIndex < rolls_.size(); ++frame) {
        total += rolls_[rollIndex];
        ++rollIndex;
        if (rollIndex < rolls_.size()) {
            total += rolls_[rollIndex];
            ++rollIndex;
        }
    }
    return total;
}
