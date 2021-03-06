#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Int = unsigned int;

Int nextInt(std::stringstream &ss) {
    std::string s;
    ss >> s;
    return std::stoull(s, nullptr, 0);
}

std::string tohex(Int n) {
    std::stringstream ss;
    ss << "0x" << std::setw(sizeof(Int)) << std::setfill('0') << std::hex << std::uppercase << n;
    return ss.str();
}

void generate(std::istream &is, std::ostream &os) {
    if (!is) {
        std::cerr << "file not found" << std::endl;
        exit(1);
    }
    if (!os) {
        std::cerr << "failed to open output file" << std::endl;
        exit(1);
    }

    Int address = 0;
    std::vector<Int> addrs, lines;

    std::string line;
    std::vector<std::string> outs;

    while (std::getline(is, line)) {
        line = std::string(line.begin(), std::find(line.begin(), line.end(), '#'));
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string op;
        ss >> op;

        if (op == "init") {
            Int n = nextInt(ss), m = nextInt(ss);
            outs.push_back(tohex(address) + ": INIT " + std::to_string(n) + " " +
                           std::to_string(m));
            address += 1 + 2 * sizeof(Int);
        } else if (op == "v") {
            Int color = nextInt(ss);
            outs.push_back(tohex(address) + ": ROTATE_D " + std::to_string(color));
            address += 1 + sizeof(Int);
        } else if (op == ">") {
            Int color = nextInt(ss);
            outs.push_back(tohex(address) + ": ROTATE_R " + std::to_string(color));
            address += 1 + sizeof(Int);
        } else if (op == "^") {
            Int color = nextInt(ss);
            outs.push_back(tohex(address) + ": ROTATE_U " + std::to_string(color));
            address += 1 + sizeof(Int);
        } else if (op == "<") {
            Int color = nextInt(ss);
            outs.push_back(tohex(address) + ": ROTATE_L " + std::to_string(color));
            address += 1 + sizeof(Int);
        } else if (op == "on") {
            Int x = nextInt(ss), y = nextInt(ss);
            std::string eq;
            ss >> eq;
            Int color = nextInt(ss);
            addrs.push_back(address);
            outs.push_back(tohex(address) + ": TEST " + std::to_string(x) + " " +
                           std::to_string(y) + " " + std::to_string(color));
            address += 1 + 2 * sizeof(Int) + sizeof(Int);
            lines.push_back(outs.size());
            outs.push_back(tohex(address) + (eq == "==" ? ": JUMPIFN " : ": JUMPIF "));
            address += 1 + sizeof(Int);
        } else if (op == "end") {
            outs.push_back(tohex(address) + ": JUMP " + tohex(addrs.back()));
            addrs.pop_back();
            address += 1 + sizeof(Int);
            outs[lines.back()] += tohex(address);
            lines.pop_back();
        }
    }

    for (auto &s : outs) os << s << std::endl;
}

int main(int argc, char *argv[]) {
    switch (argc) {
        case 1: generate(std::cin, std::cout); break;
        case 2: {
            std::ifstream ifs(argv[1]);
            std::ofstream ofs("a.crs");
            generate(ifs, ofs);
        } break;
        case 3: {
            std::ifstream ifs(argv[1]);
            std::ofstream ofs(argv[2]);
            generate(ifs, ofs);
        } break;
        default:
            std::cerr << "usage: " << argv[0] << " <preprocessed(.cri) file> [<output file>]"
                      << std::endl;
            return 1;
    }
}
