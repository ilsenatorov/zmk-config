#!/usr/bin/env bash
# Build TOTEM firmware for both halves using the same Docker image ZMK's CI uses,
# so local builds match GitHub Actions exactly. No local Zephyr SDK needed.
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE="zmkfirmware/zmk-build-arm:stable"
BOARD="xiao_ble/nrf52840/zmk"
HOST_UID_GID="$(id -u):$(id -g)"

# .zmk is a separate west workspace (sibling to config/, not nested under it)
# so the vendored `zephyr/` checkout doesn't collide with our own
# zephyr/module.yml. .zmk/config is a thin shim dir holding just a symlink to
# the real config/west.yml, which is what makes topdir=.zmk work.
INIT_CMD="echo '.zmk already initialized'"
if [ ! -d "$REPO_DIR/.zmk/.west" ]; then
  INIT_CMD="mkdir -p $REPO_DIR/.zmk/config && ln -sf ../../config/west.yml $REPO_DIR/.zmk/config/west.yml && west init -l $REPO_DIR/.zmk/config"
fi

UPDATE_CMD="west update --fetch-opt=--filter=tree:0"
if [ "${SKIP_UPDATE:-0}" = "1" ]; then
  UPDATE_CMD="echo 'Skipping west update (SKIP_UPDATE=1)'"
fi

docker run --rm \
  -v "$REPO_DIR:$REPO_DIR" \
  -w "$REPO_DIR/.zmk" \
  "$IMAGE" \
  bash -c "
    set -euo pipefail
    git config --global --add safe.directory '*'
    $INIT_CMD
    $UPDATE_CMD
    west zephyr-export
    west build -s zmk/app -d build/totem_left  -b $BOARD -p auto -- -DZMK_CONFIG=$REPO_DIR/config -DSHIELD=totem_left  -DZMK_EXTRA_MODULES=$REPO_DIR
    west build -s zmk/app -d build/totem_right -b $BOARD -p auto -- -DZMK_CONFIG=$REPO_DIR/config -DSHIELD=totem_right -DZMK_EXTRA_MODULES=$REPO_DIR
    mkdir -p $REPO_DIR/firmware
    cp build/totem_left/zephyr/zmk.uf2  $REPO_DIR/firmware/totem_left.uf2
    cp build/totem_right/zephyr/zmk.uf2 $REPO_DIR/firmware/totem_right.uf2
    chown -R $HOST_UID_GID $REPO_DIR/.zmk $REPO_DIR/firmware
  "

echo "Firmware ready: $REPO_DIR/firmware/totem_left.uf2, $REPO_DIR/firmware/totem_right.uf2"
