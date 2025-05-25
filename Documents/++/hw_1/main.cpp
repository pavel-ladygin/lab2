#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <fstream>
#include <atomic>
#include <random>

// Создание структуры для клиента
struct Client {
    int id;
    double balance;
    std::unique_ptr<std::mutex> mtx;

    // Реализация конструктора перемещения
    Client(Client&& other) noexcept
        : id(other.id), balance(other.balance), mtx(std::move(other.mtx)) {}

    // Реализация оператора перемещения
    Client& operator=(Client&& other) noexcept {
        if (this != &other) {
            id = other.id;
            balance = other.balance;
            mtx = std::move(other.mtx);
        }
        return *this;
    }

    // Конструктор клиента
    Client(int id, double balance)
        : id(id), balance(balance), mtx(std::make_unique<std::mutex>()) {}
};

std::vector<Client> clients;
std::atomic<int> transactionCount(0);
const int TOTAL_TRANSACTIONS = 100;

// Функция для проведеня транзакции
void processTransaction(int threadId) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> clientDist(0, clients.size() - 1);
    std::uniform_real_distribution<> amountDist(1.0, 100.0);

    while (transactionCount < TOTAL_TRANSACTIONS) {
        int from = clientDist(gen);
        int to = clientDist(gen);
        if (from == to) continue; 

        double amount = amountDist(gen);

        // Блокировка мьютексов для защиты данных
        std::unique_lock<std::mutex> lock1(*clients[from].mtx, std::defer_lock);
        std::unique_lock<std::mutex> lock2(*clients[to].mtx, std::defer_lock);
        std::lock(lock1, lock2);

        if (clients[from].balance >= amount) {
            clients[from].balance -= amount;
            clients[to].balance += amount;
            transactionCount++;
        }
    }
}

int main() {
    // Инициализация клиентов
    const int NUM_CLIENTS = 10;
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        clients.emplace_back(i + 1, 1000.0);
    }

    // Замер времени начала выполнения
    auto start = std::chrono::high_resolution_clock::now();

    // Создание потоков
    const int NUM_THREADS = 5;
    boost::thread_group threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.create_thread([i]() { processTransaction(i); });
    }

    // Ожидание завершения всех потоков
    threads.join_all();

    // Замер времени окончания выполнения
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Вывод итоговых балансов
    std::cout << "Final balances:" << std::endl;
    for (const auto& client : clients) {
        std::cout << "Клиент номер " << client.id << "  Баланс клиента: " << client.balance << std::endl;
    }

    // Вывод времени выполнения
    std::cout << "Общее время: " << elapsed.count() << " секунд" << std::endl;


    return 0;
}