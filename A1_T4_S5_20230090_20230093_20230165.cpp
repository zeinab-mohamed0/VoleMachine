#include "A1_T4_S5_20230090_20230093_20230165.h"
#include <iostream>


bool haltt = false;

// Register class implementation
Register::Register() : intRegisters{0}{}

int Register::getIntCell(int address) {
    if (address < 0 || address >= 16) throw out_of_range("Integer register address out of bounds.");
    return intRegisters[address];
}

void Register::setIntCell(int address, int val) {
    if (address < 0 || address >= 16) throw out_of_range("Integer register address out of bounds.");
    intRegisters[address] = val;
}

// Memory class implementation
Memory::Memory() : memory{0} {}

int Memory::getCell(int address) {
    if (address < 0 || address >= 256) throw out_of_range("Memory address out of bounds.");
    return memory[address];
}

void Memory::setCell(int address, int val) {
    if (address < 0 || address >= 256) throw out_of_range("Memory address out of bounds.");
    memory[address] = val;
}

// ALU class implementation
int ALU::hexToDec(int hex) {
    return hex;
}

// CU class implementation
void CU::load1(int regAddr, int memAddr, Register &reg, Memory &mem) {
    try {
        int value = mem.getCell(memAddr);
        cout << "LOAD1: Loading value " << hex << value << " from memory address [" << hex << memAddr << "] into register R" << dec << regAddr << endl;
        reg.setIntCell(regAddr, value);
    } catch (const out_of_range &e) {
        cout << "Error: Memory address [" << hex << memAddr << "] out of bounds." << endl;
    }
}

void CU::load2(int regAddr, int value, Register &reg) {
    cout << "LOAD2: Loading immediate value " << hex << value << " into register R" << dec << regAddr << endl;
    reg.setIntCell(regAddr, value);
}

void CU::store(int regAddr, int memAddr, Register &reg, Memory &mem) {
    int value = reg.getIntCell(regAddr);
    cout << "STORE: Storing value " << value << " from register R" << regAddr << " into memory address [" << hex << memAddr << "]" << endl;
    mem.setCell(memAddr, value);
}

void CU::add(int regIdx1, int regIdx2, int destRegIdx, Register &reg) {
    int result = reg.getIntCell(regIdx1) + reg.getIntCell(regIdx2);
    cout << "ADD: Adding value from register R" << regIdx1 << " and register R" << regIdx2 << ", result stored in register R" << destRegIdx << endl;
    reg.setIntCell(destRegIdx, result);
}

void CU::move(int srcReg, int destReg, Register &reg) {
    int value = reg.getIntCell(srcReg);
    cout << "MOVE: Moving value " << value << " from register R" << srcReg << " to register R" << destReg << endl;
    reg.setIntCell(destReg, value);
}

void CU::jump(int addr, int &programCounter) {
    cout << "JUMP: Setting program counter to address " << hex << addr << endl;
    programCounter = addr;
}

void CU::halt(bool &haltt) {
    cout << "HALT: Program execution has stopped." << endl;
    haltt = true;
}

// CPU class implementation
void CPU::fetch(int &instruction, Memory &mem, int &programCounter) {
    if (programCounter >= 256 || mem.getCell(programCounter) == 0) {
        cout << "Error: Reached end of memory or empty instruction. Halting..." << endl;
        instruction = 0;
        cu.halt(haltt);
        return;
    }

    instruction = (mem.getCell(programCounter) << 8) | mem.getCell(programCounter + 1);
    cout << "Fetched instruction: " << hex << instruction << " at PC: " << programCounter << endl;
    programCounter += 2;
}

void CPU::execute(int instruction, Memory &mem, Register &reg, CU &cu, int &programCounter, bool &haltt) {
    int opCode = (instruction >> 12) & 0xF;
    int R = (instruction >> 8) & 0xF;
    int S = (instruction >> 4) & 0xF;
    int T = instruction & 0xF;

    cout << "Executing instruction: " << hex << instruction << " | opCode: " << opCode << ", R: " << R << ", S: " << S << ", T: " << T << endl;

    switch (opCode) {
        case 0x1:  cu.load1(R, (S << 4 | T), reg, mem); break;
        case 0x2:  cu.load2(R, (S << 4 | T), reg); break;
        case 0x3:  cu.store(R, (S << 4 | T), reg, mem); break;
        case 0x5:  cu.add(S, T, R, reg); break;
        case 0x4:  cu.move(S, R, reg); break;
        case 0xB:  cu.jump((S << 4 | T), programCounter); break;
        case 0xC:  cu.halt(haltt); break;
        default:   cout << "Invalid instruction: " << hex << instruction << endl; break;
    }
}

void CPU::runNextStep(Memory &mem, int &programCounter, bool &haltt) {
    int instruction;
    fetch(instruction, mem, programCounter);
    execute(instruction, mem, reg, cu, programCounter, haltt);
}

bool &CPU::isHalted() const {
    return haltt;
}

// Machine class implementation
Machine::Machine() : programCounter(0x0A) {}

void Machine::loadProgramFile(const string &fileName) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    int address = 0;

    while (getline(file, line) && address < 256) {
        try {
            int instruction = stoi(line, nullptr, 16);
            memory.setCell(address++, instruction);
        } catch (const invalid_argument &e) {
            cout << "Invalid instruction format in file: " << line << endl;
            continue;
        }
    }

    file.close();
    cout << "Program loaded from " << fileName << " with " << address << " instructions." << endl;
}

void Machine::outputState() {
    cout << "Integer Registers:\n";
    for (int i = 0; i < 16; ++i) {
        cout << "R" << i << ": " << dec << reg.getIntCell(i) << endl;
    }
    cout << "\nMemory:\n";
    for (int i = 0; i < 256; ++i) {
        cout << hex << setw(2) << setfill('0') << memory.getCell(i) << " ";
        if ((i + 1) % 16 == 0) cout << endl;
    }
}

void Machine::run() {
    while (!haltt) {
        runNextStep(memory, programCounter, haltt);
    }
}