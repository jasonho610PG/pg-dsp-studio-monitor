# /feedback - Capture Feedback

**[Distributed command from pg-agent-dev]**

Capture user feedback and propose actions.

## What It Does

1. Record feedback with metadata (timestamp, category, source)
2. Analyze feedback for patterns
3. Propose concrete actions
4. Save to `.claude/memory/feedback/entries/`

## Feedback Categories

- BUG: Something is broken
- IMPROVEMENT: Enhancement suggestion
- CLARIFICATION: Documentation needs improvement
- PERFORMANCE: Speed or efficiency issue
- UX: User experience concern

## Usage

```
/feedback [category] [description]
```

Example:
- `/feedback IMPROVEMENT "EQ10 prototype should validate phase response"`

## Output

- Feedback entry saved
- Action items proposed
- Related patterns identified

---

*Distributed via /propagate from pg-agent-dev*
