# /ship - Git Commit + Push

**[Distributed command from pg-agent-dev]**

Create git commit and push to remote.

## What It Does

1. Run git status (show untracked files, staged/unstaged changes)
2. Run git diff (show changes that will be committed)
3. Run git log (review recent commit messages for style)
4. Draft commit message (concise, follows repo style)
5. Add relevant untracked files
6. Create commit with co-author tag
7. Push to remote (if requested)

## Usage

```
/ship [message]
```

Example:
- `/ship "Add EQ10 CMSIS-DSP implementation"`

## Commit Format

```
[Summary line]

[Optional details]

Co-Authored-By: Claude Sonnet 4.5 <noreply@anthropic.com>
```

## Important

- Only commit when explicitly asked
- Prefer adding specific files (not git add -A)
- Never use --no-verify flag
- Never force push to main/master

---

*Distributed via /propagate from pg-agent-dev*
