#pragma once

#include "grid.hh"
#include <stdexcept>
#include <vector>

using byte = unsigned char;

enum Command {
    NOP = 0x00,
    INIT = 0x10,
    ROTATE_U = 0x20,
    ROTATE_R = 0x21,
    ROTATE_D = 0x22,
    ROTATE_L = 0x23,
    JUMPIF = 0x30,
    JUMPIFN = 0x31,
    TEST_PP = 0x40,
    TEST_PC = 0x41,
};

template <typename Int> Int ReadInt(const std::vector<byte> &bytecode, Int &pos) {
    Int res = 0;
    for (Int i = 0; i < sizeof(Int); ++i) {
        res <<= 8;
        res |= bytecode[pos++];
    }
    return res;
}

template <typename Int> void Exec(const std::vector<byte> &bytecode) {
    // 初期化
    Int pc = 0;
    Grid<Int> grid;

    while (pc < bytecode.size()) {
        // 命令を取得
        byte command = bytecode[pc++];

        switch (command) {
            case NOP: break;
            case INIT: grid.Input(ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_U: grid.RotateDown(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_R: grid.RotateRight(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_D: grid.RotateUp(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_L: grid.RotateLeft(ReadInt<Int>(bytecode, pc)); break;
            case JUMPIF:
                command = bytecode[pc++];
                switch (command) {
                    case TEST_PP:
                        if (grid.Test(ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc),
                                      ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc))) {
                            pc = ReadInt<Int>(bytecode, pc);
                        } else {
                            pc += sizeof(Int);
                        }
                        break;
                    case TEST_PC:
                        if (grid.Test(ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc),
                                      ReadInt<Int>(bytecode, pc))) {
                            pc = ReadInt<Int>(bytecode, pc);
                        } else {
                            pc += sizeof(Int);
                        }
                        break;
                    default: throw std::runtime_error("unknown command");
                }
                break;
            case JUMPIFN:
                command = bytecode[pc++];
                switch (command) {
                    case TEST_PP:
                        if (!grid.Test(ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc),
                                       ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc))) {
                            pc = ReadInt<Int>(bytecode, pc);
                        } else {
                            pc += sizeof(Int);
                        }
                        break;
                    case TEST_PC:
                        if (!grid.Test(ReadInt<Int>(bytecode, pc), ReadInt<Int>(bytecode, pc),
                                       ReadInt<Int>(bytecode, pc))) {
                            pc = ReadInt<Int>(bytecode, pc);
                        } else {
                            pc += sizeof(Int);
                        }
                        break;
                    default: throw std::runtime_error("unknown command");
                }
                break;
            default: throw std::runtime_error("unknown command");
        }
    }

    grid.Print();
}
