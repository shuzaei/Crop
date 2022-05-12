#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Int = unsigned int;
using byte = unsigned char;

Int nextInt(std::stringstream &ss) {
    std::string s;
    ss >> s;
    return std::stoull(s, nullptr, 0);
}

std::vector<byte> BigEndianCast(Int x) {
    std::vector<byte> res(sizeof(Int));
    for (int i = sizeof(Int) - 1; i >= 0; i--) {
        res[i] = x & 0xff;
        x >>= 8;
    }
    return res;
}

#define CAST(x) (reinterpret_cast<char *>(BigEndianCast(x).data()))

int main(int argc, char *argv[]) {
    std::string out;
    switch (argc) {
        case 2:
        case 3: {
            std::ifstream ifs(argv[1]);
            if (!ifs) {
                std::cerr << "file not found" << std::endl;
                return 1;
            }

            if (argc == 2) {
                out = "a.cro";
            } else {
                out = argv[2];
            };

            std::ofstream ofs(out, std::ios::binary);
            if (!ofs) {
                std::cerr << "failed to open output file" << std::endl;
                return 1;
            }

            std::string line;
            Int address = 0;
            while (std::getline(ifs, line)) {
                line = std::string(line.begin(), std::find(line.begin(), line.end(), '#'));
                if (line.empty()) continue;

                Int pos = std::find(line.begin(), line.end(), ':') - line.begin();
                if (pos == line.size()) continue;

                if (address != stoull(std::string(line.begin(), line.begin() + pos), nullptr, 0)) {
                    std::cerr << "address error" << std::endl;
                    return 1;
                }

                std::stringstream ss(std::string(line.begin() + pos + 1, line.end()));
                std::string op;
                ss >> op;

                if (op == "NOP") {
                    ofs.write("\x00", 1);
                    address += 1;
                } else if (op == "INIT") {
                    Int rows = nextInt(ss), cols = nextInt(ss);
                    ofs.write("\x10", 1);
                    ofs.write(CAST(rows), sizeof(Int));
                    ofs.write(CAST(cols), sizeof(Int));
                    address += 1 + 2 * sizeof(Int);
                } else if (op == "ROTATE_D") {
                    Int color = nextInt(ss);
                    ofs.write("\x20", 1);
                    ofs.write(CAST(color), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "ROTATE_R") {
                    Int color = nextInt(ss);
                    ofs.write("\x21", 1);
                    ofs.write(CAST(color), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "ROTATE_U") {
                    Int color = nextInt(ss);
                    ofs.write("\x22", 1);
                    ofs.write(CAST(color), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "ROTATE_L") {
                    Int color = nextInt(ss);
                    ofs.write("\x23", 1);
                    ofs.write(CAST(color), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "JUMP") {
                    Int addr = nextInt(ss);
                    ofs.write("\x30", 1);
                    ofs.write(CAST(addr), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "JUMPIF") {
                    Int addr = nextInt(ss);
                    ofs.write("\x31", 1);
                    ofs.write(CAST(addr), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "JUMPIFN") {
                    Int addr = nextInt(ss);
                    ofs.write("\x32", 1);
                    ofs.write(CAST(addr), sizeof(Int));
                    address += 1 + sizeof(Int);
                } else if (op == "TEST") {
                    Int row = nextInt(ss), col = nextInt(ss), color = nextInt(ss);
                    ofs.write("\x40", 1);
                    ofs.write(CAST(row), sizeof(Int));
                    ofs.write(CAST(col), sizeof(Int));
                    ofs.write(CAST(color), sizeof(Int));
                    address += 1 + 3 * sizeof(Int);
                } else {
                    std::cerr << "unknown opcode" << std::endl;
                }
            }
        } break;

        default:
            std::cerr << "usage: " << argv[0] << " <assembly(.crs) file> [<output file>]"
                      << std::endl;
            return 1;
    }
}
