#!/usr/bin/env bash
# Build (via scripts/build-ergodox.sh) and flash the ErgoDox EZ.
# Unlike the Totem, the ErgoDox EZ uses the Teensy HalfKay bootloader, not a
# UF2 mass-storage drive, so this polls for a USB device instead of a mount.
# Usage: scripts/flash-ergodox.sh [--skip-build]
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HEX="$REPO_DIR/firmware/ergodox_ez_glow.hex"
SKIP_BUILD=0
[ "${1:-}" = "--skip-build" ] && SKIP_BUILD=1

if [ "$SKIP_BUILD" = "0" ]; then
  "$REPO_DIR/scripts/build-ergodox.sh"
fi

FLASHER=""
if command -v wally-cli >/dev/null 2>&1; then
  FLASHER="wally-cli"
elif command -v teensy_loader_cli >/dev/null 2>&1; then
  FLASHER="teensy_loader_cli"
else
  echo "Neither wally-cli nor teensy_loader_cli found on PATH." >&2
  echo "Install ZSA's Wally CLI (https://github.com/zsa/wally-cli) or teensy_loader_cli." >&2
  exit 1
fi

if [ ! -e /etc/udev/rules.d/50-zsa.rules ]; then
  echo "WARNING: /etc/udev/rules.d/50-zsa.rules not found — flashing without root may fail." >&2
  echo "See https://docs.zsa.io/wally/troubleshooting/ (Linux udev rules) if flashing fails." >&2
fi

echo
echo ">>> Press the reset button on the ErgoDox (paperclip hole, top-right of the right half) now,"
echo ">>> or hold ADJ + the QK_BOOT key if it's already running this keymap."
echo ">>> Waiting up to 60s for the HalfKay bootloader..."

FOUND=0
for _ in $(seq 1 60); do
  if lsusb -d 16c0:0478 >/dev/null 2>&1; then
    FOUND=1
    break
  fi
  sleep 1
done

if [ "$FOUND" != "1" ]; then
  echo "Timed out waiting for the HalfKay bootloader (USB 16c0:0478)." >&2
  exit 1
fi

echo "Bootloader detected — flashing."
case "$FLASHER" in
  wally-cli) wally-cli "$HEX" ;;
  teensy_loader_cli) teensy_loader_cli -mmcu=atmega32u4 -w -v "$HEX" ;;
esac

echo "ErgoDox flashed."
