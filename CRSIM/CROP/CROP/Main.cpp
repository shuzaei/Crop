#include "../../../CRVM/src/grid.hh"
#include <Siv3D.hpp>
#include <fstream>
#include <string>
#include <vector>

std::vector<ColorF> colors = {
    Palette::White, Palette::Black,      Palette::Red,  Palette::Darkorange, Palette::Yellow,
    Palette::Green, Palette::Dodgerblue, Palette::Blue, Palette::Darkviolet, Palette::Hotpink,
};

ColorF bgc = Palette::Lightgreen;
ColorF runc = Palette::Aquamarine;
ColorF bdc = Palette::White;
ColorF flc = Palette::Black;
ColorF slc = Palette::Gray;

enum Command {
    Null,
    D,
    R,
    U,
    L,
};

class ProgramView {
    private:
    const static int px = 32, border = 4;
    std::vector<std::pair<Command, int32>> program;
    String cmd2str = U"　↓→↑←";
    std::string cmd2prg = " v>^<";
    int32 scrollY = 0, done = 0;
    Font font{px, Typeface::Bold};

    int32 margin, BXLeft, BXRight, BYUp, BYDown, height;

    public:
    ProgramView() :
        margin(Window::GetState().virtualSize.y / 32), BYUp(Window::GetState().virtualSize.y / 32),
        BYDown(Window::GetState().virtualSize.y * 31 / 32),
        BXLeft(Window::GetState().virtualSize.y * 32 / 32),
        BXRight(Window::GetState().virtualSize.x - margin),
        height(Window::GetState().virtualSize.y * 4 / 32) {}

    void Add(Command command, int32 color) {
        program.push_back(std::make_pair(command, color));
    }

    void SetColor(int32 color) {
        if (program.empty()) return;
        program.back().second = color;
    }
    void SetCommand(Command command) {
        if (program.empty()) return;
        program.back().first = command;
    }
    bool IsSet() { return program.empty() || program.back().first != Null; }

    int32 GetCmdSize() { return program.size(); }

    void Reset() {
        program.clear();
        scrollY = 0;
        done = 0;
    }

    std::pair<Command, int32> GetNext() {
        if (done == program.size()) return std::make_pair(Null, -1);
        return program[done++];
    }
    std::pair<Command, int32> GetPrev() {
        if (done == 0) return std::make_pair(Null, -1);
        return program[--done];
    }

    void Print(String path, int32 n) {
        std::ofstream ofs(path.narrow());
        if (ofs.is_open()) {
            ofs << "init " << n << ' ' << n << '\n';
            for (int32 i = 0; i < program.size(); i++) {
                if (program[i].first) {
                    ofs << cmd2prg[program[i].first] << ' ' << program[i].second << '\n';
                }
            }
        }
    }

    void Scroll(Point cur, double delta) {
        if (InRange(cur.x, BXLeft, BXRight) && InRange(cur.y, BYUp, BYDown)) {
            scrollY += (int32) (delta * px);
            scrollY =
                Min(scrollY, (height + margin) * (int32) program.size() - margin - (BYDown - BYUp));
            scrollY = Max(scrollY, 0);
        }
    }

    void Fetch() {
        scrollY = Max(0, (height + margin) * (int32) program.size() - margin - (BYDown - BYUp));
    }

    void Step() { done = Min(done + 1, (int32) program.size()); }
    void StepBack() { done = Max(done - 1, 0); }

