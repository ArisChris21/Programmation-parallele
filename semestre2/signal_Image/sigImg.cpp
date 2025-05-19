#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

#define M_PI 3.14

using namespace std;

// Структура для хранения параметров гармоники
struct Harmonic {
    double amplitude;
    double frequency;
    double phase_shift;
};

// Функция для генерации сигнала
vector<double> generateSignal(const vector<Harmonic>& harmonics,
    double sampling_rate,
    double duration,
    int& num_points) {
    double delta_t = 1.0 / sampling_rate;
    num_points = static_cast<int>(duration * sampling_rate);
    vector<double> signal(num_points, 0.0);

    for (int i = 0; i < num_points; ++i) {
        double t = i * delta_t;
        for (const auto& h : harmonics) {
            signal[i] += h.amplitude * sin(2 * M_PI * h.frequency * t + h.phase_shift);
        }
    }

    return signal;
}

// Функция для квантования сигнала (преобразование в цифровой)
vector<int> digitizeSignal(const vector<double>& signal,
    int bits,
    double max_amplitude) {
    int levels = (1 << bits) - 1;
    vector<int> digital_signal(signal.size());

    for (size_t i = 0; i < signal.size(); ++i) {
        double normalized = (signal[i] + max_amplitude) / (2 * max_amplitude);
        digital_signal[i] = static_cast<int>(round(normalized * levels));
    }

    return digital_signal;
}

// Функция для сохранения данных в файл
void saveToFile(const string& filename,
    const vector<double>& time,
    const vector<double>& signal,
    const vector<int>& digital_signal) {
    ofstream out(filename);
    if (!out) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }

    out << "Time,Analog,Digital\n";
    for (size_t i = 0; i < time.size(); ++i) {
        out << time[i] << "," << signal[i] << "," << digital_signal[i] << "\n";
    }

    out.close();
    cout << "Data saved to " << filename << endl;
}

// Функция для чтения гармоник из файла
vector<Harmonic> readHarmonicsFromFile(const string& filename) {
    ifstream in(filename);
    vector<Harmonic> harmonics;

    if (!in) {
        cerr << "Cannot open file: " << filename << endl;
        return harmonics;
    }

    Harmonic h;
    while (in >> h.amplitude >> h.frequency >> h.phase_shift) {
        harmonics.push_back(h);
    }

    in.close();
    return harmonics;
}

int main() {
    vector<Harmonic> harmonics;
    char choice;

    cout << "Генератор несинусоидального сигнала\n";
    cout << "Вы хотите ввести параметры вручную (m) или загрузить из файла (f)? ";
    cin >> choice;

    if (tolower(choice) == 'f') {
        string filename;
        cout << "Введите имя файла с данными гармоник: ";
        cin >> filename;
        harmonics = readHarmonicsFromFile(filename);
    }
    else {
        int num_harmonics;
        cout << "Введите количество гармоник: ";
        cin >> num_harmonics;

        harmonics.resize(num_harmonics);
        for (int i = 0; i < num_harmonics; ++i) {
            cout << "Гармоника #" << i + 1 << ":\n";
            cout << "  Амплитуда: ";
            cin >> harmonics[i].amplitude;
            cout << "  Частота (Гц): ";
            cin >> harmonics[i].frequency;
            cout << "  Начальный сдвиг фазы (рад): ";
            cin >> harmonics[i].phase_shift;
        }
    }

    double sampling_rate, duration;
    cout << "Введите частоту дискретизации (Гц): ";
    cin >> sampling_rate;
    cout << "Введите длительность сигнала (с): ";
    cin >> duration;

    int num_points;
    vector<double> signal = generateSignal(harmonics, sampling_rate, duration, num_points);

    // Создаем массив времени
    vector<double> time(num_points);
    double delta_t = 1.0 / sampling_rate;
    for (int i = 0; i < num_points; ++i) {
        time[i] = i * delta_t;
    }

    // Находим максимальную амплитуду для квантования
    double max_amplitude = 0;
    for (const auto& h : harmonics) {
        max_amplitude += h.amplitude;
    }

    int bits;
    cout << "Введите разрядность цифрового сигнала (бит): ";
    cin >> bits;

    vector<int> digital_signal = digitizeSignal(signal, bits, max_amplitude);

    // Сохраняем данные для визуализации в Python
    saveToFile("signal_data.csv", time, signal, digital_signal);

    // Генерируем Python скрипт для визуализации
    ofstream py_script("plot_signal.py");
    py_script << "import pandas as pd\n"
        << "import matplotlib.pyplot as plt\n\n"
        << "data = pd.read_csv('signal_data.csv')\n\n"
        << "plt.figure(figsize=(12, 6))\n"
        << "plt.subplot(2, 1, 1)\n"
        << "plt.plot(data['Time'], data['Analog'], label='Analog Signal')\n"
        << "plt.title('Analog Signal')\n"
        << "plt.xlabel('Time (s)')\n"
        << "plt.ylabel('Amplitude')\n"
        << "plt.grid(True)\n\n"
        << "plt.subplot(2, 1, 2)\n"
        << "plt.stem(data['Time'], data['Digital'], linefmt='b-', markerfmt='bo', basefmt='r-', label='Digital Signal')\n"
        << "plt.title('Digital Signal')\n"
        << "plt.xlabel('Time (s)')\n"
        << "plt.ylabel('Digital Value')\n"
        << "plt.grid(True)\n\n"
        << "plt.tight_layout()\n"
        << "plt.savefig('signal_plot.png')\n"
        << "plt.show()\n";

    py_script.close();

    cout << "Для визуализации выполните: python plot_signal.py\n";

    return 0;
}
