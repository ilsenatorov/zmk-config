# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Config for two personal keyboards. The **TOTEM** (38-key wireless split) runs **ZMK**; the **ErgoDox EZ Glow** (wired, ATmega32u4) runs **QMK** — ZMK has no AVR support, so it cannot run on the ErgoDox. The ErgoDox's `qmk/ergodox_ez/` keymap deliberately mirrors the TOTEM's layer design (BASE/GAME/SYM/NUM/ADJ/FUN, the same home-row-mod behaviour, the same combos) while using the ErgoDox's extra keys — a real number row, arrow cluster, extra outer/side columns — for things the TOTEM has no room for. See "The ErgoDox EZ (QMK)" below for that side; everything else in this file is TOTEM/ZMK.

A ZMK firmware config repo (created from the `zmk-config-template`) for a **TOTEM** keyboard — a 38-key column-staggered split, wireless (Seeed XIAO BLE) build. It holds only *user config* — keymap, Kconfig overrides, and build matrix — not the ZMK firmware or shield definition itself. Those are pulled in via west:

- ZMK firmware source: `zmkfirmware/zmk` (**unpinned — tracks `main`**; `config/west.yml` sets no `revision` for it, so `defaults: revision: main` applies)
- TOTEM shield definition: `bildermankawasaki/zmk-keyboard-totem` (an external ZMK module — not vendored into this repo)

Both are declared in `config/west.yml` and checked out into `.zmk/` (gitignored, not part of this repo) by `west update`.

## Layout

