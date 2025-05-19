import matplotlib.pyplot as plt

# Данные
threads = [1, 2, 4, 8, 16]
times = [31.11, 15.86, 8.47, 7.23, 8.42]

# Вычисления
T1 = times[0]
speedups = [T1 / t for t in times]
efficiencies = [s / p * 100 for s, p in zip(speedups, threads)]

# Построение графиков
plt.figure(figsize=(15, 4))

# Время выполнения
plt.subplot(1, 3, 1)
plt.plot(threads, times, marker='o')
plt.title('Время выполнения')
plt.xlabel('Число потоков')
plt.ylabel('Время (сек)')
plt.grid(True)

# Ускорение
plt.subplot(1, 3, 2)
plt.plot(threads, speedups, marker='o', color='green')
plt.title('Ускорение')
plt.xlabel('Число потоков')
plt.ylabel('Ускорение (Sp)')
plt.grid(True)

# Эффективность
plt.subplot(1, 3, 3)
plt.plot(threads, efficiencies, marker='o', color='red')
plt.title('Эффективность')
plt.xlabel('Число потоков')
plt.ylabel('Эффективность (%)')
plt.grid(True)

plt.tight_layout()
plt.show()

