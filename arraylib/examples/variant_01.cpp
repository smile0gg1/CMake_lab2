#include "../include/arraylib.h"
#include <cstddef>
#include <iomanip>
#include <iostream>

int main() {
    // Массив температур за месяц (Вариант 1)
    int temperature[] = {
        -8, -5, -3, 0, 2, 1, -1,
        -4, -6, -2, 0, 3, 5, 4,
        2, 1, -2, -7, -9, -4, 0,
        3, 6, 5, 2, -1, -3, -5,
        1, 4
    };
    const std::size_t n = sizeof(temperature) / sizeof(temperature[0]);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Вариант 1. Температура воздуха ===\n\n";

    // Вывод результатов (используем функции из arraylib)
    std::cout << "Средняя температура:  " << arr_average(temperature, n) << " C\n";
    std::cout << "Максимальная темп.:   " << arr_max(temperature, n) << " C\n";
    std::cout << "Минимальная темп.:    " << arr_min(temperature, n) << " C\n";
    
    // Функции для количества отрицательных (морозных) дней
    std::cout << "Количество морозных дней: " << arr_count_negative(temperature, n) << "\n";
    
    std::cout << "Медиана температур:   " << arr_median(temperature, n) << " C\n";

    return 0;
}