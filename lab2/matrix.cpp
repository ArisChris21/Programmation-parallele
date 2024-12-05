#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iomanip>

template <typename T = double>
class MatrixDense {
private:
    unsigned _m, _n;
    T* data;

public:
    MatrixDense(unsigned m, unsigned n)
        : _m(m), _n(n), data(new T[m * n]{}) {}

    ~MatrixDense() {
        delete[] data;
    }

    T& operator()(unsigned i, unsigned j) {
        if (i >= _m || j >= _n) 
            throw std::out_of_range("Index out of bounds");
        return data[i * _n + j];
    }

    const T& operator()(unsigned i, unsigned j) const {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Index out of bounds");
        return data[i * _n + j];
    }

    // Плюс
    MatrixDense operator+(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions must match for addition");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    // Минус
    MatrixDense operator-(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    // Поэлементное умножение
    MatrixDense elementwise_multiply(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions must match for elementwise multiplication");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] * other.data[i];
        return result;
    }

    // Матричное умножение
    MatrixDense operator*(const MatrixDense& other) const {
        if (_n != other._m)
            throw std::invalid_argument("Matrix dimensions must match for multiplication");
        MatrixDense result(_m, other._n);
        for (unsigned i = 0; i < _m; ++i)
            for (unsigned j = 0; j < other._n; ++j)
                for (unsigned k = 0; k < _n; ++k)
                    result(i, j) += (*this)(i, k) * other(k, j);
        return result;
    }

    // Транспонирование
    MatrixDense transpose() const {
        MatrixDense result(_n, _m);
        for (unsigned i = 0; i < _m; ++i)
            for (unsigned j = 0; j < _n; ++j)
                result(j, i) = (*this)(i, j);
        return result;
    }

    // Импорт из файла
    static MatrixDense importFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file");

        std::string className;
        file >> className;
        if (className != "MatrixDense")
            throw std::invalid_argument("Unsupported class type in file");

        unsigned rows, cols;
        file >> rows >> cols;

        MatrixDense result(rows, cols);
        for (unsigned i = 0; i < rows; ++i)
            for (unsigned j = 0; j < cols; ++j)
                file >> result(i, j);

        return result;
    }

    // Экспорт в файл
    void exportToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file");

        file << "MatrixDense\n" << _m << " " << _n << "\n";
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j)
                file << (*this)(i, j) << " ";
            file << "\n";
        }
    }

    // Вывод матрицы
    void print() const {
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j)
                std::cout << std::setw(5) << (*this)(i, j) << " ";
            std::cout << "\n";
        }
    }
};

// Реализация остальных классов (MatrixDiagonal, MatrixBlock) может быть выполнена аналогичным образом.

int main() {
    try {
        // Создание двух плотных матриц
        MatrixDense<double> A(2, 3);
        MatrixDense<double> B(2, 3);

        // Заполнение матриц
        A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
        A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;

        B(0, 0) = 6; B(0, 1) = 5; B(0, 2) = 4;
        B(1, 0) = 3; B(1, 1) = 2; B(1, 2) = 1;

        std::cout << "Matrix A:\n";
        A.print();

        std::cout << "\nMatrix B:\n";
        B.print();

        // Сложение матриц
        MatrixDense<double> C = A + B;
        std::cout << "\nA + B:\n";
        C.print();

        // Вычитание матриц
        MatrixDense<double> D = A - B;
        std::cout << "\nA - B:\n";
        D.print();

        // Поэлементное умножение
        MatrixDense<double> E = A.elementwise_multiply(B);
        std::cout << "\nA .* B (elementwise multiplication):\n";
        E.print();

        // Транспонирование матрицы
        MatrixDense<double> At = A.transpose();
        std::cout << "\nTranspose of A:\n";
        At.print();

        // Сохранение матрицы в файл
        std::string filename = "matrix_output.txt";
        A.exportToFile(filename);
        std::cout << "\nMatrix A saved to " << filename << "\n";

        // Загрузка матрицы из файла
        MatrixDense<double> F = MatrixDense<double>::importFromFile(filename);
        std::cout << "\nMatrix loaded from file:\n";
        F.print();

        // Проверка матричного умножения
        MatrixDense<double> G(3, 2);
        G(0, 0) = 1; G(0, 1) = 2;
        G(1, 0) = 3; G(1, 1) = 4;
        G(2, 0) = 5; G(2, 1) = 6;

        std::cout << "\nMatrix G:\n";
        G.print();

        MatrixDense<double> H = A * G;
        std::cout << "\nA * G (matrix multiplication):\n";
        H.print();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

