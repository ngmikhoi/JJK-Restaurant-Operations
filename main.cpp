#include "main.h"
#include "restaurant.cpp"
#include <chrono>

template <typename inputType>
void executeTime(void (*func)(inputType), inputType input){
    auto start = chrono::high_resolution_clock::now();

    func(input);

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

//    cout << "\033[1;92m" << to_string(duration.count()) << "\033[0m" << endl; #MAC only
    cout << "Execute time: " << duration.count()/1e6<<" s";
}

int main(int argc, char *argv[]) {
    string fileName = "test.txt";

    executeTime<string>(simulate, fileName);

//    simulate(fileName);
    return 0;
}
