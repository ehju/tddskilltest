#include "Game.h"

void Game::roll(int pins) {
    rolls_.push_back(pins);
}

int Game::score() {
    int total = 0;
    size_t rollIndex = 0;
    for (int frame = 0; frame < 10 && rollIndex < rolls_.size(); ++frame) {
        int first = rolls_[rollIndex];
        total += first;
        ++rollIndex;

        int second = 0;
        bool hasSecond = rollIndex < rolls_.size();
        if (hasSecond) {
            second = rolls_[rollIndex];
            total += second;
            ++rollIndex;
        }

        if (hasSecond && first + second == 10) {
            // Spare: add next roll as bonus.
            if (rollIndex < rolls_.size()) {
                total += rolls_[rollIndex];
            }
        }
    }
    return total;
}
