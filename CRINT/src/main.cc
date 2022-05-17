#include "../../CRVM/src/grid.hh"
#include <iostream>
#include <string>
#include <vector>

enum command {
    down,
    right,
    up,
    left,
};

int main() {
    int n, m;
    std::cin >> n >> m;

    Grid<int> grid;
    grid.Input(n, m);

    std::string s;

    std::vector<std::pair<command, int>> history;

    /* Do not ignore and get \n intentionally to print at first */
    while (std::getline(std::cin, s)) {
        switch (s[0]) {
            case 'v': {
                int color = std::stoi(s.substr(2));
                grid.RotateDown(color);
                history.emplace_back(down, color);
            } break;
            case '>': {
                int color = std::stoi(s.substr(2));
                grid.RotateRight(color);
                history.emplace_back(right, color);
            } break;
            case '^': {
                int color = std::stoi(s.substr(2));
                grid.RotateUp(color);
                history.emplace_back(up, color);
            } break;
            case '<': {
                int color = std::stoi(s.substr(2));
                grid.RotateLeft(color);
                history.emplace_back(left, color);
            } break;
            case 'i': {
                grid.Input(n, m);
                history.clear();
            } break;
            case 'u': {
                if (history.size()) {
                    auto [command, color] = history.back();
                    history.pop_back();
                    switch (command) {
                        case down: grid.RotateUp(color); break;
                        case right: grid.RotateLeft(color); break;
                        case up: grid.RotateDown(color); break;
                        case left: grid.RotateRight(color); break;
                    }
                }
            } break;
            default: break;
        }
        grid.Print();
    }
}