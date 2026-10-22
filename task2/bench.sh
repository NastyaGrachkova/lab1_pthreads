#!/bin/bash
cd "$(dirname "$0")"
make -s

INPUT=${1:-inputs/test_1000.txt}
if [ ! -f "$INPUT" ]; then
    mkdir -p "$(dirname "$INPUT")"
    n=$(basename "$INPUT" | sed -n 's/^test_\([0-9][0-9]*\)\.txt$/\1/p')
    if [ -z "$n" ]; then
        echo "Нет файла $INPUT. Создайте его: ./gen_input N $INPUT" >&2
        exit 1
    fi
    ./gen_input "$n" "$INPUT"
fi
T_END=${2:-1.0}
DT=${3:-0.01}
REPEATS=${4:-5}

echo "Input: $INPUT"
echo "t_end=$T_END dt=$DT repeats=$REPEATS"
echo "threads,min,avg,median"

for T in 1 2 4 6 8 12; do
    F=/tmp/times_${T}.txt
    > "$F"
    for r in $(seq 1 "$REPEATS"); do
        ./nbody "$T_END" "$INPUT" "$T" "$DT" /tmp/bench_${T}.csv \
            | awk '/Elapsed/ {print $2}' >> "$F"
    done

    sort -n "$F" -o "$F"
    min=$(head -1 "$F")
    avg=$(awk '{s+=$1} END {printf "%.4f", s/NR}' "$F")
    median=$(awk '{a[NR]=$1} END {if (NR%2) printf "%.4f", a[(NR+1)/2]; else printf "%.4f", (a[NR/2]+a[NR/2+1])/2}' "$F")

    echo "$T,$min,$avg,$median"
done
