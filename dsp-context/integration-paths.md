# Integration Paths

**How new system integrates with legacy `/Users/jasonho610/Desktop/studio-monitor`.**

---

## Strategy

**Read-only references:** New system reads legacy, never modifies.

---

## Integration Points

### 1. Testbed Code

**Legacy Path:** `/Users/jasonho610/Desktop/studio-monitor/testbed/`

**Use Case:** Reference existing JUCE plugins

**Who Uses:** Prototype agent

**How:**
- Read legacy JUCE code for algorithm reference
- Copy validated algorithms to new testbed
- Do NOT modify legacy testbed

---

### 2. Measurement Tools

**Legacy Path:** `/Users/jasonho610/Desktop/studio-monitor/measurements/`

**Use Case:** Reuse Python scripts, REW templates

**Who Uses:** Validation agent

**How:**
- Read legacy measurement scripts
- Copy to new system if useful
- Adapt to new directory structure

---

### 3. Documentation

**Legacy Path:** `/Users/jasonho610/Desktop/studio-monitor/docs/`

**Use Case:** Historical TRDs, design docs

**Who Uses:** Documentation agent

**How:**
- Read legacy docs for reference
- Compare to historical measurements
- Do NOT modify legacy docs

---

### 4. Validated Algorithms

**Legacy Knowledge → New Skills:**

Example: EQ10 algorithm validated in legacy system → extracted to `dsp-bass-correction/eq10-algorithm.md`

**Important:** Extract methodology, not code. Skills are knowledge, not implementation.

---

## Migration Path (Optional, Future)

As new system matures, features can migrate from legacy to new repo:

1. Validate feature in new system
2. Confirm parity with legacy system
3. Deprecate legacy implementation
4. Update references

**Current Status:** Read-only integration (no migration yet)

---

*Load this file when working with legacy system.*
