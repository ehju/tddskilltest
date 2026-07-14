export const meta = {
  name: 'tdd-workflow',
  description: 'Turn a requirement into a phased PLAN.md, then drive each phase through an independent RED -> GREEN -> REFACTOR gmock TDD loop',
  whenToUse: 'When the user gives a coding requirement (e.g. the bowling kata) and wants it implemented via strict TDD with gmock, phase by phase, where each RED/GREEN/REFACTOR step is done by an agent that only sees the phase spec and the current repo state (no shared memory across steps).',
  phases: [
    { title: 'Plan', detail: 'read requirement, write PLAN.md and phase{n}.md files' },
    { title: 'RED', detail: 'write failing gmock tests from the phase spec only' },
    { title: 'GREEN', detail: 'make the tests pass' },
    { title: 'REFACTOR', detail: 'review against spec, refactor, decide if phase is done' },
  ],
}

const PLAN_SCHEMA = {
  type: 'object',
  properties: {
    phaseCount: { type: 'number', description: 'total number of phases created (phase1.md..phaseN.md)' },
    summary: { type: 'string', description: 'one-line summary of the phase breakdown' },
  },
  required: ['phaseCount'],
}

const REFACTOR_VERDICT_SCHEMA = {
  type: 'object',
  properties: {
    requirementsMet: { type: 'boolean', description: 'are all requirements in phase{n}.md fully implemented' },
    refactorDone: { type: 'boolean', description: 'is the resulting code clean, with no further refactor needed' },
    notes: { type: 'string', description: 'what is missing, or what was changed during refactor' },
  },
  required: ['requirementsMet', 'refactorDone'],
}

if (!args || !args.requirements) {
  throw new Error('args.requirements (string) is required — e.g. the contents of bowlingkata.md')
}
const requirements = args.requirements
const maxRoundsPerPhase = args.maxRoundsPerPhase || 3

phase('Plan')
const plan = await agent(
  `너는 이 리포지토리(Visual Studio C++ 프로젝트, gmock 사용)의 구현 계획을 세우는 에이전트다.
다음 요구사항을 구현해야 한다:

${requirements}

작업:
1. 리포지토리 루트에 PLAN.md 를 작성하라. 전체 구현을 작고 검증 가능한 단위의 phase 들로 나누고, 각 phase의 목표를 요약하라.
2. 각 phase마다 phase{N}.md 파일(N=1부터 시작하는 정수, 예: phase1.md, phase2.md)을 리포지토리 루트에 작성하라. 이 파일은 자기완결적이어야 한다 — 이후 이 phase를 담당할 에이전트들은 오직 이 파일과 현재 코드만 보고 작업하며, 이 계획을 세운 과정이나 다른 phase 파일의 맥락을 전혀 알지 못한다. 따라서 phase{N}.md 에는 해당 phase에서 구현할 요구사항, 관련 입출력 예시, 완료 조건을 구체적으로 적어라.
3. 빌드/테스트는 리포지토리 루트의 build-and-test.ps1 스크립트로 수행된다. 이 스크립트를 직접 실행할 필요는 없다.
작업이 끝나면 생성한 phase 개수를 구조화된 결과로 반환하라.`,
  { schema: PLAN_SCHEMA, label: 'plan' }
)

log(`계획 수립 완료: 총 ${plan.phaseCount}개 phase${plan.summary ? ' — ' + plan.summary : ''}`)