    std::pair<Command, int32> Pop() {
        if (program.empty()) return std::make_pair(Null, -1);
        if (IsSet()) {
            std::pair<Command, int32> res = program.back();
            program.pop_back();
            StepBack();
            return res;
        } else {
            program.pop_back();
            return std::make_pair(Null, -1);
        }
    }
    void Draw() {
        margin = Window::GetState().virtualSize.y / 32;
        BYUp = Window::GetState().virtualSize.y / 32;
        BYDown = Window::GetState().virtualSize.y * 31 / 32;
        BXLeft = Window::GetState().virtualSize.y * 32 / 32;
        BXRight = Window::GetState().virtualSize.x - margin;
        height = Window::GetState().virtualSize.y * 4 / 32;
        for (int i = 0; i < program.size(); i++) {
            int32 y = i * (height + margin);
            int32 viewY = y - scrollY + BYUp;
            if (InRange(viewY, BYUp - height, BYDown)) {
                DrawLine(BXLeft, viewY, BXRight, height, program[i].first, program[i].second,
                         i < done);
            }
        }

        Rect{BXLeft - margin, 0, BXRight - BXLeft + margin * 2, margin}.draw(bgc);
        Rect{BXLeft - margin, BYDown, BXRight - BXLeft + margin * 2, margin * 2}.draw(bgc);
    }
    void DrawLine(int32 fromX, int32 fromY, int32 toX, int32 height, int32 command, int32 color,
                  bool done) {
        Rect{fromX, fromY, toX - fromX, height}.draw(done ? runc : bdc);
        Rect{Arg::center(fromX + (toX - fromX) / 4, fromY + height / 2), px}.draw(bdc).drawFrame(
            border / 2, border / 2, flc);
        font({cmd2str[command]}).drawAt(fromX + (toX - fromX) / 4, fromY + height / 2, flc);
        Rect{Arg::center(fromX + (toX - fromX) * 3 / 4, fromY + height / 2), px}
            .draw(colors[color])
            .drawFrame(border / 2, border / 2, flc);
    }
};

void Input(::Grid<int32> &grid, String path, int32 &n) {
    std::ifstream ifs(path.narrow());
    if (ifs.is_open()) {
        int32 n_, m_;
        ::Grid<int32> grid_;
        try {
            ifs >> n_ >> m_;
            if (n_ != m_) throw std::runtime_error("not square");
            if (n_ < 2) throw std::runtime_error("too small");
            grid_.Input(n_, n_, ifs);
            for (int32 i = 0; i < n_; i++) {
                for (int32 j = 0; j < n_; j++) {
                    if (!InRange(grid_.Get(j, i), 0, 9)) {
                        throw std::runtime_error("out of range");
                    }
                }
            }
            n = n_;
            grid = grid_;
        } catch (std::runtime_error &e) { return; };
    }
}

void Output(::Grid<int32> &grid, String path, int32 n) {
    std::ofstream ofs(path.narrow());
    if (ofs.is_open()) {
        ofs << n << ' ' << n << '\n';
        grid.Print(ofs);
    }
}

