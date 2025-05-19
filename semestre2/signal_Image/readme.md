# 1. Структура программы
Программа состоит из нескольких функций, которые выполняют разные задачи:
Генерация сигнала (сумма гармоник)
Квантование сигнала (преобразование в цифровой вид)
Сохранение данных в файл (для визуализации)
Чтение параметров гармоник из файла
Главная функция main(), которая управляет всем процессом

# 2. Основные компоненты
# 2.1. Структура Harmonic
struct Harmonic {
    double amplitude;   // Амплитуда гармоники
    double frequency;   // Частота (в Гц)
    double phase_shift; // Фазовый сдвиг (в радианах)
};

Эта структура хранит параметры одной гармоники:
amplitude — "высота" волны
frequency — количество колебаний в секунду
phase_shift — сдвиг волны по времени

# 2.2. Генерация сигнала (generateSignal)
vector<double> generateSignal(const vector<Harmonic>& harmonics, 
                             double sampling_rate, 
                             double duration, 
                             int& num_points) {
    double delta_t = 1.0 / sampling_rate; // Шаг времени
    num_points = duration * sampling_rate; // Количество точек
    vector<double> signal(num_points, 0.0);

    for (int i = 0; i < num_points; ++i) {
        double t = i * delta_t; // Текущее время
        for (const auto& h : harmonics) {
            // Суммируем все гармоники
            signal[i] += h.amplitude * sin(2 * M_PI * h.frequency * t + h.phase_shift);
        }
    }
    return signal;
}

Что делает эта функция?

-Рассчитывает delta_t — временной шаг между отсчётами (1 / частота_дискретизации).
-Вычисляет общее количество точек (num_points = длительность * частота_дискретизации).
-Для каждой точки времени t вычисляет значение сигнала как сумму всех гармоник.

Пример:
Если есть гармоники 50 Гц, 150 Гц и 300 Гц, то сигнал будет их суммой.

# 2.3. Квантование сигнала (digitizeSignal)
vector<int> digitizeSignal(const vector<double>& signal, 
                          int bits, 
                          double max_amplitude) {
    int levels = (1 << bits) - 1; // Количество уровней квантования
    vector<int> digital_signal(signal.size());

    for (size_t i = 0; i < signal.size(); ++i) {
        // Нормализация сигнала в диапазон [0, 1]
        double normalized = (signal[i] + max_amplitude) / (2 * max_amplitude);
        // Преобразование в цифровое значение
        digital_signal[i] = round(normalized * levels);
    }
    return digital_signal;
}

Что делает эта функция?

-Определяет количество уровней квантования (levels = 2^bits - 1). Например, для 8 бит это 255 уровней.
-Нормализует аналоговый сигнал в диапазон [0, 1].
-Преобразует значение в ближайший целочисленный уровень (digital_signal[i]).

Пример:

Если сигнал имеет амплитуду 1.0, а max_amplitude = 2.0, то:
normalized = (1.0 + 2.0) / (2 * 2.0) = 0.75
При bits = 8 → levels = 255 → digital_value = 0.75 * 255 ≈ 191

# 2.4. Сохранение данных в файл (saveToFile)
void saveToFile(const string& filename, 
               const vector<double>& time, 
               const vector<double>& signal,
               const vector<int>& digital_signal) {
    ofstream out(filename);
    out << "Time,Analog,Digital\n";
    for (size_t i = 0; i < time.size(); ++i) {
        out << time[i] << "," << signal[i] << "," << digital_signal[i] << "\n";
    }
    out.close();
}

Что делает эта функция?
-Сохраняет данные в CSV-файл в формате

# 2.5. Чтение гармоник из файла (readHarmonicsFromFile)
vector<Harmonic> readHarmonicsFromFile(const string& filename) {
    ifstream in(filename);
    vector<Harmonic> harmonics;
    Harmonic h;
    while (in >> h.amplitude >> h.frequency >> h.phase_shift) {
        harmonics.push_back(h);
    }
    return harmonics;
}

Что делает эта функция?

-Читает из файла строки вида амплитуда частота фазовый_сдвиг.
-Возвращает вектор структур Harmonic.

# 2.6. Главная функция main()
int main() {
    // 1. Запрашиваем у пользователя способ ввода данных (файл или ручной)
    // 2. Получаем параметры гармоник
    // 3. Запрашиваем частоту дискретизации и длительность сигнала
    // 4. Генерируем сигнал
    // 5. Квантуем сигнал
    // 6. Сохраняем данные в CSV
    // 7. Генерируем Python-скрипт для визуализации
}

Что делает?

-Спрашивает, вводить ли данные вручную или из файла.
-Получает параметры гармоник.
-Запрашивает частоту дискретизации (например, 1000 Гц) и длительность сигнала (например, 0.1 сек).
-Генерирует аналоговый сигнал.
-Преобразует его в цифровой (например, 8-битный).
-Сохраняет данные в signal_data.csv.
-Создаёт Python-скрипт plot_signal.py для построения графиков.
