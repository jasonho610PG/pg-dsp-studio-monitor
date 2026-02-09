# Binary-Only Delivery

**IP protection requirements for Studio Monitor DSP.**

---

## Delivery Format

**Customers receive:**
- `.bin` files (compiled binaries)
- `.hex` files (for flashing)
- API documentation (interface only)

**Customers DO NOT receive:**
- Source code (.c, .cpp, .h)
- Algorithm details
- Implementation notes

---

## Rationale

1. **IP Protection:** Proprietary bass correction algorithms remain confidential
2. **Competitive Advantage:** Algorithm details are trade secrets
3. **Update Control:** Customers cannot modify DSP behavior
4. **Quality Assurance:** Only validated binaries released

---

## Build Process

1. Develop in private repo (this repo)
2. Test and validate
3. Compile with optimization (`-O3`)
4. Strip debug symbols
5. Generate binary
6. Sign binary (if required)
7. Release to customer via secure channel

---

## Documentation

**Public documentation includes:**
- ProcessBlock interface (function signature)
- Feature descriptions (high-level)
- Parameter ranges
- Performance specs (latency, CPU usage)

**Private documentation (internal only):**
- Algorithm details
- Implementation notes
- Optimization strategies
- Measurement data

---

## Version Control

- **Source code:** Private git repo (this repo)
- **Binaries:** Release artifacts, versioned
- **Changelog:** Customer-facing (no algorithm details)

---

*Load this file when preparing delivery.*