void Main() {
    Scene::SetBackground(bgc);
    Window::SetTitle(U"Crop");

    int32 n = 4;
    int32 focusX = -1, focusY = -1;
    ::Grid<int32> grid(n, n);

    std::vector<std::vector<Rect>> rects;

    ProgramView programView;

	uint64 lastMS = 0;

    while (System::Update()) {

        int32 gridSize = Window::GetState().virtualSize.y * 31 / 32;
        int32 margin = Window::GetState().virtualSize.y / 32;
        int32 size = gridSize / n - margin;

        rects = std::vector<std::vector<Rect>>(n, std::vector<Rect>(n));

        for (int32 i = 0; i < n; ++i) {
            for (int32 j = 0; j < n; ++j) {
                int32 x = margin + i * (size + margin);
                int32 y = margin + j * (size + margin);
                int32 color = grid.Get(j, i);
                rects[i][j] = Rect{x, y, size};

                if (i == focusX && j == focusY) {
                    rects[i][j].draw(colors[color]).drawFrame(margin / 4, margin / 4, slc);
                } else {
                    rects[i][j].draw(colors[color]);
                }
            }
        }

        programView.Draw();

        if (MouseL.down()) {
            int32 x = Cursor::Pos().x / (size + margin);
            int32 y = Cursor::Pos().y / (size + margin);

            if (InRange(x, 0, n - 1) && InRange(y, 0, n - 1) &&
                Rect(rects[x][y]).contains(Cursor::Pos())) {
                focusX = x;
                focusY = y;
            } else if (InRange(Cursor::Pos().x, 0, Window::GetState().virtualSize.x - 1) &&
                       InRange(Cursor::Pos().y, 0, Window::GetState().virtualSize.y - 1)) {
                focusX = -1;
                focusY = -1;
            }
        }

        programView.Scroll(Cursor::Pos(), Mouse::Wheel());

        if (focusX == -1 || focusY == -1) {
            if (programView.IsSet()) {
                if ((Key0 | KeyNum0).pressed())
					programView.Add(Null, 0);
                else if ((Key1 | KeyNum1).pressed())
					programView.Add(Null, 1);
                else if ((Key2 | KeyNum2).pressed())
                    programView.Add(Null, 2);
                else if ((Key3 | KeyNum3).pressed())
                    programView.Add(Null, 3);
                else if ((Key4 | KeyNum4).pressed())
                    programView.Add(Null, 4);
                else if ((Key5 | KeyNum5).pressed())
                    programView.Add(Null, 5);
                else if ((Key6 | KeyNum6).pressed())
                    programView.Add(Null, 6);
                else if ((Key7 | KeyNum7).pressed())
                    programView.Add(Null, 7);
                else if ((Key8 | KeyNum8).pressed())
                    programView.Add(Null, 8);
                else if ((Key9 | KeyNum9).pressed())
                    programView.Add(Null, 9);

                if ((Key0 | KeyNum0 | Key1 | KeyNum1 | Key2 | KeyNum2 | Key3 | KeyNum3 | Key4 |
                     KeyNum4 | Key5 | KeyNum5 | Key6 | KeyNum6 | Key7 | KeyNum7 | Key8 | KeyNum8 |
                     Key9 | KeyNum9)
                        .pressed()) {
                    programView.Fetch();
                }
            } else {
                if ((Key0 | KeyNum0).down()) programView.SetColor(0);
                if ((Key1 | KeyNum1).down()) programView.SetColor(1);
                if ((Key2 | KeyNum2).down()) programView.SetColor(2);
                if ((Key3 | KeyNum3).down()) programView.SetColor(3);
                if ((Key4 | KeyNum4).down()) programView.SetColor(4);
                if ((Key5 | KeyNum5).down()) programView.SetColor(5);
                if ((Key6 | KeyNum6).down()) programView.SetColor(6);
                if ((Key7 | KeyNum7).down()) programView.SetColor(7);
                if ((Key8 | KeyNum8).down()) programView.SetColor(8);
                if ((Key9 | KeyNum9).down()) programView.SetColor(9);

                if ((KeyS | KeyDown).down()) programView.SetCommand(D);
                if ((KeyD | KeyRight).down()) programView.SetCommand(R);
                if ((KeyW | KeyUp).down()) programView.SetCommand(U);
                if ((KeyA | KeyLeft).down()) programView.SetCommand(L);

                if ((KeyS | KeyD | KeyW | KeyA | KeyDown | KeyRight | KeyUp | KeyLeft).down()) {
                    std::pair<Command, int32> command = programView.GetNext();
                    switch (command.first) {
                        case D: grid.RotateDown(command.second); break;
                        case R: grid.RotateRight(command.second); break;
                        case U: grid.RotateUp(command.second); break;
                        case L: grid.RotateLeft(command.second); break;
                        default: break;
                    }
                    programView.Fetch();
                }

                if ((Key0 | KeyNum0 | Key1 | KeyNum1 | Key2 | KeyNum2 | Key3 | KeyNum3 | Key4 |
                     KeyNum4 | Key5 | KeyNum5 | Key6 | KeyNum6 | Key7 | KeyNum7 | Key8 | KeyNum8 |
                     Key9 | KeyNum9)
                        .down()) {
                    programView.Fetch();
                }
            }

            if ((KeyBackspace.down() && !KeyShift.pressed()) ||
				(KeyBackspace.pressed() && !KeyShift.pressed() && lastMS && lastMS + 150 <= Time::GetMillisec())) {
                std::pair<Command, int32> command = programView.Pop();
                switch (command.first) {
                    case D: grid.RotateUp(command.second); break;
                    case R: grid.RotateLeft(command.second); break;
                    case U: grid.RotateDown(command.second); break;
                    case L: grid.RotateRight(command.second); break;
                    default: break;
                }
                programView.Fetch();
                lastMS = Time::GetMillisec();
                if (KeyBackspace.down() && !KeyShift.pressed()) lastMS += 450;
            }
			if (!KeyBackspace.pressed()) {
                lastMS = 0;
			}

            if (KeyE.down()) {
                focusX = 0;
                focusY = 0;
            }
        } else {
            if ((KeyS | KeyDown).down()) { focusY = (focusY + 1) % n; }
            if ((KeyD | KeyRight).down()) { focusX = (focusX + 1) % n; }
            if ((KeyW | KeyUp).down()) { focusY = (focusY + n - 1) % n; }
            if ((KeyA | KeyLeft).down()) { focusX = (focusX + n - 1) % n; }

            if ((Key0 | KeyNum0).down()) grid.Set(focusY, focusX, 0);
            if ((Key1 | KeyNum1).down()) grid.Set(focusY, focusX, 1);
            if ((Key2 | KeyNum2).down()) grid.Set(focusY, focusX, 2);
            if ((Key3 | KeyNum3).down()) grid.Set(focusY, focusX, 3);
            if ((Key4 | KeyNum4).down()) grid.Set(focusY, focusX, 4);
            if ((Key5 | KeyNum5).down()) grid.Set(focusY, focusX, 5);
            if ((Key6 | KeyNum6).down()) grid.Set(focusY, focusX, 6);
            if ((Key7 | KeyNum7).down()) grid.Set(focusY, focusX, 7);
            if ((Key8 | KeyNum8).down()) grid.Set(focusY, focusX, 8);
            if ((Key9 | KeyNum9).down()) grid.Set(focusY, focusX, 9);

            if (KeyBackspace.down() && !KeyShift.pressed()) grid.Set(focusY, focusX, 0);

            if ((Key0 | KeyNum0 | Key1 | KeyNum1 | Key2 | KeyNum2 | Key3 | KeyNum3 | Key4 |
                 KeyNum4 | Key5 | KeyNum5 | Key6 | KeyNum6 | Key7 | KeyNum7 | Key8 | KeyNum8 |
                 Key9 | KeyNum9)
                    .down() ||
                (KeyBackspace.down() && !KeyShift.pressed())) {
                programView.Reset();
            }

            if ((KeyEscape | KeyE).down()) {
                focusX = -1;
                focusY = -1;
            }
        }

        if ((KeyShift.pressed() && (KeyEqual_US | KeySemicolon_JIS).down()) || KeyNumAdd.down()) {
            n++;
            grid.Reset(n, n);
            programView.Reset();
        }

        if ((KeyMinus | KeyNumSubtract).down() && n > 2) {
            n--;
            grid.Reset(n, n);
            programView.Reset();
        }

        if (KeyBackspace.down() && KeyShift.pressed()) {
            grid.Reset(n, n);
            programView.Reset();
        }

        if (KeyR.down()) {
            while (programView.GetCmdSize()) {
                std::pair<Command, int32> command = programView.Pop();
                switch (command.first) {
                    case D: grid.RotateUp(command.second); break;
                    case R: grid.RotateLeft(command.second); break;
                    case U: grid.RotateDown(command.second); break;
                    case L: grid.RotateRight(command.second); break;
                    default: break;
                }
            }
        }

        if (DragDrop::HasNewFilePaths()) {
            String path = DragDrop::GetDroppedFilePaths().back().path;
            ::Input(grid, path, n);
        }

        if (KeyO.down()) {
            Optional<FilePath> path = Dialog::OpenFile({{U"Crop Board File", {U"cb", U"crbd"}}});
            if (path) ::Input(grid, *path, n);
        }

        if (KeyP.down()) {
            Optional<FilePath> path = Dialog::SaveFile({{U"Crop Board File", {U"cb", U"crbd"}}});
            if (path) ::Output(grid, *path, n);
        }

        if (KeyM.down()) {
            Optional<FilePath> path = Dialog::SaveFile({{U"Crop Program File", {U"cp", U"crop"}}});
            if (path) programView.Print(*path, n);
        }
    }
}
