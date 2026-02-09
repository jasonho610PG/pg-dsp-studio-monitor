# Memory System

**Feedback and session learnings for pg-dsp-studio-monitor.**

---

## Structure

```
.claude/memory/
├── README.md (this file)
└── feedback/
    └── entries/
        └── [timestamped entries]
```

---

## Feedback System

Use `/feedback` command to capture feedback:

```
/feedback [category] [description]
```

Categories: BUG, IMPROVEMENT, CLARIFICATION, PERFORMANCE, UX

Entries saved to `.claude/memory/feedback/entries/YYYY-MM-DD-HHMMSS.md`

---

## Root Memory Files

Session learnings (corrections, patterns, decisions) are saved to:
- `memory/corrections.md` (when Claude is wrong)
- `memory/patterns.md` (recurring methods)
- `memory/decisions.md` (architectural choices)

Use `/save-memory` to capture learnings.

---

*Memory accumulates knowledge over time.*
