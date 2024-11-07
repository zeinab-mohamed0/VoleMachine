#include <iostream>
#include "A1_T4_S5_20230090_20230093_20230165.h"

int main() {
    Machine machine;

    std::string fileName;
    std::cout << "Enter the program file name to load: ";
    std::cin >> fileName;

    // Load the program file into memory
    machine.loadProgramFile(fileName);

    std::cout << "Initial State:" << std::endl;
    machine.outputState();

    // Run the machine
    std::cout << "\nStarting program execution..." << std::endl;
    machine.run();

    std::cout << "\nFinal State after program execution:" << std::endl;
    machine.outputState();

    return 0;
}