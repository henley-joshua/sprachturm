#!/bin/bash
# Usage: gen-wayland-protocols.sh <out_dir>


set -e
OUT="$1"
XML="vendor/raylib/src/external/glfw/deps/wayland"
mkdir -p "$OUT"

for name in wayland xdg-shell xdg-decoration-unstable-v1 viewporter \
	relative-pointer-unstable-v1 pointer-constraints-unstable-v1 \
	fractional-scale-v1 xdg-activation-v1 idle-inhibit-unstable-v1; do
    wayland-scanner client-header "${XML}/${name}.xml" "${OUT}/${name}-client-protocol.h"
    wayland-scanner private-code "${XML}/${name}.xml" "${OUT}/${name}-client-protocol-code.h"
done
