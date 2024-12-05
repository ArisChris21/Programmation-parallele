#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Классы для характеристик оборудования
class GpuSpec {
public:
    std::string model;
    int memorySize; // в MB
    int coreClock;  // в MHz

    void Print() const {
        std::cout << "GPU Model: " << model << ", Memory: " << memorySize 
                  << "MB, Core Clock: " << coreClock << "MHz\n";
    }

    void Export(std::ofstream& ofs) const {
        ofs << model << " " << memorySize << " " << coreClock << "\n";
    }

    void Import(std::ifstream& ifs) {
        ifs >> model >> memorySize >> coreClock;
    }
};

class CpuSpec {
public:
    std::string model;
    int cores;
    double clockSpeed; // в GHz

    void Print() const {
        std::cout << "CPU Model: " << model << ", Cores: " << cores 
                  << ", Clock Speed: " << clockSpeed << "GHz\n";
    }

    void Export(std::ofstream& ofs) const {
        ofs << model << " " << cores << " " << clockSpeed << "\n";
    }

    void Import(std::ifstream& ifs) {
        ifs >> model >> cores >> clockSpeed;
    }
};

class RamSpec {
public:
    int capacity;  // в MB
    int frequency; // в MHz

    void Print() const {
        std::cout << "RAM Capacity: " << capacity << "MB, Frequency: " << frequency << "MHz\n";
    }

    void Export(std::ofstream& ofs) const {
        ofs << capacity << " " << frequency << "\n";
    }

    void Import(std::ifstream& ifs) {
        ifs >> capacity >> frequency;
    }
};

class LanSpec {
public:
    int speed;       // в Mbps
    std::string type; // Ethernet, Fiber, и т.д.

    void Print() const {
        std::cout << "LAN Speed: " << speed << "Mbps, Type: " << type << "\n";
    }

    void Export(std::ofstream& ofs) const {
        ofs << speed << " " << type << "\n";
    }

    void Import(std::ifstream& ifs) {
        ifs >> speed >> type;
    }
};

// Класс ClusterNode
class ClusterNode {
public:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;

    void Print() const {
        std::cout << "Cluster Node:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Export(std::ofstream& ofs) const {
        gpu.Export(ofs);
        cpu.Export(ofs);
        ram.Export(ofs);
        lan.Export(ofs);
    }

    void Import(std::ifstream& ifs) {
        gpu.Import(ifs);
        cpu.Import(ifs);
        ram.Import(ifs);
        lan.Import(ifs);
    }
};

// Класс Cluster
class Cluster {
public:
    std::vector<ClusterNode> nodes;

    void AddNode(const ClusterNode& node) {
        nodes.push_back(node);
    }

    void Print() const {
        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << "Node " << i + 1 << ":\n";
            nodes[i].Print();
            std::cout << "----------------------------------\n";
        }
    }

    void Export(const std::string& filename) const {
        std::ofstream ofs(filename);
        if (ofs.is_open()) {
            ofs << nodes.size() << "\n";
            for (const auto& node : nodes) {
                node.Export(ofs);
            }
        }
    }

    void Import(const std::string& filename) {
        std::ifstream ifs(filename);
        if (ifs.is_open()) {
            size_t nodeCount;
            ifs >> nodeCount;
            nodes.resize(nodeCount);
            for (auto& node : nodes) {
                node.Import(ifs);
            }
        }
    }
};

// Функция main() для демонстрации
int main() {
    Cluster cluster;

    // Создание первого узла
    ClusterNode node1;
    node1.gpu = { "RTX2080", 8192, 1600 };
    node1.cpu = { "AMD Ryzen 7", 8, 3.4 };
    node1.ram = { 8192, 2400 };
    node1.lan = { 1000, "Ethernet" };

    // Создание второго узла
    ClusterNode node2;
    node2.gpu = { "RTX3090", 24576, 1900 };
    node2.cpu = { "Intel i7", 8, 3.8 };
    node2.ram = { 32768, 3200 };
    node2.lan = { 10000, "Fiber" };

    // Добавление узлов в кластер
    cluster.AddNode(node1);
    cluster.AddNode(node2);

    // Вывод кластера в консоль
    std::cout << "Cluster data:\n";
    cluster.Print();

    // Сохранение кластера в файл
    std::string filename = "cluster_data.txt";
    cluster.Export(filename);
    std::cout << "\nCluster exported to " << filename << "\n";

    // Импорт данных кластера из файла
    Cluster importedCluster;
    importedCluster.Import(filename);
    std::cout << "\nImported cluster data:\n";
    importedCluster.Print();

    return 0;
}
