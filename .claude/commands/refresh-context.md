# /refresh-context - Reload Context

**[Distributed command from pg-agent-dev]**

Reload context after propagation or external changes.

## What It Does

1. Re-read CLAUDE.md
2. Re-read .claude/rules/
3. Check for new skills in .claude/skills/
4. Verify shared skills from ~/.claude/skills/
5. Report context status

## Usage

```
/refresh-context
```

## When to Use

- After running /propagate from platform
- After manual file changes outside Claude
- After pulling git changes
- When context seems stale

---

*Distributed via /propagate from pg-agent-dev*
