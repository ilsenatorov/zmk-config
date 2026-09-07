# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

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

## Adding another keyboard to this repo

1. If it's a custom/unreleased shield, add its definition under `boards/shields/<shield_name>/`; if it's an existing community shield, add it as a module in `config/west.yml` instead (see how TOTEM is set up above).
2. Add the keymap as `config/<shield_name>.keymap` (and `config/<shield_name>.conf` for Kconfig overrides).
3. Add a `board`/`shield` (or `include:`) entry in `build.yaml`.
