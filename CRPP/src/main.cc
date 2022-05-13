#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using Int = unsigned int;

Int nextInt(std::stringstream &ss, const std::map<std::string, Int> &labels = {}) {
    std::string s;
    ss >> s;
    if (s.front() == '[') s = s.substr(1, s.size() - 1);
    if (s.back() == ',') s.pop_back();
    if (s.back() == ']') s.pop_back();
    if (s.back() == ':') s.pop_back();
    if (labels.count(s)) return labels.at(s);
    return std::stoull(s, nullptr, 0);
}

std::string convert(const std::string &s, const std::map<std::string, Int> &labels = {}) {
    if (s.empty()) return s;
    std::string token;
    std::stringstream ss(s), res;
    ss >> token;
    do {
        if (labels.count(token)) res << labels.at(token);
        else
            res << token;
    } while (ss >> token && res << ' ');
    return res.str();
}

void preprocess(std::istream &is, std::ostream &os) {
    if (!is) {
        std::cerr << "file not found" << std::endl;
        exit(1);
    }
    if (!os) {
        std::cerr << "failed to open output file" << std::endl;
        exit(1);
    }

    std::vector<Int> indents(1);

    std::string line;

    std::map<std::string, Int> labels;

    while (std::getline(is, line)) {
        line = std::string(line.begin(), std::find(line.begin(), line.end(), '#'));
        if (line.empty()) continue;

        Int pos = line.find_first_not_of(' ');
        if (pos == std::string::npos) pos = line.size();

        if (pos > indents.back()) indents.push_back(pos);
        while (pos < indents.back()) {
            indents.pop_back();
            os << "end\n";
        }

        std::stringstream ss(line);
        std::string op;
        ss >> op;

        if (op == "init") {
            Int n = nextInt(ss, labels), m = nextInt(ss, labels);
            os << "init " << n << " " << m << "\n";
        } else if (op == "set") {
            std::string label;
            ss >> label;
            Int num = nextInt(ss);
            labels[label] = num;
        } else if (op == "on") {
            Int x = nextInt(ss, labels), y = nextInt(ss, labels);
            std::string eq;
            ss >> eq;
            Int color = nextInt(ss, labels);
            os << "on " << x << " " << y << " " << eq << " " << color << "\n";
        } else {
            os << convert(line.substr(pos), labels) << "\n";
        }
    }

    while (indents.size() > 1) {
        indents.pop_back();
        os << "end\n";
    }
}

int main(int argc, char *argv[]) {
    switch (argc) {
        case 1: preprocess(std::cin, std::cout); break;
        case 2: {
            std::ifstream ifs(argv[1]);
            std::ofstream ofs("a.cro");
            preprocess(ifs, ofs);
        } break;
        case 3: {
            std::ifstream ifs(argv[1]);
            std::ofstream ofs(argv[2]);
            preprocess(ifs, ofs);
        } break;
        default:
            std::cerr << "usage: " << argv[0] << " <source(.cr/.crop) file> [<output file>]"
                      << std::endl;
            return 1;
    }
}