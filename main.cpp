#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <future>
#include "ThreadPool.h"

int main() {
    // 1. Initialize a thread pool with 4 concurrent worker threads
    ThreadPool pool(4);
    
    // 2. Open the dummy log file
    std::ifstream file("system_logs.txt");
    std::string line;
    std::vector<std::future<void>> results;

    if (!file.is_open()) {
        std::cerr << "Failed to open system_logs.txt\n";
        return 1;
    }

    std::cout << "Starting Concurrent Log Processor...\n";
    std::cout << "------------------------------------\n";

    // 3. Producer: Read the file line-by-line and push tasks to the queue
    while (std::getline(file, line)) {
        results.emplace_back(
            pool.enqueue([line] {
                // 4. Consumer: Threads process the string to find errors
                if (line.find("[ERROR]") != std::string::npos) {
                    std::cout << "Thread [" << std::this_thread::get_id() << "] found critical error: " << line << "\n";
                }
            })
        );
    }

    // 5. Wait for all threads to finish processing the queue
    for(auto && result: results) {
        result.get();
    }
    
    std::cout << "------------------------------------\n";
    std::cout << "Log processing complete.\n";

    return 0;
}