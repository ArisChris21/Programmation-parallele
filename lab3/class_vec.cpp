#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <random>
#include <chrono>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <mutex>

template<typename T>
class Vector {
private:
    T* data;
    size_t n;
    bool is_initialized;
    std::mutex mtx;

public:
    // Constructor
    explicit Vector(size_t size) : n(size), is_initialized(false) {
        data = new T[size]();
    }

    // Destructor
    ~Vector() {
        delete[] data;
    }

    // Initialization methods
    void initialize_with_constant(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < n; ++i) {
            data[i] = value;
        }
        is_initialized = true;
    }

    void initialize_with_random(T min_value, T max_value) {
        std::lock_guard<std::mutex> lock(mtx);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dist(min_value, max_value);
        for (size_t i = 0; i < n; ++i) {
            data[i] = dist(gen);
        }
        is_initialized = true;
    }

    // State check
    void check_initialized() const {
        if (!is_initialized) {
            throw std::runtime_error("Vector is not initialized.");
        }
    }

    // Export to file
    void export_to_file(const std::string& filename) {
        check_initialized();
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing.");
        }
        for (size_t i = 0; i < n; ++i) {
            file << data[i] << " ";
        }
        file.close();
    }

    // Import from file
    void import_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading.");
        }
        for (size_t i = 0; i < n && file >> data[i]; ++i) {
        }
        is_initialized = true;
        file.close();
    }

    // Utility functions with timing
    std::pair<T, double> find_min() {
        check_initialized();
        auto start = std::chrono::high_resolution_clock::now();
        T min_val = std::numeric_limits<T>::max();
        for (size_t i = 0; i < n; ++i) {
            min_val = std::min(min_val, data[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return {min_val, std::chrono::duration<double>(end - start).count()};
    }

    std::pair<T, double> find_max() {
        check_initialized();
        auto start = std::chrono::high_resolution_clock::now();
        T max_val = std::numeric_limits<T>::lowest();
        for (size_t i = 0; i < n; ++i) {
            max_val = std::max(max_val, data[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return {max_val, std::chrono::duration<double>(end - start).count()};
    }

    // Parallel functions
    T parallel_find_min(size_t num_threads) {
        check_initialized();
        std::vector<std::thread> threads;
        std::vector<T> mins(num_threads, std::numeric_limits<T>::max());

        auto task = [&](size_t thread_id, size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) {
                mins[thread_id] = std::min(mins[thread_id], data[i]);
            }
        };

        size_t block_size = n / num_threads;
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * block_size;
            size_t end = (i == num_threads - 1) ? n : start + block_size;
            threads.emplace_back(task, i, start, end);
        }

        for (auto& t : threads) {
            t.join();
        }

        return *std::min_element(mins.begin(), mins.end());
    }

    // Additional utility methods can be added here following the same pattern.

    // Print results
    void print_results(const std::string& description, T result, double time) {
        std::cout << description << ": " << result << ", Time: " << time << " seconds\n";
    }
};

int main() {
    size_t vector_size = 1000000; // Adjust for testing
    Vector<double> vec(vector_size);

    // Initialize with random values
    vec.initialize_with_random(-100.0, 100.0);

    // Find min
    auto [min_val, min_time] = vec.find_min();
    vec.print_results("Minimum value", min_val, min_time);

    // Parallel find min
    double parallel_min = vec.parallel_find_min(4); // Using 4 threads
    std::cout << "Parallel minimum value: " << parallel_min << "\n";

    return 0;
}
