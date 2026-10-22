// Задание 1.
// Сборка: c++ -std=c++17 -O3 -DNDEBUG first.cpp -o search_demo
// Быстрая проверка: ./search_demo --size 100000 --queries 1000 --sample-ms 50
// Основной опыт: ./search_demo --size 1000000 --queries 10000 --sample-ms 250
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using Id = std::uint64_t;

// Реализуйте только эту функцию. Готовая часть находится ниже.
bool contains(const std::vector<Id>& ids, Id target) {
    bool contains(const std::vector<Id>& ids, Id target) {
    return std::find(ids.begin(), ids.end(), target) != ids.end();
}
    return false;
}

// ------------------------------------------------------------------
// Готовая часть: данные, проверки и измерения. Дописывать её не нужно.
// ------------------------------------------------------------------

using Clock = std::chrono::steady_clock;

struct Query {
    Id id;
    bool expected;
};

struct Options {
    std::size_t size = 1'000'000;
    std::size_t queries = 10'000;
    unsigned sample_ms = 250;
    Id seed = 20260929;
};

void fail(const std::string& message) {
    std::cerr << "Error: " << message << '\n';
    std::exit(1);
}

Id parse_number(const char* text) {
    const std::string number(text);
    if (number.empty()) fail("empty number");
    Id value = 0;
    for (std::size_t i = 0; i < number.size(); ++i) {
        if (number[i] < '0' || number[i] > '9')
            fail("invalid number: " + number);
        const unsigned digit = number[i] - '0';
        if (value > (UINT64_MAX - digit) / 10)
            fail("number is too large: " + number);
        value = value * 10 + digit;
    }
    return value;
}

Options parse_options(int argc, char** argv) {
    Options options;
    for (int i = 1; i < argc; i += 2) {
        const std::string arg(argv[i]);
        if (arg == "--help") {
            std::cout << "Usage: " << argv[0]
                      << " [--size N] [--queries N] [--sample-ms N] [--seed N]\n";
            std::exit(0);
        }
        if (i + 1 >= argc) fail("missing value after " + arg);
        const Id value = parse_number(argv[i + 1]);
        if (arg == "--size" || arg == "--queries") {
            if (value > SIZE_MAX) fail("size or query count is too large");
            if (arg == "--size") options.size = value;
            else options.queries = value;
        } else if (arg == "--sample-ms") {
            if (value > UINT_MAX) fail("sample duration is too large");
            options.sample_ms = value;
        } else if (arg == "--seed") {
            options.seed = value;
        } else {
            fail("unknown option: " + arg);
        }
    }
    if (options.queries < 2 || options.queries % 2 != 0 ||
        options.size < options.queries / 2 || options.sample_ms == 0)
        fail("need even queries >= 2, size >= queries/2, sample-ms > 0");
    return options;
}

std::vector<Id> make_ids(std::size_t size, std::mt19937_64& random) {
    // Воспроизводимый набор уникальных ID. Формула — часть готовой оснастки.
    const Id mask = (Id(1) << 63) - 1;
    const Id multiplier = random() | Id(1);
    const Id offset = random();
    std::vector<Id> ids;
    ids.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
        ids.push_back(((Id(i) * multiplier + offset) & mask) << 1);
    std::shuffle(ids.begin(), ids.end(), random);
    return ids;
}

std::vector<Query> make_queries(const std::vector<Id>& ids, std::size_t count,
                                std::mt19937_64& random) {
    std::vector<Query> queries;
    queries.reserve(count);
    std::vector<char> chosen(ids.size(), 0);
    std::uniform_int_distribution<std::size_t> choose(0, ids.size() - 1);
    std::size_t present = 0;
    while (present < count / 2) {
        const std::size_t position = choose(random);
        if (chosen[position]) continue;
        chosen[position] = 1;
        ++present;
        queries.push_back({ids[position], true});
        queries.push_back({random() | Id(1), false});
    }
    std::shuffle(queries.begin(), queries.end(), random);
    return queries;
}

double milliseconds(Clock::duration duration) {
    return std::chrono::duration<double, std::milli>(duration).count();
}

void verify(const std::vector<Id>& data, const std::vector<Query>& queries) {
    for (std::size_t i = 0; i < queries.size(); ++i) {
        if (contains(data, queries[i].id) != queries[i].expected)
            fail("wrong answer at query " + std::to_string(i));
    }
}

// Счётчик выводится в конце, чтобы результаты поиска использовались программой.
Id timed_matches = 0;

double time_search(const std::vector<Id>& data, const std::vector<Query>& queries,
                   unsigned sample_ms) {
    std::vector<double> samples(5);
    for (std::size_t sample = 0; sample < samples.size(); ++sample) {
        Id found = 0;
        std::size_t passes = 0;
        const Clock::time_point start = Clock::now();
        do {
            for (std::size_t i = 0; i < queries.size(); ++i)
                found += contains(data, queries[i].id);
            ++passes;
        } while (milliseconds(Clock::now() - start) < sample_ms);
        const Clock::time_point end = Clock::now();
        timed_matches += found;
        samples[sample] = milliseconds(end - start) / passes;
    }
    std::sort(samples.begin(), samples.end());
    return samples[2];
}

void print_results(const std::string& title, const Options& options,
                   double build_ms, double batch_ms, double data_mib) {
    std::cout << title << '\n'
              << "IDs: " << options.size << ", queries: " << options.queries
              << " (" << options.queries / 2 << " present, "
              << options.queries / 2 << " absent), seed: " << options.seed << '\n'
              << "Correctness: all queries passed\n"
              << std::fixed << std::setprecision(3)
              << "Build ms: " << build_ms << '\n'
              << "Batch ms: " << batch_ms << '\n'
              << "Total ms: " << build_ms + batch_ms << '\n';
    if (data_mib >= 0)
        std::cout << "Data MiB: " << data_mib << '\n';
    else
        std::cout << "Data MiB: not measured\n";
    std::cout << "Timing: median of five samples, each at least "
              << options.sample_ms << " ms and one complete query batch.\n"
              << "Timed matches: " << timed_matches << '\n'
              << "Data MiB is structure storage, not process peak RSS.\n";
}

int main(int argc, char** argv) {
    const Options options = parse_options(argc, argv);

    const std::vector<Id> empty;
    const std::vector<Id> small{9, 2, 7, 4};
    const std::vector<Id> single{9};
    if (contains(empty, 2) || !contains(small, 9) ||
        !contains(small, 4) || contains(small, 5) ||
        !contains(single, 9) || contains(single, 2))
        fail("contains failed an edge case; complete the function at the top");

    std::mt19937_64 random(options.seed);
    std::vector<Id> ids = make_ids(options.size, random);
    const std::vector<Query> queries = make_queries(ids, options.queries, random);

    const double build_ms = 0;
    const double data_mib = ids.capacity() * sizeof(Id) / 1048576.0;
    verify(ids, queries);
    const double batch_ms = time_search(ids, queries, options.sample_ms);
    print_results("First experiment", options, build_ms, batch_ms, data_mib);
    return 0;
}
