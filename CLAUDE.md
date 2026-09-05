# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A ZMK firmware config repo (created from the `zmk-config-template`) for a **TOTEM** keyboard — a 38-key column-staggered split, wireless (Seeed XIAO BLE) build. It holds only *user config* — keymap, Kconfig overrides, and build matrix — not the ZMK firmware or shield definition itself. Those are pulled in via west:

- ZMK firmware source: `zmkfirmware/zmk` (pinned to `v0.3`)
- TOTEM shield definition: `bildermankawasaki/zmk-keyboard-totem` (an external ZMK module — not vendored into this repo)

Both are declared in `config/west.yml` and checked out into `.zmk/` (gitignored, not part of this repo) by `west update`.

## Layout

- `config/west.yml` — west manifest. Pins `zmk` (`zmkfirmware`, revision `v0.3`) and the `zmk-keyboard-totem` module (`bildermankawasaki`, revision `main`) which provides the `totem_left`/`totem_right` shields.
- `build.yaml` — GitHub Actions build matrix: `seeeduino_xiao_ble` board with `totem_left` and `totem_right` shields.
- `config/totem.keymap` — the keymap (4 layers: `BASE` QWERTY w/ home-row mods, `NAV`, `SYM`, `ADJ`). This overrides the default keymap bundled in the `zmk-keyboard-totem` module — ZMK prefers a `<shield>.keymap` found in `config/` over the shield's own.
- `config/totem.conf` — Kconfig overrides (currently just `CONFIG_ZMK_SLEEP=y` for battery life).
- `boards/shields/` — for a custom/unreleased shield defined *in this repo* rather than pulled in as a module. Currently just a placeholder (`.gitkeep`); not used for TOTEM since its shield lives in the external module above.
- `zephyr/module.yml` — declares this repo as a Zephyr module with `board_root: .`, so any shields placed under `boards/` here (not currently used) would be discoverable by the build.
- `.github/workflows/build.yml` — CI: calls ZMK's reusable `build-user-config.yml` workflow, which builds every entry in `build.yaml` and uploads `.uf2` firmware as build artifacts.
- `.zmk/` — local west workspace (gitignored). Populated by `west init`/`west update`; do not edit or commit anything here.

## Building firmware

Via GitHub Actions (automatic on push, or trigger manually with `workflow_dispatch`) — firmware `.uf2` files are attached as workflow artifacts. Or locally with `west`:

```sh
# one-time setup (creates/updates .zmk/)
west init -l config
west update

# build one half:
west build -s .zmk/zmk/app -b seeeduino_xiao_ble -- -DSHIELD=totem_left -DZMK_CONFIG="$PWD/config"
west build -s .zmk/zmk/app -b seeeduino_xiao_ble -- -DSHIELD=totem_right -DZMK_CONFIG="$PWD/config"
```

Flash by copying the resulting `zmk.uf2` to the XIAO BLE's mass-storage bootloader drive (double-tap reset to enter it).

## Editing the keymap

Edit `config/totem.keymap` directly. Layer indices are `#define`d at the top (`BASE`/`NAV`/`SYM`/`ADJ`); each `bindings` block's physical layout is documented by the TOTEM shield's `totem-layout.dtsi` (in the `zmk-keyboard-totem` module) — rows read top-to-bottom, left-half then right-half, with the thumb cluster last.

## Adding another keyboard to this repo

1. If it's a custom/unreleased shield, add its definition under `boards/shields/<shield_name>/`; if it's an existing community shield, add it as a module in `config/west.yml` instead (see how TOTEM is set up above).
2. Add the keymap as `config/<shield_name>.keymap` (and `config/<shield_name>.conf` for Kconfig overrides).
3. Add a `board`/`shield` (or `include:`) entry in `build.yaml`.
