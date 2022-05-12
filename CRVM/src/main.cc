#include "exec.hh"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using Int = unsigned int;

int main(int argc, char *argv[]) {

    std::vector<byte> bytecode;
    switch (argc) {
        case 2: {
            std::ifstream ifs(argv[1]);
            if (!ifs) {
                std::cerr << "file not found" << std::endl;
                return 1;
            }
            bytecode.resize(std::filesystem::file_size(argv[1]));
            ifs.read(reinterpret_cast<char *>(bytecode.data()), bytecode.size());
        } break;

        default: std::cerr << "usage: " << argv[0] << " <bytecode file>" << std::endl; return 1;
    }

    Exec<Int>(bytecode);

    return 0;
}