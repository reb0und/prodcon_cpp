#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv;
int buffer_size = 5;

void produce() {
    std::unique_lock<std::mutex> lock(mtx);

    // handle full buffer
    while(buffer.size() >= buffer_size) {
        // wait for buffer to empty
        cv.wait(lock);
    }

    // produce data
    for (int i = 0; i < buffer_size-buffer.size(); i++) {
        buffer.push(i);
        std::cout << "produced\n";
    }
   
    // alert thread that buffer is full
    cv.notify_one();
}

void consume() {
    std::unique_lock<std::mutex> lock(mtx);

    while (true) {
        // handle empty buffer
        while (buffer.size() <= 0) {
            // wait for buffer to fill
            cv.wait(lock);
        }

        // consume data
        for (int i = 0; i < buffer.size(); i++) {
            buffer.pop();
            std::cout << "consumed\n";
        }

        if (buffer.empty()) {
            break;
        }
        
    }

    // alert thread that buffer is empty
    cv.notify_one();
}
