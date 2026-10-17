import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

threads = np.array([1, 2, 4, 6, 8, 12])

# min-время из замеров
data = {
    "1000 частиц, t_end=1.0": [0.892304, 0.855176, 0.616149, 0.598654, 0.572182, 0.557523],
    "5000 частиц, t_end=1.0": [17.315784, 10.600144, 7.594173, 6.104214, 4.902288, 4.674120],
    "20000 частиц, t_end=0.2":[63.981223, 41.512978, 23.988269, 19.009406, 15.135718, 14.208085],
}

fig, axes = plt.subplots(1, 2, figsize=(14, 5))

# Ускорение
ax = axes[0]
ax.plot(threads, threads, 'k--', label='Идеальное (линейное)')
for label, times in data.items():
    t = np.array(times)
    S = t[0] / t
    ax.plot(threads, S, 'o-', label=label)
ax.axvline(6, color='red', alpha=0.3, linestyle=':', label='6 физических ядер')
ax.set_xlabel('Число потоков p')
ax.set_ylabel('Ускорение S = T1 / Tp')
ax.set_title('Ускорение')
ax.grid(True)
ax.legend(fontsize=8)

# Эффективность
ax = axes[1]
for label, times in data.items():
    t = np.array(times)
    S = t[0] / t
    E = S / threads
    ax.plot(threads, E, 's-', label=label)
ax.axhline(1.0, color='k', linestyle='--', alpha=0.5)
ax.axvline(6, color='red', alpha=0.3, linestyle=':', label='6 физических ядер')
ax.set_xlabel('Число потоков p')
ax.set_ylabel('Эффективность E = S / p')
ax.set_title('Эффективность')
ax.set_ylim(0, 1.1)
ax.grid(True)
ax.legend(fontsize=8)

plt.tight_layout()
plt.savefig('results/speedup.png', dpi=150)
print("Saved results/speedup.png")

# Таблица S, E для 5000 и 20000
for label in ["5000 частиц, t_end=1.0", "20000 частиц, t_end=0.2"]:
    t = np.array(data[label])
    S = t[0] / t
    E = S / threads
    print(f"\n{label}")
    print(" p |    T     |   S    |   E")
    for p, tt, s, e in zip(threads, t, S, E):
        print(f"{p:2d} | {tt:8.4f} | {s:6.3f} | {e:6.3f}")
