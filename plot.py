import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("genealogy_sorting_times.csv")

plt.figure(figsize=(12, 7))
plt.plot(df["Size"], df["Bubble"], label="Bubble Sort", marker='o')
plt.plot(df["Size"], df["Shaker"], label="Shaker Sort", marker='s')
plt.plot(df["Size"], df["Heap"], label="Heap Sort", marker='^')
plt.plot(df["Size"], df["Std"], label="std::sort", marker='x')

plt.title("Сравнение времени сортировки (в мс)")
plt.xlabel("Размер входных данных")
plt.ylabel("Время, мс")
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig("sort_comparison_plot.png", dpi=300)
plt.show()
