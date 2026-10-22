#include "../include/arraylib.h" // Так VS Code найдет файл
#include <iostream>

int main() {
    int data[] = {5, 3, 8, 1, 9, 2}; // Убрали [0]=, [1]= и т.д.
    const std::size_t n = sizeof(data) / sizeof(data[0]);

    std::cout << "Sum: " << arr_sum(data, n) << '\n';
    std::cout << "Max: " << arr_max(data, n) << '\n';
    std::cout << "Min: " << arr_min(data, n) << '\n';
    std::cout << "Average: " << arr_average(data, n) << '\n';

    return 0;
}