# zmk-config

Personal firmware config for two keyboards: a [TOTEM](https://github.com/bildermankawasaki/zmk-keyboard-totem) — a 38-key column-staggered split, wireless (Seeed XIAO BLE) build on [ZMK](https://zmk.dev/) — and an ErgoDox EZ Glow on QMK, whose keymap deliberately mirrors the TOTEM's layer design.

See [CLAUDE.md](CLAUDE.md) for details on the repo layout and local build/flash workflow.

## TOTEM keymap

Rendered automatically on every push by [keymap-drawer](https://github.com/caksoylar/keymap-drawer) — regenerate manually via the "Draw keymaps" GitHub Action if it ever drifts from `config/totem.keymap`.

![totem keymap](keymap-drawer/totem.svg)

## ErgoDox EZ keymap

Rendered automatically on every push by the "Draw ErgoDox keymap" GitHub Action — regenerate manually via that Action if it ever drifts from `qmk/ergodox_ez/keymap.c`.

![ergodox keymap](keymap-drawer/ergodox_ez.svg)
