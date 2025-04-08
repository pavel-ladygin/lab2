#include <iostream>
#include <thread>
#include <queue>
#include <random>
#include <semaphore>
#include <chrono>
#include <vector>
#include <mutex>
#include <atomic>

struct EnergyData {
    int station_id;
    int priority; 
    bool isCritical; 
    int data_size;
};

// Оператор сравнения для очереди с приоритетами
struct CompareEnergyPriority {
    bool operator()(const EnergyData& d1, const EnergyData& d2) {
        if (d1.priority != d2.priority) {
            return d1.priority > d2.priority;
        }
        return !d1.isCritical && d2.isCritical;
    }
};

std::priority_queue<EnergyData, std::vector<EnergyData>, CompareEnergyPriority> energy_queue;
std::counting_semaphore<5> server_handlers(5);
std::mutex output_mutex;
std::atomic<int> total_load(0);
std::atomic<int> active_handlers(5);
const int MAX_LOAD = 100;

// Функция обработки данных энергосети
void process_energy_data(EnergyData data) {
    server_handlers.acquire();
    
    int new_load = total_load += data.data_size;
    
    {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "Processing data from station " << data.station_id 
                  << " (priority " << data.priority 
                  << ", " << (data.isCritical ? "critical" : "normal") 
                  << "). Load: " << new_load << "%\n";
    }
    
    // Симуляция обработки данных (время зависит от размера данных)
    std::this_thread::sleep_for(std::chrono::milliseconds(data.data_size * 100));
    
    if (new_load > 80 && active_handlers < 10) {
        active_handlers++;
        server_handlers.release(); 
        
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "!!! High load detected (" << new_load << "%). Adding new handler. Total handlers: " 
                  << active_handlers << "\n";
    }
    
    if (!data.isCritical && new_load > 90) {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "High load (" << new_load << "%). Discarding non-critical data from station " 
                  << data.station_id << "\n";
        
        total_load -= data.data_size;
        server_handlers.release();
        return;
    }
    
    {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "Completed processing data from station " << data.station_id << "\n";
    }
    
    total_load -= data.data_size;
    server_handlers.release();
}

// Функция для добавления данных в очередь
void add_energy_data(int station_id, int priority, bool isCritical) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);
    
    EnergyData data = {station_id, priority, isCritical, dis(gen)};
    energy_queue.push(data);
    
    std::lock_guard<std::mutex> guard(output_mutex);
    std::cout << "Data from station " << station_id << " (priority " << priority 
              << ", " << (isCritical ? "critical" : "normal") 
              << ") added to queue. Size: " << data.data_size << "\n";
}

// Функция обработки данных из очереди
void process_energy_queue() {
    while (!energy_queue.empty()) {
        EnergyData data = energy_queue.top();
        energy_queue.pop();
        
        process_energy_data(data);
    }
}

int main() {
    add_energy_data(1, 1, false);
    add_energy_data(2, 1, true);  
    add_energy_data(3, 1, false);
    
    add_energy_data(4, 2, false);
    add_energy_data(5, 2, true);  
    add_energy_data(6, 2, false);
    add_energy_data(7, 2, false);
    
    add_energy_data(8, 3, false);
    add_energy_data(9, 3, false);
    add_energy_data(10, 3, false);
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread(process_energy_queue));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return 0;
}