/**
 * @file lab1.cpp
 * @brief Реализация и сравнение алгоритмов сортировки объектов GenealogyPerson.
 *
 * В данной программе реализованы и сравниваются по времени работы следующие алгоритмы сортировки:
 * - Пузырьковая сортировка (bubbleSort)
 * - Шейкер-сортировка (shakerSort)
 * - Пирамидальная сортировка (heapSort)
 * - Стандартная сортировка STL (std::sort)
 * 
 * Все сортировки применяются к структурам GenealogyPerson, содержащим данные о людях:
 * имя, год рождения, год смерти и количество детей. Результаты сортировки и время выполнения
 * сохраняются в CSV-файлы.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>

/**
 * @struct GenealogyPerson
 * @brief Структура, описывающая человека из генеалогического дерева.
 * 
 * Используется для хранения и сортировки данных по заданным критериям.
 */
struct GenealogyPerson {
    std::string fullName;     /**< Полное имя человека */
    int birthYear;            /**< Год рождения */
    int deathYear;            /**< Год смерти */
    int childrenCount;        /**< Количество детей */

    /**
     * @brief Оператор < для сортировки.
     * @param other Другой объект для сравнения.
     * @return true, если текущий объект "меньше" другого по правилам сортировки.
     *
     * Приоритет сортировки: по году рождения -> имени -> количеству детей.
     */
    bool operator<(const GenealogyPerson &other) const {
        if (birthYear != other.birthYear)
            return birthYear < other.birthYear;
        if (fullName != other.fullName)
            return fullName < other.fullName;
        return childrenCount < other.childrenCount;
    }

    /// Оператор > (обратный <)
    bool operator>(const GenealogyPerson &other) const {
        return other < *this;
    }

    /// Оператор <= (не больше другого)
    bool operator<=(const GenealogyPerson &other) const {
        return !(other < *this);
    }

    /// Оператор >= (не меньше другого)
    bool operator>=(const GenealogyPerson &other) const {
        return !(*this < other);
    }
};

/**
 * @brief Пузырьковая сортировка.
 * @tparam T Тип сортируемых объектов.
 * @param arr Вектор для сортировки.
 */
template <typename T>
void bubbleSort(std::vector<T> &arr) {
    size_t n = arr.size();
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 1; i < n; ++i) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                swapped = true;
            }
        }
        --n;
    } while (swapped);
}

/**
 * @brief Шейкер-сортировка (двунаправленная версия пузырьковой).
 * @tparam T Тип сортируемых объектов.
 * @param arr Вектор для сортировки.
 */
template <typename T>
void shakerSort(std::vector<T> &arr) {
    size_t left = 0;
    size_t right = arr.size() - 1;
    while (left < right) {
        for (size_t i = left; i < right; ++i) {
            if (arr[i] > arr[i + 1])
                std::swap(arr[i], arr[i + 1]);
        }
        --right;
        for (size_t i = right; i > left; --i) {
            if (arr[i] < arr[i - 1])
                std::swap(arr[i], arr[i - 1]);
        }
        ++left;
    }
}

/**
 * @brief Просеивание в пирамидальной сортировке.
 * @tparam T Тип сортируемых объектов.
 * @param arr Вектор для сортировки.
 * @param n Размер кучи.
 * @param i Индекс текущего узла.
 */
template <typename T>
void heapify(std::vector<T> &arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest])
        largest = l;
    if (r < n && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}



/**
 * @brief Пирамидальная сортировка (HeapSort).
 * @tparam T Тип сортируемых объектов.
 * @param arr Вектор для сортировки.
 */
template <typename T>
void heapSort(std::vector<T> &arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

/**
 * @brief Генерация случайного объекта GenealogyPerson.
 * @return Случайно сгенерированный человек.
 */
GenealogyPerson generateRandomPerson() {
    static const std::string names[] = {
        "Иванов Иван", "Петров Петр", "Сидоров Сидор", "Кузнецова Анна", "Морозова Мария"
    };
    GenealogyPerson p;
    p.fullName = names[rand() % 5];
    p.birthYear = 1800 + rand() % 221;
    p.deathYear = p.birthYear + 30 + rand() % 60;
    p.childrenCount = rand() % 10;
    return p;
}

/**
 * @brief Генерация массива людей.
 * @param size Размер массива.
 * @return Вектор GenealogyPerson.
 */
std::vector<GenealogyPerson> generatePeople(size_t size) {
    std::vector<GenealogyPerson> people;
    people.reserve(size);
    for (size_t i = 0; i < size; ++i)
        people.push_back(generateRandomPerson());
    return people;
}

/**
 * @brief Замер времени выполнения сортировки.
 * @tparam Func Тип функции сортировки.
 * @param sortFunction Сортирующая функция.
 * @param data Копия массива данных для сортировки.
 * @return Время в миллисекундах.
 */
template <typename Func>
double measureSortTime(Func sortFunction, std::vector<GenealogyPerson> &data) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    return diff.count();
}

/**
 * @brief Сохраняет данные в CSV-файл.
 * @param people Отсортированный список людей.
 * @param filename Имя выходного файла.
 */
void writePeopleToCSV(const std::vector<GenealogyPerson> &people, const std::string &filename) {
    std::ofstream file(filename);
    file << "FullName,BirthYear,DeathYear,ChildrenCount\n";
    for (const auto &p : people) {
        file << p.fullName << "," << p.birthYear << "," << p.deathYear << "," << p.childrenCount << "\n";
    }
    file.close();
}

/**
 * @brief Главная функция. Генерация, сортировка и анализ времени сортировок.
 * @return Код завершения.
 */
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<size_t> sizes = {
        100, 500, 1000, 2000, 3000, 5000, 7500,
        10000, 15000, 20000, 25000, 30000, 35000, 40000,
        50000
    };

    std::ofstream timingFile("genealogy_sorting_times.csv");
    timingFile << "Size,Bubble,Shaker,Heap,Std\n";

    for (size_t size : sizes) {
        std::vector<GenealogyPerson> original = generatePeople(size);

        auto copy1 = original;
        auto copy2 = original;
        auto copy3 = original;
        auto copy4 = original;

        double tBubble = measureSortTime(bubbleSort<GenealogyPerson>, copy1);
        double tShaker = measureSortTime(shakerSort<GenealogyPerson>, copy2);
        double tHeap = measureSortTime(heapSort<GenealogyPerson>, copy3);
        double tStd = measureSortTime([](auto &v){ std::sort(v.begin(), v.end()); }, copy4);

        timingFile << size << "," << tBubble << "," << tShaker << "," << tHeap << "," << tStd << "\n";

        std::cout 
<< "Размер: " << size
                  << " | Bubble: " << tBubble << " ms"
                  << " | Shaker: " << tShaker << " ms"
                  << " | Heap: " << tHeap << " ms"
                  << " | StdSort: " << tStd << " ms\n";
    }

    timingFile.close();

    auto finalData = generatePeople(1000);
    std::sort(finalData.begin(), finalData.end());
    writePeopleToCSV(finalData, "sorted_genealogy_output.csv");

    std::cout << "Готово. Данные сохранены в CSV." << std::endl;
    return 0;
}
