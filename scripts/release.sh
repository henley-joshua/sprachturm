#!/bin/bash
# scripts/release.sh

set -e

RAYLIB_SRC="vendor/raylib/src"
SRC_DIR="src"

build_release() {
    local PLATFORM=$1
    local CC=$2
    local PLATFORM_FLAGS=$3
    local LDFLAGS=$4
    local EXT=$5
    local OUT_DIR="build/release-${PLATFORM}"

    echo "=== Building ${PLATFORM} release ==="
    mkdir -p "${OUT_DIR}/bin" "${OUT_DIR}/raylib"

    # Compile Raylib
    echo "Compiling Raylib..."
    for src in ${RAYLIB_SRC}/rcore.c \
               ${RAYLIB_SRC}/rshapes.c \
               ${RAYLIB_SRC}/rtextures.c \
               ${RAYLIB_SRC}/rtext.c \
               ${RAYLIB_SRC}/rmodels.c \
               ${RAYLIB_SRC}/raudio.c \
               ${RAYLIB_SRC}/rglfw.c; do
        name=$(basename $src .c)
        ${CC} -std=c11 \
            -I${RAYLIB_SRC} \
            -I${RAYLIB_SRC}/external/glfw/include \
            ${PLATFORM_FLAGS} -O2 -w \
            -c $src -o "${OUT_DIR}/raylib/${name}.o"
    done

    ar rcs "${OUT_DIR}/libraylib.a" ${OUT_DIR}/raylib/*.o
    echo "Raylib built."

    # Compile game sources
    echo "Compiling game..."
    GAME_OBJS=""
    while IFS= read -r src; do
        rel="${src#${SRC_DIR}/}"          # strip src/ prefix
        obj="${OUT_DIR}/${rel%.c}.o"      # swap .c for .o
        mkdir -p "$(dirname $obj)"
        ${CC} -std=c11 -O2 -DNDEBUG \
            -I./utils -I./include -I./src \
            -I${RAYLIB_SRC} \
            -I${RAYLIB_SRC}/external/glfw/include \
            ${PLATFORM_FLAGS} \
            -c "$src" -o "$obj"
        GAME_OBJS="$GAME_OBJS $obj"
    done < <(find ${SRC_DIR} -name '*.c')

    # Link
    echo "Linking..."
    ${CC} ${GAME_OBJS} "${OUT_DIR}/libraylib.a" \
        -o "${OUT_DIR}/bin/game${EXT}" \
        ${LDFLAGS}

    echo "=== ${PLATFORM} release built: ${OUT_DIR}/bin/game${EXT} ==="
}

case "${1}" in
    linux)
        build_release "linux" "gcc" \
            "-D_GLFW_X11 -DPLATFORM_DESKTOP" \
            "-lm -lGL -lpthread -ldl -lrt -lX11" \
            ""
        ;;
    windows)
        build_release "windows" "x86_64-w64-mingw32-gcc" \
            "-DPLATFORM_DESKTOP" \
            "-lm -lopengl32 -lgdi32 -lwinmm -static" \
            ".exe"
        ;;
    all)
        build_release "linux" "gcc" \
            "-D_GLFW_X11 -DPLATFORM_DESKTOP" \
            "-lm -lGL -lpthread -ldl -lrt -lX11" \
            ""
        build_release "windows" "x86_64-w64-mingw32-gcc" \
            "-DPLATFORM_DESKTOP" \
            "-lm -lopengl32 -lgdi32 -lwinmm -static" \
            ".exe"
        ;;
    *)
        echo "Usage: $0 [linux|windows|all]"
        exit 1
        ;;
esac
