# /update-docs - Sync Documentation

**[Distributed command from pg-agent-dev]**

Sync documentation after structural changes.

## What It Does

1. Read CLAUDE.md, agent files, skill files
2. Check for inconsistencies:
   - Commands listed but files missing
   - Agents referenced but not defined
   - Skills listed but not present
   - Orphaned files
3. Update CLAUDE.md if needed
4. Report sync status

## Usage

```
/update-docs
```

## When to Use

- After adding new agents
- After adding new skills
- After adding new commands
- Before committing major changes

## Output

- Inconsistencies found (if any)
- Files updated (if any)
- Sync status (SYNCED / NEEDS_REVIEW)

---

*Distributed via /propagate from pg-agent-dev*
