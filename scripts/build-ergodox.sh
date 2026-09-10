#!/usr/bin/env bash
# Build ErgoDox EZ Glow firmware in Docker against ZSA's QMK fork
# (zsa/qmk_firmware, branch firmware25 — same branch Oryx itself builds
# from), so no local AVR toolchain is needed. Mirrors build.sh's structure.
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
QMK_DIR="$REPO_DIR/.qmk"
QMK_REPO="https://github.com/zsa/qmk_firmware"
QMK_REF="firmware25"
IMAGE="qmkfm/qmk_cli"
KB="zsa/ergodox_ez/m32u4/glow"
KM="totem"
HOST_UID_GID="$(id -u):$(id -g)"

CLONE_CMD="echo '.qmk already initialized'"
if [ ! -d "$QMK_DIR/.git" ]; then
  CLONE_CMD="git clone --depth 1 --branch $QMK_REF --no-recurse-submodules $QMK_REPO $QMK_DIR"
fi

UPDATE_CMD="git -C $QMK_DIR fetch --depth 1 origin $QMK_REF && git -C $QMK_DIR checkout $QMK_REF && git -C $QMK_DIR submodule update --init --depth 1 lib/lufa modules/zsa"
if [ "${SKIP_UPDATE:-0}" = "1" ]; then
  UPDATE_CMD="git -C $QMK_DIR submodule update --init --depth 1 lib/lufa modules/zsa"
fi

docker run --rm \
  -v "$REPO_DIR:$REPO_DIR" \
  -w "$QMK_DIR" \
  -e "GIT_CEILING_DIRECTORIES=$REPO_DIR" \
  "$IMAGE" \
  bash -c "
    set -euo pipefail
    git config --global --add safe.directory '*'
    $CLONE_CMD
    $UPDATE_CMD
    rm -rf keyboards/zsa/ergodox_ez/m32u4/glow/keymaps/$KM
    mkdir -p keyboards/zsa/ergodox_ez/m32u4/glow/keymaps/$KM
    cp -f $REPO_DIR/qmk/ergodox_ez/* keyboards/zsa/ergodox_ez/m32u4/glow/keymaps/$KM/
    make -j\$(nproc) $KB:$KM | tee $QMK_DIR/build.log
    mkdir -p $REPO_DIR/firmware
    cp $QMK_DIR/*.hex $REPO_DIR/firmware/ergodox_ez_glow.hex
    chown -R $HOST_UID_GID $QMK_DIR $REPO_DIR/firmware
  "

FREE_BYTES="$(grep -oE '[0-9]+ bytes free' "$QMK_DIR/build.log" | grep -oE '^[0-9]+' || true)"
if [ -n "$FREE_BYTES" ] && [ "$FREE_BYTES" -lt 512 ]; then
  echo "WARNING: only $FREE_BYTES bytes of flash free — see the flash budget notes in the repo's CLAUDE.md" >&2
fi

echo "Firmware ready: $REPO_DIR/firmware/ergodox_ez_glow.hex"
