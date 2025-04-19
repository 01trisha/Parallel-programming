import matplotlib.pyplot as plt

# Время выполнения программы для разных чисел потоков (значения из задачи)
threads = [1, 2, 4, 8, 16]

# Время работы программы с omp_parallel
omp_parallel_times = [67.114254, 33.871753, 19.229057, 9.703501, 8.821988]

# Время работы программы с omp_parallel_for
omp_parallel_for_times = [68.392633, 33.878401, 19.211936, 9.737794, 8.764055]

# Время работы последовательной программы
T1 = 67.404712

# Создание графиков
plt.figure(figsize=(10, 12))

# График времени выполнения (новый график, первый по порядку)
plt.subplot(3, 1, 1)
plt.plot(threads, omp_parallel_times, label='omp_parallel', marker='o', color='b')
plt.plot(threads, omp_parallel_for_times, label='omp_parallel_for', marker='s', color='r')
plt.title('Зависимость времени выполнения от количества потоков')
plt.xlabel('Количество потоков')
plt.ylabel('Время выполнения (сек)')
plt.legend()

# График ускорения
plt.subplot(3, 1, 2)
sp_parallel = [T1 / time for time in omp_parallel_times]
sp_parallel_for = [T1 / time for time in omp_parallel_for_times]
plt.plot(threads, sp_parallel, label='omp_parallel', marker='o', color='b')
plt.plot(threads, sp_parallel_for, label='omp_parallel_for', marker='s', color='r')
plt.title('Ускорение от количества потоков')
plt.xlabel('Количество потоков')
plt.ylabel('Ускорение (Sp)')
plt.legend()

# График эффективности
plt.subplot(3, 1, 3)
ep_parallel = [sp / p * 100 for sp, p in zip(sp_parallel, threads)]
ep_parallel_for = [sp / p * 100 for sp, p in zip(sp_parallel_for, threads)]
plt.plot(threads, ep_parallel, label='omp_parallel', marker='o', color='b')
plt.plot(threads, ep_parallel_for, label='omp_parallel_for', marker='s', color='r')
plt.title('Эффективность от количества потоков')
plt.xlabel('Количество потоков')
plt.ylabel('Эффективность (Ep)')
plt.legend()

# Отображение графиков
plt.tight_layout()
plt.show()
