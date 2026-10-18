#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"
make -s

REPEATS=${REPEATS:-5}
THREADS=(1 2 4 6 8 12)

run_once() {
    local bin=$1 t=$2 ins=$3 ops=$4 sp=$5 ip=$6
    "$bin" "$t" "$ins" "$ops" "$sp" "$ip" | awk '/^Elapsed/ {print $4}'
}

stats() {
    sort -n "$1" -o "$1"
    local min avg
    min=$(head -1 "$1")
    avg=$(awk '{s+=$1} END {printf "%.6f", s/NR}' "$1")
    echo "$min $avg"
}

echo "impl,mix,threads,min,avg"

for spec in "read 8000 12000 1.0 0.0" "mixed 1000 24000 0.90 0.01"; do
    read -r mix ins ops sp ip <<< "$spec"
    for impl in ll_lib ll_my; do
        for t in "${THREADS[@]}"; do
            f=$(mktemp)
            for ((r = 1; r <= REPEATS; r++)); do
                run_once "./$impl" "$t" "$ins" "$ops" "$sp" "$ip" >> "$f"
            done
            read -r min avg <<< "$(stats "$f")"
            rm -f "$f"
            echo "$impl,$mix,$t,$min,$avg"
        done
    done
done
