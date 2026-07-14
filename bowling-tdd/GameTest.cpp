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

// Phase 3 — strike bonus (single strike and consecutive strikes),
// with strikes occurring only in frames 1-9 so that the bonus rolls are
// always ordinary frame rolls (10th-frame bonus rolls are out of scope).

TEST(GameTest, OneStrikeFollowedByOpenFramesScoresTwentyFour) {
    Game game;
    game.roll(10); // strike
    game.roll(3);
    game.roll(4);
    for (int i = 0; i < 16; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(24, game.score());
}

TEST(GameTest, TwoConsecutiveStrikesFollowedByOpenFrameScoresFortySeven) {
    Game game;
    game.roll(10); // frame 1 strike
    game.roll(10); // frame 2 strike
    game.roll(3);
    game.roll(4);
    for (int i = 0; i < 14; ++i) {
        game.roll(0);
    }
    EXPECT_EQ(47, game.score());
}

TEST(GameTest, NineStrikesThenOpenTenthFrameScoresTwoHundredSixtySeven) {
    Game game;
    for (int frame = 0; frame < 9; ++frame) {
        game.roll(10); // strike
    }
    game.roll(9);
    game.roll(0); // 10th frame: open frame, not a strike
    EXPECT_EQ(267, game.score());
}

// Phase 4 — 10th (final) frame bonus rolls and perfect game.

TEST(GameTest, PerfectGameScoresThreeHundred) {
    // Scenario A: 12 rolls of 10 (strikes throughout, including the two
    // bonus rolls in the 10th frame).
    Game game;
    for (int i = 0; i < 12; ++i) {
        game.roll(10);
    }
    EXPECT_EQ(300, game.score());
}

TEST(GameTest, TenthFrameStrikeWithTwoBonusRollsScoresSeventeen) {
    // Scenario B: frames 1-9 are all gutter balls (18 rolls of 0), and the
    // 10th frame is a strike followed by two bonus rolls of 3 and 4.
    Game game;
    for (int i = 0; i < 18; ++i) {
        game.roll(0);
    }
    game.roll(10); // 10th frame strike
    game.roll(3);  // bonus roll 1
    game.roll(4);  // bonus roll 2
    EXPECT_EQ(17, game.score());
}

TEST(GameTest, TenthFrameSpareWithOneBonusRollScoresSixteen) {
    // Scenario C: frames 1-9 are all gutter balls (18 rolls of 0), and the
    // 10th frame is a spare (5, 5) followed by one bonus roll of 6.
    Game game;
    for (int i = 0; i < 18; ++i) {
        game.roll(0);
    }
    game.roll(5); // 10th frame first roll
    game.roll(5); // 10th frame spare
    game.roll(6); // bonus roll
    EXPECT_EQ(16, game.score());
}

TEST(GameTest, TenthFrameOpenFrameHasNoBonusRollAndScoresSeven) {
    // Scenario D: frames 1-9 are all gutter balls (18 rolls of 0), and the
    // 10th frame is an open frame (4, 3) with no bonus rolls at all.
    Game game;
    for (int i = 0; i < 18; ++i) {
        game.roll(0);
    }
    game.roll(4); // 10th frame first roll
    game.roll(3); // 10th frame second roll (open, no bonus)
    EXPECT_EQ(7, game.score());
}

TEST(GameTest, NinthFrameStrikeBonusPulledFromTenthFrameBonusRollsScoresThirtySeven) {
    // Scenario E: frames 1-8 are all gutter balls (16 rolls of 0), frame 9
    // is a strike, and the 10th frame is a strike followed by two bonus
    // rolls of 2 and 3.
    //
    // Frame 9 = 10 + (next two rolls, which are the 10th frame's first two
    // rolls: 10 and 2) = 10 + 10 + 2 = 22.
    // Frame 10 = 10 + 2 + 3 = 15.
    // Total = 22 + 15 = 37.
    Game game;
    for (int i = 0; i < 16; ++i) {
        game.roll(0);
    }
    game.roll(10); // frame 9 strike
    game.roll(10); // 10th frame strike
    game.roll(2);  // bonus roll 1
    game.roll(3);  // bonus roll 2
    EXPECT_EQ(37, game.score());
}
