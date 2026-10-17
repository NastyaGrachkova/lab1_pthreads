#!/usr/bin/env python3
import random
import sys

def generate(n, filename, seed=42):
    random.seed(seed)
    with open(filename, "w") as f:
        f.write(f"{n}\n")
        for _ in range(n):
            mass = random.uniform(1e20, 1e24)
            x = random.uniform(-1e11, 1e11)
            y = random.uniform(-1e11, 1e11)
            z = random.uniform(-1e11, 1e11)
            vx = random.uniform(-1e3, 1e3)
            vy = random.uniform(-1e3, 1e3)
            vz = random.uniform(-1e3, 1e3)
            f.write(f"{mass} {x} {y} {z} {vx} {vy} {vz}\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: gen_input.py n filename")
        sys.exit(1)
    n = int(sys.argv[1])
    generate(n, sys.argv[2])
    print(f"Generated {n} bodies in {sys.argv[2]}")
