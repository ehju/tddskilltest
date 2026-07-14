#include "Game.h"

void Game::roll(int pins) {
    rolls_.push_back(pins);
}

int Game::score() const {
    int total = 0;
    int rollIndex = 0;
    for (int frame = 0; frame < 10; ++frame) {
        if (isStrike(rollIndex)) {
            total += 10 + rolls_[rollIndex + 1] + rolls_[rollIndex + 2];
            rollIndex += 1;
        } else if (isSpare(rollIndex)) {
            total += 10 + rolls_[rollIndex + 2];
            rollIndex += 2;
        } else {
            total += rolls_[rollIndex] + rolls_[rollIndex + 1];
            rollIndex += 2;
        }
    }
    return total;
}

bool Game::isStrike(int rollIndex) const {
    return rolls_[rollIndex] == 10;
}

bool Game::isSpare(int rollIndex) const {
    return rolls_[rollIndex] + rolls_[rollIndex + 1] == 10;
}
