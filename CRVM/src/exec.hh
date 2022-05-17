#pragma once

#include "grid.hh"
#include <stdexcept>
#include <vector>

using byte = unsigned char;

enum Command {
    NOP = 0x00,
    INIT = 0x10,
    ROTATE_D = 0x20,
    ROTATE_R = 0x21,
    ROTATE_U = 0x22,
    ROTATE_L = 0x23,
    JUMP = 0x30,
    JUMPIF = 0x31,
    JUMPIFN = 0x32,
    TEST = 0x40,
};

template <typename Int> Int ReadInt(const std::vector<byte> &bytecode, Int &pc) {
    Int res = 0;
    for (Int i = 0; i < sizeof(Int); ++i) {
        res <<= 8;
        res |= bytecode[pc++];
    }

    return res;
}

#include <iomanip>
#include <unistd.h>

template <typename Int> void Exec(const std::vector<byte> &bytecode, bool debug = false) {
    Int pc = 0, flag = 0;
    Grid<Int> grid;

    while (pc < bytecode.size()) {
        byte command = bytecode[pc++];

        switch (command) {
            case NOP: break;
            case INIT: {
                Int rows = ReadInt(bytecode, pc);
                Int cols = ReadInt(bytecode, pc);
                grid.Input(rows, cols);
            } break;
            case ROTATE_D: grid.RotateDown(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_R: grid.RotateRight(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_U: grid.RotateUp(ReadInt<Int>(bytecode, pc)); break;
            case ROTATE_L: grid.RotateLeft(ReadInt<Int>(bytecode, pc)); break;
            case JUMP: pc = ReadInt<Int>(bytecode, pc); break;
            case JUMPIF:
                if (flag) {
                    pc = ReadInt<Int>(bytecode, pc);
                } else {
                    pc += sizeof(Int);
                }
                break;
            case JUMPIFN:
                if (!flag) {
                    pc = ReadInt<Int>(bytecode, pc);
                } else {
                    pc += sizeof(Int);
                }
                break;
            case TEST: {
                Int x = ReadInt<Int>(bytecode, pc);
                Int y = ReadInt<Int>(bytecode, pc);
                Int color = ReadInt<Int>(bytecode, pc);
                flag = grid.Test(x, y, color);
            } break;
            default: throw std::runtime_error("unknown command");
        }

        if (debug) grid.Print();
    }

    grid.Print();
}
