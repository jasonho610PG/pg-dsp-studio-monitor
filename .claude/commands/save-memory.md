# /save-memory - Save Session Learnings

**[Distributed command from pg-agent-dev]**

Capture session learnings to memory system.

## What It Does

1. Review conversation for learnings:
   - Corrections (when Claude made mistakes)
   - Patterns (recurring decisions or methods)
   - Decisions (architectural choices)
2. Categorize learnings by type
3. Write to appropriate memory file
4. Confirm saved entries

## Memory Types

| File | Purpose |
|------|---------|
| `memory/corrections.md` | When Claude was wrong |
| `memory/patterns.md` | Recurring methods or approaches |
| `memory/decisions.md` | Architectural choices, rationale |

## Usage

```
/save-memory
```

## Output Format

Each entry includes:
- Date
- Category
- Summary
- Context
- Impact

---

*Distributed via /propagate from pg-agent-dev*
