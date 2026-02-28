#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT/build"
STATE_BASE="${XDG_STATE_HOME:-$HOME/.local/state}/caelestia"
WALLPAPER_DIR="${CAELESTIA_WALLPAPERS_DIR:-$HOME/Wallpapers}"
STATE_FILE="$STATE_BASE/wallpaper/path.txt"
FIRST_WALLPAPER=""

export PATH="$HOME/.local/bin:$PATH"
export QML2_IMPORT_PATH="$BUILD_DIR/qml${QML2_IMPORT_PATH:+:$QML2_IMPORT_PATH}"
export QML_IMPORT_PATH="$BUILD_DIR/qml${QML_IMPORT_PATH:+:$QML_IMPORT_PATH}"

if [ ! -d "$BUILD_DIR/qml/Caelestia" ]; then
  cmake -S "$ROOT" -B "$BUILD_DIR"
  cmake --build "$BUILD_DIR"
fi

mkdir -p "$(dirname "$STATE_FILE")"

if [ ! -s "$STATE_FILE" ] && [ -d "$WALLPAPER_DIR" ]; then
  FIRST_WALLPAPER="$(find "$WALLPAPER_DIR" -type f \( -iname '*.png' -o -iname '*.jpg' -o -iname '*.jpeg' -o -iname '*.webp' -o -iname '*.bmp' \) | head -n 1 || true)"
  if [ -n "$FIRST_WALLPAPER" ]; then
    printf '%s\n' "$FIRST_WALLPAPER" > "$STATE_FILE"
  fi
fi

exec quickshell -p "$ROOT"
