#include <gtest/gtest.h>
#include "Game.h"

TEST(GameTest, GutterGameScoresZero) {
    Game game;
    for (int i = 0; i < 20; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(0, game.score());
}

TEST(GameTest, AllOnesScoresTwenty) {
    Game game;
    for (int i = 0; i < 20; ++i) {
        game.roll(1);
    }
    EXPECT_EQ(20, game.score());
}

TEST(GameTest, OneSpare) {
    Game game;
    game.roll(5);
    game.roll(5); // spare
    game.roll(3);
    for (int i = 0; i < 17; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(16, game.score());
}

TEST(GameTest, OneStrike) {
    Game game;
    game.roll(10); // strike
    game.roll(3);
    game.roll(4);
    for (int i = 0; i < 16; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(24, game.score());
}

TEST(GameTest, PerfectGameScoresThreeHundred) {
    Game game;
    for (int i = 0; i < 12; ++i) {
        game.roll(10);
    }
    EXPECT_EQ(300, game.score());
}