for (let n = 1; n <= plan.phaseCount; n++) {
  log(`--- Phase ${n}/${plan.phaseCount} 시작 ---`)
  let verdict = { requirementsMet: false, refactorDone: false }
  let round = 0

  while (round < maxRoundsPerPhase && !(verdict.requirementsMet && verdict.refactorDone)) {
    round++

    await agent(
      `너는 RED 단계를 담당하는 에이전트다. 이전 단계 에이전트들의 사고 과정이나 대화 내용은 전혀 알 수 없고, 오직 리포지토리에 있는 파일만 보고 작업한다.
리포지토리 루트의 phase${n}.md 를 읽어라. 그 파일에 적힌 요구사항만을 근거로 gmock 테스트를 작성하라.
실제 구현이 어떻게 될지는 절대 고려하지 마라 — 오직 요구사항이 정의하는 동작을 검증하는 테스트만 작성한다.
테스트가 지금 통과하는지는 신경쓰지 마라 (아직 구현이 없거나 불완전하므로 실패하는 것이 정상이다).
이 phase를 위해 이미 작성된 테스트가 있다면 필요에 따라 수정/보강하되, phase${n}.md 의 요구사항 범위를 벗어나지 마라.
새 소스/헤더 파일을 추가했다면 bowling-tdd/bowling-tdd.vcxproj 에 <ClCompile>/<ClInclude> 항목으로 등록해야 빌드에 포함된다.`,
      { label: `red-p${n}-r${round}`, phase: 'RED' }
    )

    await agent(
      `너는 GREEN 단계를 담당하는 에이전트다. 이전 단계 에이전트들의 사고 과정이나 대화 내용은 전혀 알 수 없고, 오직 리포지토리에 있는 파일만 보고 작업한다.
리포지토리 루트의 phase${n}.md 와 현재 작성되어 있는 gmock 테스트를 읽어라.
테스트를 통과시키기 위한 구현 코드를 작성하라. 테스트 코드 자체는 수정하지 마라 (테스트가 phase${n}.md 의 요구사항을 명백히 잘못 반영하고 있는 경우가 아니라면).
리포지토리 루트의 build-and-test.ps1 스크립트를 powershell로 실행해서 빌드하고 테스트를 돌려라 (예: powershell -File build-and-test.ps1). 모든 테스트가 통과할 때까지 구현을 수정하고 다시 실행하는 것을 반복하라.
새 소스/헤더 파일을 추가했다면 bowling-tdd/bowling-tdd.vcxproj 에 <ClCompile>/<ClInclude> 항목으로 등록해야 빌드에 포함된다.
모든 테스트가 통과하는 것을 build-and-test.ps1 실행 결과로 확인한 뒤에만 작업을 끝내라.`,
      { label: `green-p${n}-r${round}`, phase: 'GREEN' }
    )

    verdict = await agent(
      `너는 REFACTOR 단계를 담당하는 에이전트다. 이전 단계 에이전트들의 사고 과정이나 대화 내용은 전혀 알 수 없고, 오직 리포지토리에 있는 파일만 보고 작업한다.
리포지토리 루트의 phase${n}.md 와 현재 코드, 테스트를 검토하라.
1. phase${n}.md 에 적힌 요구사항이 모두 구현되었는지 판단하라.
2. 코드에 개선이 필요하면 (중복 제거, 이름 개선, 구조 정리 등) 직접 리팩토링하라. 리팩토링 후에는 반드시 리포지토리 루트의 build-and-test.ps1 을 실행해서 (예: powershell -File build-and-test.ps1) 모든 테스트가 여전히 통과하는지 확인하고, 통과하지 않으면 통과할 때까지 수정하라.
3. 작업이 끝나면 요구사항 충족 여부(requirementsMet), 리팩토링 완료 여부(refactorDone), 특기사항(notes: 무엇이 부족한지 또는 무엇을 바꿨는지)을 구조화된 결과로 반환하라.`,
      { schema: REFACTOR_VERDICT_SCHEMA, label: `refactor-p${n}-r${round}` }
    )

    log(`Phase ${n} round ${round}: requirementsMet=${verdict.requirementsMet}, refactorDone=${verdict.refactorDone}${verdict.notes ? ' — ' + verdict.notes : ''}`)
  }

  if (!(verdict.requirementsMet && verdict.refactorDone)) {
    log(`Phase ${n} 이 ${maxRoundsPerPhase}회 반복 후에도 완료되지 않았습니다. 수동 확인이 필요합니다: ${verdict.notes || ''}`)
  } else {
    log(`--- Phase ${n} 완료 ---`)
  }
}

return { totalPhases: plan.phaseCount }
