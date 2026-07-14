#include "Game.h"

namespace {
constexpr int kPinsPerFrame = 10;
}

void Game::roll(int pins) {
    rolls_.push_back(pins);
}

int Game::rollAt(size_t index) const {
    return index < rolls_.size() ? rolls_[index] : 0;
}

int Game::score() {
    int total = 0;
    size_t rollIndex = 0;
    for (int frame = 0; frame < 10 && rollIndex < rolls_.size(); ++frame) {
        int first = rolls_[rollIndex];

        if (first == kPinsPerFrame) {
            // Strike: frame ends after one roll; bonus is next two rolls.
            total += kPinsPerFrame + rollAt(rollIndex + 1) + rollAt(rollIndex + 2);
            ++rollIndex;
            continue;
        }

        int second = rollAt(rollIndex + 1);
        total += first + second;

        if (first + second == kPinsPerFrame) {
            // Spare: add next roll as bonus.
            total += rollAt(rollIndex + 2);
        }

        rollIndex += 2;
    }
    return total;
}
