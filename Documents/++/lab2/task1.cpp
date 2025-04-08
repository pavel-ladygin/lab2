#include <iostream>
#include <thread>
#include <queue>
#include <random>
#include <semaphore>
#include <chrono>
#include <vector>
#include <mutex>
#include <atomic>

struct QuantumTask {
    int id;
    int priority; 
    bool isCritical; 
    int duration; 
};

// Создаем оператор сравнения для очереди с приоритетами
struct CompareQuantumPriority {
    bool operator()(const QuantumTask& t1, const QuantumTask& t2) {
        if (t1.priority != t2.priority) {
            return t1.priority > t2.priority;
        }
        return !t1.isCritical && t2.isCritical;
    }
};

std::priority_queue<QuantumTask, std::vector<QuantumTask>, CompareQuantumPriority> quantum_queue;
std::counting_semaphore<4> quantum_processors(4);
std::mutex output_mutex;
std::atomic<int> active_processors(4);
std::atomic<bool> processor_failure(false);

// Пишем функцию обработки задачи на квантовом процессоре
void process_quantum_task(QuantumTask task) {
    quantum_processors.acquire();
    
    {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "Quantum task " << task.id << " (priority " << task.priority 
                  << ", " << (task.isCritical ? "critical" : "normal") 
                  << ") started on processor. Duration: " << task.duration << "s\n";
    }
    
    // Симуляция обработки задачи
    std::this_thread::sleep_for(std::chrono::seconds(task.duration));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    if (dis(gen) == 1) {
        processor_failure = true;
        active_processors--;
        
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "!!! Processor failure during task " << task.id << " !!!\n";
        std::cout << "Active processors now: " << active_processors << "\n";
        
        quantum_processors.release();
        return;
    }
    
    {
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "Quantum task " << task.id << " completed.\n";
    }
    
    quantum_processors.release();
}

// Функция для добавления задач в очередь
void add_quantum_task(int id, int priority, bool isCritical) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 5);
    
    QuantumTask task = {id, priority, isCritical, dis(gen)};
    quantum_queue.push(task);
    
    std::lock_guard<std::mutex> guard(output_mutex);
    std::cout << "Quantum task " << id << " (priority " << priority 
              << ", " << (isCritical ? "critical" : "normal") 
              << ") added to queue.\n";
}

// Функция обработки задач из очереди
void process_quantum_tasks() {
    while (!quantum_queue.empty()) {
        if (processor_failure) {
            // Если был сбой, перераспределяем задачи
            std::lock_guard<std::mutex> guard(output_mutex);
            std::cout << "Redistributing tasks due to processor failure...\n";
            
            std::priority_queue<QuantumTask, std::vector<QuantumTask>, CompareQuantumPriority> temp_queue;
            
            while (!quantum_queue.empty()) {
                auto task = quantum_queue.top();
                quantum_queue.pop();
                
                task.duration += 1;
                temp_queue.push(task);
            }
            
            quantum_queue = std::move(temp_queue);
            processor_failure = false;
        }
        
        QuantumTask task = quantum_queue.top();
        quantum_queue.pop();
        
        if(task.isCritical && active_processors <= 2) {
            task.duration = std::max(1, task.duration / 2);
            
            std::lock_guard<std::mutex> guard(output_mutex);
            std::cout << "Critical task " << task.id << " expedited due to low processor count.\n";
        }
        process_quantum_task(task);
    }
}
int main() {
    // Добавляем задачи в очередь
    add_quantum_task(1, 1, true);
    add_quantum_task(2, 3, false); 
    add_quantum_task(3, 1, false);
    add_quantum_task(5, 4, false); 
    add_quantum_task(6, 1, true);  
    add_quantum_task(7, 5, false);
    add_quantum_task(8, 2, false); 
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread(process_quantum_tasks));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}