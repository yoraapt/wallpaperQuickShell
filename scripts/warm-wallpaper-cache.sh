#!/usr/bin/env bash
set -euo pipefail

walls_dir="${1:-}"
cache_dir="${2:-}"
thumb_width="${3:-280}"
thumb_height="${4:-158}"
mode="${5:-warm}"
parallel_jobs="${6:-}"

if [ -z "$walls_dir" ] || [ -z "$cache_dir" ]; then
  exit 1
fi

if [ ! -d "$walls_dir" ]; then
  exit 0
fi

mkdir -p "$cache_dir"

if [ -z "$parallel_jobs" ]; then
  parallel_jobs="$(nproc 2>/dev/null || printf '4')"
fi

case "$parallel_jobs" in
  ''|*[!0-9]*)
    parallel_jobs=4
    ;;
esac

if [ "$parallel_jobs" -lt 1 ]; then
  parallel_jobs=1
fi

if [ "$mode" = "refresh" ]; then
  find "$cache_dir" -maxdepth 1 -type f -name "*@${thumb_width}x${thumb_height}-crop.png" -delete
fi

timestamp_ms() {
  local path="$1"
  local raw
  raw="$(stat -c '%y' "$path" 2>/dev/null || true)"
  if [ -z "$raw" ]; then
    printf '0'
    return
  fi

  date -u -d "$raw" +%s%3N 2>/dev/null || printf '0'
}

cache_key() {
  local path="$1"
  local canonical size mtime_ms

  canonical="$(readlink -f "$path" 2>/dev/null || realpath "$path" 2>/dev/null || printf '%s' "$path")"
  size="$(stat -c '%s' "$path")"
  mtime_ms="$(timestamp_ms "$path")"

  printf '%s:%s:%s' "$canonical" "$size" "$mtime_ms" | sha256sum | awk '{print $1}'
}

create_thumb() {
  local path="$1"
  local key target

  key="$(cache_key "$path")"
  target="$cache_dir/${key}@${thumb_width}x${thumb_height}-crop.png"

  if [ "$mode" != "refresh" ] && [ -f "$target" ]; then
    return
  fi

  magick "$path" \
    -auto-orient \
    -resize "${thumb_width}x${thumb_height}^" \
    -gravity center \
    -extent "${thumb_width}x${thumb_height}" \
    "$target"
}

running_jobs=0
while IFS= read -r -d '' path; do
  create_thumb "$path" &
  running_jobs=$((running_jobs + 1))

  if [ "$running_jobs" -ge "$parallel_jobs" ]; then
    wait -n
    running_jobs=$((running_jobs - 1))
  fi
done < <(
  find "$walls_dir" -type f \
    \( -iname '*.png' -o -iname '*.jpg' -o -iname '*.jpeg' -o -iname '*.webp' -o -iname '*.bmp' \) \
    -print0
)

wait
