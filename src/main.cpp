#include <thread>

#include "prod_con.h"

int main() {
    std::thread prod1(produce);
    std::thread con1(consume);

    prod1.join();
    con1.join();
}