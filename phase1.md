# Phase 1 — 프로젝트 부트스트랩 + 기본 프레임 점수 (오픈 프레임/거터 게임)

## 배경
이 리포지토리는 Visual Studio C++ 프로젝트(`bowling-tdd.slnx` / `bowling-tdd/bowling-tdd.vcxproj`)이며, gmock(NuGet 패키지 `gmock.1.11.0`, `packages.config`에 이미 참조됨)을 사용해 테스트를 작성한다. 빌드/테스트는 리포지토리 루트의 `build-and-test.ps1`이 수행한다 — 이 스크립트는 `bowling-tdd.slnx`를 MSBuild로 빌드한 뒤, 생성된 `x64\Debug\bowling-tdd.exe`(gmock 콘솔 실행 파일)를 실행하여 종료 코드로 성공/실패를 판단한다. 즉 산출물 실행 파일(`bowling-tdd.exe`)이 gmock 테스트 러너여야 한다.

**중요: 현재 이 프로젝트에는 `.cpp`/`.h` 소스 파일이 하나도 없다.** `bowling-tdd/bowling-tdd.vcxproj`에는 `packages.config`만 `<None Include>`로 등록되어 있을 뿐, `<ItemGroup>`에 `ClCompile`/`ClInclude` 항목이 전혀 없다. 이번 phase에서 처음으로 소스 파일들을 추가해야 한다.

## 이번 phase에서 해야 할 일

1. gmock 테스트를 구동할 수 있는 최소한의 진입점을 만든다. 예: `bowling-tdd/main.cpp`에서
   ```cpp
   #include <gmock/gmock.h>

   int main(int argc, char** argv) {
       ::testing::InitGoogleMock(&argc, argv);
       return RUN_ALL_TESTS();
   }
   ```
   (프로젝트에서 이미 채택하고 있는 스타일이 있다면 그것을 따르되, 핵심은 `RUN_ALL_TESTS()`를 통해 등록된 모든 테스트가 실행되고, 테스트 실패 시 0이 아닌 종료 코드를 반환해야 한다는 점이다.)
2. `Game` 클래스를 구현한다 (예: `bowling-tdd/Game.h`, `bowling-tdd/Game.cpp` — 헤더/구현부 분리 여부는 자유롭게 결정 가능).
   - `void roll(int pins)`: 투구 1회를 기록.
   - `int score()`: 지금까지 기록된 투구를 바탕으로 총점 계산.
   - 이번 phase에서는 **스트라이크(첫 투구 10핀)나 스페어(두 투구 합 10)로 인한 보너스 점수는 고려하지 않아도 된다.** 즉, 모든 프레임이 "오픈 프레임"(두 번의 투구 합이 10 미만)인 경우만 정확하면 통과.
3. 테스트 파일을 추가한다 (예: `bowling-tdd/GameTest.cpp`), gmock의 `TEST(...)` 매크로를 사용해 아래 "완료 조건"의 시나리오를 검증하는 테스트를 작성한다.
4. 새로 추가한 모든 소스 파일(`main.cpp`, `Game.h`, `Game.cpp`, `GameTest.cpp` 등)을 `bowling-tdd/bowling-tdd.vcxproj`와 `bowling-tdd/bowling-tdd.vcxproj.filters`에 등록한다.
   - `.vcxproj`에는 `<ItemGroup>` 안에 `.cpp` 파일은 `<ClCompile Include="파일명" />`, `.h` 파일은 `<ClInclude Include="파일명" />`로 추가한다.
   - `.filters` 파일에는 이미 정의된 필터("소스 파일", "헤더 파일")에 맞춰 각 파일을 등록한다 (`<ClCompile>`은 "소스 파일" 필터, `<ClInclude>`는 "헤더 파일" 필터).
   - 기존에 있던 `packages.config` 관련 설정은 그대로 유지한다.

## 입출력 예시 (이번 phase에서 통과해야 하는 시나리오)

- 시나리오 A — 거터 게임: 20번의 투구 모두 0핀. `score()` == `0`.
- 시나리오 B — 전부 1핀: 20번의 투구 모두 1핀. `score()` == `20`.
- (선택) 시나리오 C — 스트라이크/스페어가 없는 임의의 오픈 프레임 게임, 예: 10프레임 모두 `(3, 4)`로 투구 → 프레임당 7점 × 10프레임 = `70`.

각 시나리오는 다음과 같은 패턴으로 `roll`을 반복 호출한 뒤 `score()`를 호출하는 방식으로 테스트한다.
```cpp
Game game;
for (int i = 0; i < 20; ++i) {
    game.roll(0);
}
EXPECT_EQ(0, game.score());
```

## 완료 조건
- `build-and-test.ps1` 실행 시 빌드가 성공하고, 위 시나리오들을 검증하는 gmock 테스트가 모두 통과한다 (스크립트 종료 코드 0).
- `Game` 클래스가 `roll(int pins)`와 `score()` 시그니처를 정확히 제공한다.
- 스트라이크/스페어 보너스 로직은 구현하지 않아도 되며(구현해도 무방하지만 필수 아님), 다음 phase(Phase 2)에서 별도로 다룬다.
