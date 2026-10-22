#include "../include/arraylib.h"
#include <cstddef>
#include <iomanip>
#include <iostream>

int main() {
    int receipts[] = { 850, 1240, 430, 2190, 760, 1580, 990, 3100 };
    const std::size_t n = sizeof(receipts) / sizeof(receipts[0]);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Вариант 17. Суммы чеков ===\n\n";

    std::cout << "Общая сумма покупок: " << arr_sum(receipts, n) << " руб.\n";
    std::cout << "Самый дорогой чек:   " << arr_max(receipts, n) << " руб.\n";
    std::cout << "Самый дешёвый чек:   " << arr_min(receipts, n) << " руб.\n";
    std::cout << "Средний чек:         " << arr_average(receipts, n) << " руб.\n";
    std::cout << "Медиана чеков:       " << arr_median(receipts, n) << " руб.\n";

    return 0;
}