# /cleanup - Pre-Commit Review

**[Distributed command from pg-agent-dev]**

Analyze changes, run quality checks, and prepare repo for commit.

## What It Does

1. Show changes (git status)
2. Run /lint for garbage patterns
3. Analyze issues:
   - Large files (> 1MB)
   - Sensitive data (.env, secrets, credentials)
   - Untracked generated files
   - Sensitivity scan (API keys, SSH keys, hardcoded credentials, .pem/.key files)
4. Check docs (CLAUDE.md, SPEC.md changes)
5. Report status (GO/NO-GO)

## Status Determination

- **READY:** No critical issues
- **REVIEW NEEDED:** Warnings exist but not blocking
- **BLOCKED:** Critical issues, sensitive data, credentials in git

## Important

- Read-only command (does not modify files)
- Run BEFORE /ship
- Critical issues block shipping

---

*Distributed via /propagate from pg-agent-dev*
