# Customized QMK + Vial Firmware Setup

A high-level overview of this customized QMK / Vial repository setup, target hardware platform, core QMK framework modifications, build instructions, and the Antigravity (`agy`) AI agent rules discovery mechanism.

---

## 🚀 Setup Overview

This repository is a customized fork of **QMK Firmware** integrated with **Vial**, specifically engineered for split ergonomic keyboards powered by **RP2040** microcontrollers (specifically the **Adafruit KB2040**).

### Hardware & Architecture Summary
* **Keyboard Target:** Sofle Rev1 Split Keyboard (`sofle/rev1`)
* **Microcontroller (MCU):** Adafruit KB2040 (Raspberry Pi RP2040 MCU) via `CONVERT_TO=kb2040`
* **Custom Keymap Name:** `hp_km`
* **Keymap Location:** [`keyboards/sofle/rev1/keymaps/hp_km`](file:///home/hope/full_vial_qmk_hope/keyboards/sofle/rev1/keymaps/hp_km)
* **Connectivity Modes:** Seamless hardware USB host detection supporting standard TRRS split operation, reversed split master/slave operation, and dual-USB independent split operation without requiring re-flashing.

---

## 🛠️ Core QMK File Modifications

To support advanced runtime behavior beyond standard QMK/Vial capabilities, targeted modifications have been made directly to core QMK submodules and drivers:

1. **Dynamic Mouse Acceleration & Speedup:**
   * Modified core mousekey routines (`quantum/mousekey.c`) to provide non-linear dynamic acceleration curves for smooth precision control at low speeds while rapidly scaling up during continuous movement or encoder input.
2. **RP2040 Memory Alignment & RPC Protocol:**
   * Tailored split communications (`oled_sync_data_t` and custom split RPC transactions) using strict `__attribute__((packed))` struct definitions and explicit `memcpy` buffering to prevent Cortex-M0+ unaligned 32-bit access hard-fault crashes.
3. **Quantum Painter (QP) & OLED Graphics Engine:**
   * Optimized monochrome OLED rendering allocations (`QUANTUM_PAINTER_NUM_FONTS` memory alignment) and font drawing routines for low-latency visual feedback on display screens.

---

## 📦 Build & Compile Commands

Compile commands require target conversion flags (`CONVERT_TO=kb2040`) and split side definition flags:

### Left Side (Default / Master):
```bash
qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE"
```

### Right Side:
```bash
qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_RIGHT_SIDE"
```

---

## 🤖 Antigravity (`agy`) Rule Discovery Mechanism (`AGENTS.md`)

This repository includes configuration files for pair programming with Google Antigravity AI agents (`agy`). Antigravity uses a **hierarchical upward traversal mechanism** to discover rule files (`AGENTS.md`, `GEMINI.md`, and `.agents/rules/*.md`).

### How `agy` Discovers and Loads `AGENTS.md`

1. **Upward Walk Traversal:**
   When `agy` starts a session or executes a prompt, it starts at the **Current Working Directory (PWD)** and walks **UPWARDS** folder-by-folder until it reaches the Git repository root (where `.git` is located).
2. **Contextual File Open Traversal:**
   When files are opened or edited during a turn, `agy` walks UP from that file's parent directory to the Git root, gathering any rule files encountered along the path.
3. **Rule Merging & Deduplication:**
   Rules found across parent directories are combined into system instructions. `agy` automatically deduplicates rule files to ensure each file is injected only once per turn.
4. **No Downward Traversal:**
   `agy` **never scans downwards** into child directories automatically. If your PWD is the repository root, sub-directory `AGENTS.md` files (such as inside `keyboards/sofle/rev1/keymaps/hp_km/`) are **not** loaded unless a file inside that directory is explicitly opened or edited.

---

## 📊 Visual Representation of `AGENTS.md` Discovery

### Scenario 1: PWD is Repository Root (`/home/hope/full_vial_qmk_hope`)
When starting at the repository root, only the root `AGENTS.md` is loaded initially:

```text
📁 /home/hope/full_vial_qmk_hope (PWD & Git Root)
 ├── 📄 AGENTS.md                          <-- [READ & LOADED]
 └── 📁 keyboards/
      └── 📁 sofle/
           └── 📁 rev1/
                └── 📁 keymaps/
                     └── 📁 hp_km/
                          └── 📄 AGENTS.md  <-- [IGNORED] (Below PWD; loaded only if hp_km file is opened)
```

---

### Scenario 2: PWD is inside Keymap Folder (`.../keymaps/hp_km`)
When starting or working inside `hp_km`, `agy` walks **UP** and loads **BOTH** rule files:

```text
📁 /home/hope/full_vial_qmk_hope (Git Root)
 │    📄 AGENTS.md                          <-- [READ & LOADED] (Step 2: Parent walk-up target)
 └── 📁 keyboards/
      └── 📁 sofle/
           └── 📁 rev1/
                └── 📁 keymaps/
                     └── 📁 hp_km (PWD)
                          └── 📄 AGENTS.md  <-- [READ & LOADED] (Step 1: Starting PWD)
```

---

### Scenario 3: PWD is Root, but a file in `hp_km/` is edited
When editing a file in `hp_km` from the root, `agy` triggers a walk-up from that file:

```text
📄 Editing: keyboards/sofle/rev1/keymaps/hp_km/keymap.c
 ├── Step 1: Read keyboards/sofle/rev1/keymaps/hp_km/AGENTS.md  <-- [LOADED]
 └── Step 2: Walk UP to repository root -> Read AGENTS.md     <-- [LOADED & MERGED]
```
