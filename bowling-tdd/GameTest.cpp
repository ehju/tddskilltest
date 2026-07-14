#include <gmock/gmock.h>

#include "Game.h"

// Phase 1 — open frames only (no strike/spare bonus logic required).

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

TEST(GameTest, AllOpenFramesOfThreeAndFourScoresSeventy) {
    Game game;
    for (int frame = 0; frame < 10; ++frame) {
        game.roll(3);
        game.roll(4);
    }
    EXPECT_EQ(70, game.score());
}

// Phase 2 — spare bonus.

TEST(GameTest, OneSpareFollowedByOpenFramesScoresSixteen) {
    Game game;
    game.roll(5);
    game.roll(5); // spare
    game.roll(3);
    for (int i = 0; i < 17; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(16, game.score());
}

TEST(GameTest, AllSparesGameScoresOneHundredFortyEight) {
    Game game;
    for (int frame = 0; frame < 9; ++frame) {
        game.roll(5);
        game.roll(5); // spare
    }
    game.roll(5);
    game.roll(5); // 10th frame spare
    game.roll(3); // bonus roll
    EXPECT_EQ(148, game.score());
}
