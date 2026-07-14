#include "Game.h"

void Game::roll(int pins) {
    rolls_.push_back(pins);
}

int Game::score() {
    int total = 0;
    size_t rollIndex = 0;
    for (int frame = 0; frame < 10 && rollIndex < rolls_.size(); ++frame) {
        int first = rolls_[rollIndex];

        if (first == 10) {
            // Strike: frame ends after one roll; bonus is next two rolls.
            total += 10;
            ++rollIndex;
            if (rollIndex < rolls_.size()) {
                total += rolls_[rollIndex];
            }
            if (rollIndex + 1 < rolls_.size()) {
                total += rolls_[rollIndex + 1];
            }
            continue;
        }

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