- `config/west.yml` — west manifest. Declares `zmk` (`zmkfirmware`) and the `zmk-keyboard-totem` module (`bildermankawasaki`), which provides the `totem_left`/`totem_right` shields. **Neither declares a `revision`, so both track `main`** via `defaults: revision: main`. Verify behavior/keycode names against ZMK `main`, not a release tag.
- `build.yaml` — GitHub Actions build matrix: board `xiao_ble/nrf52840/zmk` with the `totem_left` and `totem_right` shields, no extra snippet or cmake args. ZMK Studio support was dropped (no `studio-rpc-usb-uart` snippet, no `-DCONFIG_ZMK_STUDIO=y`, no `&studio_unlock` in the keymap) since it's no longer used.
- `config/totem.keymap` — the keymap (5 layers: `BASE`, `SYM`, `NUM`, `ADJ`, `FUN` — see "Editing the keymap" below for the *why*; the file itself, including its header comment with the physical key-position numbering, is the source of truth for the *what*). This overrides the default keymap bundled in the `zmk-keyboard-totem` module — ZMK prefers a `<shield>.keymap` found in `config/` over the shield's own.
- `config/totem.conf` — Kconfig overrides for battery life: `CONFIG_ZMK_SLEEP=y` (deep sleep after 15 min idle; the shield's `kscan` has `wakeup-source`, so any keypress wakes it — reconnect just takes a couple seconds) and `CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING`/`_PROXY` (so the host can read the right half's battery, not just the left's).
- `boards/shields/` — for a custom/unreleased shield defined *in this repo* rather than pulled in as a module. Currently just a placeholder (`.gitkeep`); not used for TOTEM since its shield lives in the external module above.
- `zephyr/module.yml` — declares this repo as a Zephyr module with `board_root: .`, so any shields placed under `boards/` here (not currently used) would be discoverable by the build.
- `.github/workflows/build.yml` — CI: calls ZMK's reusable `build-user-config.yml` workflow, which builds every entry in `build.yaml` and uploads `.uf2` firmware as build artifacts.
- `.github/workflows/draw.yml` + `keymap-drawer/` — CI: calls `caksoylar/keymap-drawer`'s `draw-zmk.yml` on any push touching `config/*.keymap`, regenerating `keymap-drawer/totem.{yaml,svg}` and committing them back. **These two files are generated output — don't hand-edit them.** To refresh locally: `uvx --from keymap-drawer keymap parse -z config/totem.keymap > keymap-drawer/totem.yaml && uvx --from keymap-drawer keymap draw keymap-drawer/totem.yaml > keymap-drawer/totem.svg`.
- `.github/workflows/totem-ergodox-sync.yml` — CI: on any push/PR touching `config/totem.keymap`, warns (doesn't fail the build) if `qmk/ergodox_ez/keymap.c` wasn't touched in the same diff — a non-blocking nudge to port the change, since the two keymaps are meant to mirror each other (see "The ErgoDox EZ (QMK)" below) but are different formats with no automated way to keep them in sync.
- `.zmk/` — local west workspace (gitignored, disposable). `.zmk/config/` is a thin shim dir holding only a symlink to the real `config/west.yml` — this makes `.zmk` its own west topdir (separate from `config/`) so the vendored `zephyr/` checkout doesn't collide with this repo's own `zephyr/module.yml`. `scripts/build.sh` creates/repairs it automatically; don't edit or commit anything under it.
- `scripts/build.sh` / `scripts/flash.sh` — local build/flash automation, see below.
- `firmware/` — build output (`totem_left.uf2`, `totem_right.uf2`), gitignored.

## Building and flashing firmware

**CI**: GitHub Actions builds on every push (or manual `workflow_dispatch`); `.uf2` files are attached as workflow artifacts. Good for verifying, too slow for iterating on a keymap.

**Local (fast loop)**: `scripts/build.sh` and `scripts/flash.sh` build inside the same `zmkfirmware/zmk-build-arm:stable` Docker image CI uses — no local Zephyr SDK install needed, just Docker.

```sh
./scripts/build.sh              # builds both halves -> firmware/totem_{left,right}.uf2
SKIP_UPDATE=1 ./scripts/build.sh  # skip `west update`; ~10s incremental rebuild after a keymap-only edit

./scripts/flash.sh left         # build + flash just the left half
./scripts/flash.sh right
./scripts/flash.sh both         # flash both, one after the other
./scripts/flash.sh left --skip-build   # flash the last build without rebuilding
```

`flash.sh` polls `/run/media/$USER/*`, `/media/$USER/*`, `/media/*`, `/mnt/*` for a mounted UF2 bootloader drive (identified by `INFO_UF2.TXT`) and copies the firmware once it appears. Put the target half into bootloader mode when prompted — double-tap its reset button, or (once it's already running this keymap) toggle the `ADJ` layer with the bottom-right outer key and press the three bottom-outer keys of the half you want to flash (positions 20-21-22 on the left, 29-30-31 on the right). `&bootloader` and `&sys_reset` act on the half that receives them, which is why each exists twice.

## Editing the keymap

Edit `config/totem.keymap` directly. Layer indices are `#define`d at the top (`BASE`/`SYM`/`NUM`/`ADJ`/`FUN`); each `bindings` block's physical layout is documented by the TOTEM shield's `totem-layout.dtsi` (in the `zmk-keyboard-totem` module) — rows read top-to-bottom, left-half then right-half, with the thumb cluster last. **The keymap's own header comment is the authoritative, current physical key-position numbering** (0-37) and the `KEYS_L`/`KEYS_R`/`THUMBS` groupings derived from it — don't re-derive or duplicate that numbering here; read it there. The home-row-mod positional guard depends on it, so keep the comment in sync if the shield layout ever changes.

Layer design, and *why* it is the way it is (for the actual bindings, read the layer itself — this is only the non-obvious reasoning behind them):

- **BASE** — QWERTY with timer-less home-row mods (`hml`/`hmr`: `balanced` flavor, 280 ms term, `require-prior-idle-ms`, a bilateral `hold-trigger-key-positions` guard and `hold-trigger-on-release`). The guard is deliberate: `Super`+a left-hand letter must take GUI from the **right** hand (`;`) and vice-versa, otherwise the hold resolves as a tap.
- **SYM** (held on the **left** thumb, `Space`) — symbols, all plain `&kp`. Openers sit on the right index column and closers on the right middle column.
- **NUM** (held on the **right** thumb, `Tab`) — a real number row and **mirrored modifiers on both home rows**. The mirrored mods are what make the Hyprland workspace bindings work: `Super+1..0`, `Super+Shift+1..0` and `Super+Ctrl+1..0` are 30 combinations and cannot be pre-composed onto a layer. Rule: **digits 1-5 take right-hand modifiers, digits 6-0 take left-hand modifiers** — every same-hand pairing is a finger collision. `&mo FUN` also lives on NUM's left pinky (position 20), so FUN nests under NUM too — hold `Tab`, hold the pinky key, tap a top-row key.
- **ADJ** — Bluetooth, output and media. Reached by holding `&mo ADJ` on the bottom-right outer key (momentary, not toggled); position 31 stays `&trans` on ADJ so releasing that key always exits. `&sys_reset`/`&bootloader` are 2-key combos gated to this layer (outer pair = boot, inner pair = reset, mirrored per half) rather than plain keys.
- **FUN** — F1-F12, reached directly from BASE by holding the left pinky key (position 20, `&mo FUN`) or nested under NUM (see above). F1-F10 mirror NUM's number row exactly (same top-row positions as `1`-`0`); F11/F12 continue down the rightmost column — F11 at the home-row position under F10, F12 at the bottom-row position under that.

`&lt` is overridden globally with `quick-tap-ms` (upstream has none, so hold-to-repeat on Space/Esc/Tab would not work). Its flavor stays `tap-preferred` on purpose: with `balanced`, rolling from `Space` into the next word's first letter resolves as a hold and fires SYM.

No local Docker? `docker` may be installed but inactive and the user not in the `docker` group. Without a build you can still validate a keymap edit statically: check 38 bindings per layer, and that every keycode and behavior resolves against `app/include/dt-bindings/zmk/keys.h` and `app/dts/behaviors/` on ZMK `main`. `uvx --from keymap-drawer keymap parse -z config/totem.keymap` is also a real parse and will fail on malformed devicetree.

## Adding another ZMK keyboard to this repo

1. If it's a custom/unreleased shield, add its definition under `boards/shields/<shield_name>/`; if it's an existing community shield, add it as a module in `config/west.yml` instead (see how TOTEM is set up above).
2. Add the keymap as `config/<shield_name>.keymap` (and `config/<shield_name>.conf` for Kconfig overrides).
3. Add a `board`/`shield` (or `include:`) entry in `build.yaml`.

## The ErgoDox EZ (QMK)

Not built via west/Zephyr at all — QMK has its own toolchain, and this board doesn't use ZMK Studio, west, or `config/`.

When you change `config/totem.keymap`, check whether the change belongs on the ErgoDox too (layer design, home-row-mod tuning, combos) and port it to `qmk/ergodox_ez/keymap.c` in the same commit/PR — `.github/workflows/totem-ergodox-sync.yml` warns (non-blocking) if it doesn't. Genuinely Totem-only changes (physical position numbering, ZMK-specific behaviors like sleep/BLE) are expected to trip the warning; that's fine, it's just a nudge.

- `qmk/ergodox_ez/` — the QMK "keymap folder": `keymap.c` (six `LAYOUT_ergodox_pretty` layers, the `chordal_hold_layout` bilateral-mod guard, the dual-function number row, combos), `config.h` (tap-hold/combo tuning), `rules.mk` (feature flags), `keymap.json` (`{"modules": ["zsa/oryx", "zsa/defaults"]}` — this, not an `ORYX_ENABLE` flag, is what keeps ZSA's Keymapp/live-training working on this firmware branch; `ORYX_ENABLE` doesn't exist here and is silently ignored if set).
- Built against **ZSA's QMK fork, branch `firmware25`** (`https://github.com/zsa/qmk_firmware`) — the same branch Oryx itself compiles from — not mainline QMK, because the fork carries the `zsa/oryx` community module and matches Oryx's `CHORDAL_HOLD`/`FLOW_TAP` behaviour. Pin changes deliberately; don't float to a newer `firmwareNN` branch without re-checking flash usage.
- **Flash is the hard constraint on every future edit.** `LAYER_STATE_8BIT` caps layers at 8 (this keymap uses 6) and the ATmega32u4 has 32256 bytes total; the current keymap compiles to 28656 bytes (3600 free). Before adding a layer, combo, or feature flag, check the build's printed size line (`scripts/build-ergodox.sh` also greps and warns if free space drops under 512 bytes). If space runs out, the RGB Matrix effect table (`keyboards/zsa/ergodox_ez/config.h` upstream enables ~45 animations) is the single biggest lever — trim it with `#undef ENABLE_RGB_MATRIX_*` entries in `qmk/ergodox_ez/config.h`.
- `scripts/build-ergodox.sh` — Docker build (`qmkfm/qmk_cli` image), mirrors `scripts/build.sh`'s structure. Clones `zsa/qmk_firmware` into `.qmk/` (gitignored) and initializes only the `lib/lufa` and `modules/zsa` submodules (not the full submodule set — that pulls in ChibiOS/pico-sdk/lvgl for nothing on AVR). `SKIP_UPDATE=1` skips the fetch for a fast keymap-only rebuild. Sets `GIT_CEILING_DIRECTORIES` to stop git's repo-discovery from walking up past the repo root — needed because `.qmk` is nested inside this checkout.
  ```sh
  ./scripts/build-ergodox.sh              # -> firmware/ergodox_ez_glow.hex
  SKIP_UPDATE=1 ./scripts/build-ergodox.sh  # fast rebuild after a keymap-only edit
  ```
- `scripts/flash-ergodox.sh` — the ErgoDox EZ uses the **Teensy HalfKay bootloader, not a UF2 drive** (unlike the TOTEM), so this polls for the HalfKay USB device (`16c0:0478`) instead of a mount point, then flashes with `wally-cli` or `teensy_loader_cli`. Needs ZSA's udev rules (`/etc/udev/rules.d/50-zsa.rules`) to flash without root.
- Bootloader entry: hold `MO(ADJ)` (row-3 right outer key) and tap the `QK_BOOT` key on ADJ, or press the physical reset button (paperclip hole, top-right of the right half).
