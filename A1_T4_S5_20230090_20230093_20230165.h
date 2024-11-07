#ifndef A1_T4_S5_20230090_20230093_20230165_H
#define A1_T4_S5_20230090_20230093_20230165_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

extern bool haltt;

class Register {
protected:
    int intRegisters[16];

public:
    Register();
    int getIntCell(int address);
    void setIntCell(int address, int val);
};

class Memory {
public:
    int memory[256];
    Memory();
    int getCell(int address);
    void setCell(int address, int val);
};

class ALU {
public:
    int hexToDec(int hex);
};

class CU {
public:
    void load1(int regAddr, int memAddr, Register &reg, Memory &mem);
    void load2(int regAddr, int value, Register &reg);
    void store(int regAddr, int memAddr, Register &reg, Memory &mem);
    void add(int regIdx1, int regIdx2, int destRegIdx, Register &reg);
    void move(int srcReg, int destReg, Register &reg);
    void jump(int addr, int &programCounter);
    void halt(bool &haltt);
};

class CPU {
protected:
    Register reg;
    ALU alu;
    CU cu;

public:
    void fetch(int &instruction, Memory &mem, int &programCounter);
    void execute(int instruction, Memory &mem, Register &reg, CU &cu, int &programCounter, bool &haltt);
    void runNextStep(Memory &mem, int &programCounter, bool &haltt);
    bool &isHalted() const;
};

class Machine : public CPU {
    Memory memory;
    int programCounter;

public:
    Machine();
    void loadProgramFile(const string &fileName);
    void outputState();
    void run();
};

#endif // A1_T4_S5_20230090_20230093_20230165_H