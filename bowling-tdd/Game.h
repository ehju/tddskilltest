#pragma once
#include <vector>

class Game {
public:
    void roll(int pins);
    int score() const;

private:
    bool isStrike(int rollIndex) const;
    bool isSpare(int rollIndex) const;

    std::vector<int> rolls_;
};
