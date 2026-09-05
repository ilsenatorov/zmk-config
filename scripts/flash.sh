#!/usr/bin/env bash
# Build (via scripts/build.sh) and flash the TOTEM halves.
# Usage: scripts/flash.sh <left|right|both> [--skip-build]
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SIDE="${1:-}"
SKIP_BUILD=0
[ "${2:-}" = "--skip-build" ] && SKIP_BUILD=1

if [ -z "$SIDE" ] || [[ ! "$SIDE" =~ ^(left|right|both)$ ]]; then
  echo "Usage: $0 <left|right|both> [--skip-build]"
  exit 1
fi

if [ "$SKIP_BUILD" = "0" ]; then
  "$REPO_DIR/scripts/build.sh"
fi

find_uf2_mount() {
  for base in "/run/media/ilya" "/run/media/$USER" "/media/$USER" "/media" "/mnt"; do
    [ -d "$base" ] || continue
    for d in "$base"/*/; do
      [ -f "${d}INFO_UF2.TXT" ] && { echo "${d%/}"; return 0; }
    done
  done

  # Not auto-mounted yet: try mounting any unmounted vfat partition (the
  # bootloader drive shows up as one) and check if it's the UF2 drive.
  local dev mount
  while read -r dev; do
    [ -n "$dev" ] || continue
    mount="$(udisksctl mount -b "/dev/$dev" --no-user-interaction 2>/dev/null | sed -n 's/.*at \(.*\)\.$/\1/p')"
    [ -n "$mount" ] || continue
    if [ -f "$mount/INFO_UF2.TXT" ]; then
      echo "$mount"
      return 0
    fi
    udisksctl unmount -b "/dev/$dev" --no-user-interaction >/dev/null 2>&1 || true
  done < <(lsblk -rno NAME,FSTYPE,MOUNTPOINT | awk '$2=="vfat" && $3=="" {print $1}')

  return 1
}

flash_one() {
  local uf2="$1" label="$2"
  echo
  echo ">>> Put the $label half into bootloader mode now (double-tap reset, or hold the &bootloader key if it's already running ZMK)."
  echo ">>> Waiting up to 60s for its UF2 drive to mount..."

  local mount=""
  for _ in $(seq 1 60); do
    if mount="$(find_uf2_mount)"; then break; fi
    sleep 1
  done

  if [ -z "$mount" ]; then
    echo "Timed out waiting for a UF2 bootloader drive. Skipping $label."
    return 1
  fi

  echo "Found bootloader drive at $mount — copying firmware."
  cp "$uf2" "$mount/"
  sync
  echo "$label flashed."
}

case "$SIDE" in
  left)  flash_one "$REPO_DIR/firmware/totem_left.uf2" left ;;
  right) flash_one "$REPO_DIR/firmware/totem_right.uf2" right ;;
  both)
    flash_one "$REPO_DIR/firmware/totem_left.uf2" left
    flash_one "$REPO_DIR/firmware/totem_right.uf2" right
    ;;
esac
