---
name: git_commit
description: >-
  Instructions and workflow for generating and executing well-structured Git commits 
  with a short summary line and a detailed body description.
---

# Git Commit Workflow Skill

Use this skill whenever generating or executing Git commits.

## Key Rules & Guidelines

1. **No Pre-Commit Compilation Checks**:
   * **Do NOT run `qmk compile`** or attempt build verification prior to committing changes.
   * Assume all code edits are already functional and verified by the user.

2. **Inspect Staged/Unstaged Changes**:
   * Run `git status` and `git diff` (or `git diff --cached`) to analyze the exact modifications.

3. **Message Formatting Guidelines**:
   * **Short Summary (Subject Line)**:
     * Keep to 50–72 characters.
     * Use conventional commit prefixes where appropriate (`fix:`, `feat:`, `refactor:`, `docs:`, `style:`).
     * Written in imperative mood (e.g., `fix(hp_km): update OLED timeout` instead of `fixed...`).
   * **Detailed Body**:
     * Separated from the subject line by a blank line.
     * Explains *what* was modified, *why* the change was made, and key context or rationale.
     * Can contain multiple paragraphs if needed.

4. **Executing the Commit**:
   * Use `git add` to stage the relevant files.
   * Format the commit command with multiple `-m` flags to separate subject line and body paragraphs cleanly:
     ```bash
     git commit -m "<Short summary line>" \
                -m "<Detailed body paragraph 1>" \
                -m "<Detailed body paragraph 2>"
     ```

5. **Post-Commit Verification**:
   * Run `git log -n 1` to verify the committed message format and output the result to the user.
