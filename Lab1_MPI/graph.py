import matplotlib.pyplot as plt

# Данные для последовательной программы и двух вариантов параллельных запусков
T1 = 36.969714  # Время работы последовательной программы

# Время работы для Var1 и Var2 на различных числах потоков
var1_times = [36.389871, 18.742376, 9.244554, 4.911699, 4.619284]
var2_times = [48.718565, 25.879797, 12.426162, 6.846443, 6.262918]

# Количество потоков
threads = [1, 2, 4, 8, 16]

# Вычисляем ускорение и эффективность для каждого варианта
def calculate_speedup_and_efficiency(T1, times):
    speedups = [T1 / t for t in times]
    efficiencies = [(s / p) * 100 for s, p in zip(speedups, threads)]
    return speedups, efficiencies

# Для первого варианта
var1_speedups, var1_efficiencies = calculate_speedup_and_efficiency(T1, var1_times)

# Для второго варианта
var2_speedups, var2_efficiencies = calculate_speedup_and_efficiency(T1, var2_times)

# Создаем графики
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 6))

# График времени
ax1.plot(threads, [T1] * len(threads), label="Sequential", color='r', marker='o')
ax1.plot(threads, var1_times, label="Var1", color='g', marker='s')
ax1.plot(threads, var2_times, label="Var2", color='b', marker='^')
ax1.set_xlabel("Number of Threads")
ax1.set_ylabel("Time (sec)")
ax1.set_title("время выполнения от кол-ва потоков")
ax1.legend()

# График ускорения
ax2.plot(threads, [T1 / t for t in var1_times], label="Var1", color='g', marker='s')
ax2.plot(threads, [T1 / t for t in var2_times], label="Var2", color='b', marker='^')
ax2.set_xlabel("Number of Threads")
ax2.set_ylabel("Speedup")
ax2.set_title("ускорение от кол-ва потоков")
ax2.legend()

# График эффективности
ax3.plot(threads, [s / p * 100 for s, p in zip(var1_speedups, threads)], label="Var1", color='g', marker='s')
ax3.plot(threads, [s / p * 100 for s, p in zip(var2_speedups, threads)], label="Var2", color='b', marker='^')
ax3.set_xlabel("Number of Threads")
ax3.set_ylabel("Efficiency (%)")
ax3.set_title("эффективность потоков ")
ax3.legend()

# Показать графики
plt.tight_layout()
plt.show()
