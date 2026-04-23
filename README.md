# My notes and solutions to 'Computer Systems a Programmer's Perspective'
### Targeted study notes and solutions — GPU programming track

<img src="./images/computer-systems-programmers-perspective-cover.png" width="25%">

Get a solid foundation to understand low level concepts on computer systems as a basis for GPU programming. 

The book is written for systems programmers who need to understand everything from logic gates to OS internals. We'll focus on a targeted subset of the entirety of the book - the parts that explain why hardware behaves the way it does, because GPU programming is fundamentally about exploiting hardware behavior at a low level.

<img src="./images/csapp_cuda_roadmap.svg" width="75%">

---

## Purpose

This repository contains my notes, problem solutions, and working C code from a
**targeted reading of CSAPP 3rd edition** (Bryant & O'Hallaron), filtered
specifically for GPU programming, CUDA kernel optimization, and ML workloads.

The book is written for systems programmers who need to understand everything
from logic gates to OS internals. This repo covers a deliberate subset — the
chapters that explain why hardware behaves the way it does, because GPU
programming is fundamentally about exploiting hardware behavior at a low level.

The **north star** is *Programming Massively Parallel Processors* (PMPP) 5th
edition by Hwu, Kirk & El Hajj. Every chapter and problem in this repo was
selected against its payoff for the following PMPP topics, in priority order:

1. Memory coalescing, shared memory tiling, bank conflicts (PMPP Ch. 5, 6, 7, 8, 15)
2. Warp execution, latency hiding, occupancy (PMPP Ch. 4, 6)
3. Atomic operations, race conditions, synchronization (PMPP Ch. 9, 10, 11, 12)
4. Mixed-precision arithmetic — fp16, bf16, fp8 (PMPP Appendix A, Ch. 15, 20)
5. CUDA memory model: unified memory, pinned memory, PCIe transfer costs (PMPP Appendix C)
6. LLM kernels: attention mechanism, flash attention, KV caching (PMPP Ch. 20)

---

## Chapter roadmap

The full interactive roadmap (priority tiers, curated sections, problem
selection, rationale) is available as a rendered HTML file:

**[`csapp_roadmap_v2.html`](csapp_roadmap.html)** — open locally in a browser.

A visual overview:

![Roadmap](images/csapp_cuda_roadmap.svg)

### Chapter coverage summary

| Chapter | Title | Priority | PMPP connection |
|---------|-------|----------|-----------------|
| Ch. 2 | Representing and Manipulating Information | **Critical** | fp16/bf16 layout, IEEE 754 rounding, integer overflow in index arithmetic |
| Ch. 3 | Machine-Level Representation of Programs | Skim | PTX register model, predicate execution, data alignment → bank conflicts |
| Ch. 4 | Processor Architecture | Skim | Pipeline hazards → warp latency hiding, CPI → occupancy reasoning |
| Ch. 5 | Optimizing Program Performance | **Critical** | ILP, loop unrolling, latency- vs throughput-bound — the CPU roofline analog |
| Ch. 6 | The Memory Hierarchy | **Critical** | Coalescing, tiling, bank conflicts, memory mountain → roofline model |
| Ch. 7 | Linking | Skim | CUDA compilation model, runtime kernel loading, cuBLAS/cuDNN linking |
| Ch. 8 | Exceptional Control Flow | Skim | CUDA context model, user/kernel mode crossings, checkpoint signal handling |
| Ch. 9 | Virtual Memory | **Critical** | Unified memory page faults, pinned memory, PCIe transfer mechanics |
| Ch. 12 | Concurrent Programming | Skim | Race conditions, memory visibility — conceptual basis for CUDA atomics |

Chapters 1, 10, and 11 are skipped entirely (no traceable PMPP payoff).

### Reading sequence

```
CSAPP Ch. 2 + Ch. 6
        |
        v
   PMPP Ch. 2–4         <- begin GPU programming foundations
        |
        v
   CSAPP Ch. 9          <- complete before PMPP Ch. 5
        |
        v
   PMPP Ch. 5+          <- memory architecture, tiling, performance
        |
  (alongside)
        |
   CSAPP Ch. 3 skim     <- before PMPP Ch. 6 (bank conflicts)
   CSAPP Ch. 12 skim    <- before PMPP Ch. 9 (atomics)
```

---

## Repository structure

```
csapp/
├── src/
│   ├── ch02/          # Data representation — bit manipulation, integer, float
│   ├── ch05/          # Performance optimization — loop unrolling, ILP
│   ├── ch06/          # Memory hierarchy — cache-friendly access patterns
│   ├── ch09/          # Virtual memory — address translation exercises
│   └── ch12/          # Concurrent programming — race conditions, atomics
├── images/
│   ├── computer-systems-programmers-perspective-cover.png
│   └── csapp_cuda_roadmap.svg
├── vendor/
│   └── unity/         # Unity test framework for C
├── csapp_roadmap_v2.html
└── README.md
```

Solutions are organized by chapter. Each subdirectory contains a `Makefile`
and uses [Unity](https://github.com/ThrowTheSwitch/Unity) for C unit tests.

---

## Curated problem sets

Problems were tiered by how directly they train a skill required to understand
a specific PMPP section — not just by chapter topic.

**Ch. 2 — Data Representation** (43 total → 13 selected)
- Tier 1: 2.61, 2.63, 2.72, 2.82, 2.87, 2.89, 2.90, 2.96
- Tier 2: 2.59, 2.60, 2.70, 2.84, 2.88

**Ch. 5 — Optimizing Program Performance** (7 total → 5 selected)
- Tier 1: 5.15, 5.17, 5.19
- Tier 2: 5.13, 5.14

**Ch. 6 — The Memory Hierarchy** (25 total → 11 selected)
- Tier 1: 6.29, 6.33, 6.37, 6.44, 6.45
- Tier 2: 6.25, 6.34, 6.36, 6.38, 6.39, 6.46

**Ch. 9 — Virtual Memory** (selected)
- Tier 1: 9.11, 9.14, 9.17 *(address translation and TLB reasoning only — allocator implementation is out of scope)*

**Ch. 12 — Concurrent Programming** (24 total → 2 selected)
- Tier 1: 12.28, 12.29

The rationale for each selection is documented in the roadmap HTML.

---

## Supplementary reading

Resources that fill gaps not covered by CSAPP, ordered by priority:

| Resource | Read when | Gap it fills |
|----------|-----------|--------------|
| Williams, Waterman & Patterson — *Roofline* (2009) | Before PMPP Ch. 5 | Arithmetic intensity, memory-boundedness, speed-of-light analysis — the primary analytical framework of PMPP Ch. 5–6 |
| Dao et al. — *FlashAttention* (2022) | Alongside PMPP §20.5 | Online softmax correctness argument; PMPP's treatment is pedagogically clean but doesn't fully derive it |
| NVIDIA CUDA Programming Guide — SIMT Architecture | Alongside PMPP Ch. 4 | Warp execution model, divergence, scheduling — no CSAPP analog |
| NVIDIA Hopper Architecture Whitepaper | Before PMPP Ch. 15 + 20 | Tensor core hardware mechanism; explains fp16 vs bf16 behavior across GPU generations |
| Micikevicius et al. — *Mixed Precision Training* (2018) | Before PMPP Appendix A + Ch. 20 | Loss scaling, fp16 accumulation strategy, fp32 master weights |

---

## Background

This study plan assumes:
- Strong background in mathematics, statistics, and data science
- Some C/C++ programming experience
- No prior low-level systems or GPU programming background

The goal is to build the systems intuition required to read PMPP fluently — not
to become a systems programmer. Chapters and problems were selected with that
distinction in mind.

---

*CSAPP 3rd ed. — Bryant & O'Hallaron · PMPP 5th ed. — Hwu, Kirk & El Hajj*
