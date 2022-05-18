#include "../../../CRVM/src/grid.hh"
#include <Siv3D.hpp>
#include <string>
#include <vector>

std::vector<ColorF> colors = {
    Palette::White,   Palette::Red,  Palette::Orange, Palette::Yellow, Palette::Green,
    Palette::Skyblue, Palette::Blue, Palette::Purple, Palette::Pink,   Palette::Black,
};

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

    void Add(Command command, int32 color) { program.push_back(std::make_pair(command, color)); }

    void SetColor(int32 color) {
        if (program.empty()) return;
        program.back().second = color;
    }
    void SetCommand(Command command) {
        if (program.empty()) return;
        program.back().first = command;
    }
    bool IsSet() { return program.empty() || program.back().first != Null; }

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

    void Pop() {
        if (program.empty()) return;
        program.pop_back();
        done = Min(done, (int32) program.size());
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

        Rect{BXLeft - margin, 0, BXRight - BXLeft + margin * 2, margin}.draw(ColorF{0.8, 0.9, 1.0});
        Rect{BXLeft - margin, BYDown, BXRight - BXLeft + margin * 2, margin * 2}.draw(
            ColorF{0.8, 0.9, 1.0});
    }
    void DrawLine(int32 fromX, int32 fromY, int32 toX, int32 height, int32 command, int32 color,
                  bool done) {
        Rect{fromX, fromY, toX - fromX, height}.draw(done ? Palette::Skyblue : Palette::White);
        Rect{Arg::center(fromX + (toX - fromX) / 4, fromY + height / 2), px}
            .draw(Palette::White)
            .drawFrame(border / 2, border / 2, Palette::Black);
        font({cmd2str[command]})
            .drawAt(fromX + (toX - fromX) / 4, fromY + height / 2, Palette::Black);
        Rect{Arg::center(fromX + (toX - fromX) * 3 / 4, fromY + height / 2), px}
            .draw(colors[color])
            .drawFrame(border / 2, border / 2, Palette::Black);
    }
};

void Main() {
    Scene::SetBackground(ColorF{0.8, 0.9, 1.0});
    int32 n = 4;
    int32 focusX = -1, focusY = -1;
    ::Grid<int32> grid(n, n);

    std::vector<std::vector<Rect>> rects;

    ProgramView programView;

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
                    rects[i][j]
                        .draw(colors[color])
                        .drawFrame(margin / 4, margin / 4, Palette::Gray);
                } else {
                    rects[i][j].draw(colors[color]);
                }
            }
        }

        programView.Draw();

        if (MouseL.up()) {
            int32 x = Cursor::Pos().x / (size + margin);
            int32 y = Cursor::Pos().y / (size + margin);

            if (InRange(x, 0, n - 1) && InRange(y, 0, n - 1) &&
                Rect(rects[x][y]).contains(Cursor::Pos())) {
                focusX = x;
                focusY = y;
            } else {
                focusX = -1;
                focusY = -1;
            }
        }

        programView.Scroll(Cursor::Pos(), Mouse::Wheel());

        if (focusX == -1 || focusY == -1) {
            if (programView.IsSet()) {
                if ((Key0 | KeyNum0).up()) programView.Add(Null, 0);
                if ((Key1 | KeyNum1).up()) programView.Add(Null, 1);
                if ((Key2 | KeyNum2).up()) programView.Add(Null, 2);
                if ((Key3 | KeyNum3).up()) programView.Add(Null, 3);
                if ((Key4 | KeyNum4).up()) programView.Add(Null, 4);
                if ((Key5 | KeyNum5).up()) programView.Add(Null, 5);
                if ((Key6 | KeyNum6).up()) programView.Add(Null, 6);
                if ((Key7 | KeyNum7).up()) programView.Add(Null, 7);
                if ((Key8 | KeyNum8).up()) programView.Add(Null, 8);
                if ((Key9 | KeyNum9).up()) programView.Add(Null, 9);

                if ((Key0 | KeyNum0 | Key1 | KeyNum1 | Key2 | KeyNum2 | Key3 | KeyNum3 | Key4 |
                     KeyNum4 | Key5 | KeyNum5 | Key6 | KeyNum6 | Key7 | KeyNum7 | Key8 | KeyNum8 |
                     Key9 | KeyNum9)
                        .up()) {
                    programView.Fetch();
                }
            } else {
                if ((Key0 | KeyNum0).up()) programView.SetColor(0);
                if ((Key1 | KeyNum1).up()) programView.SetColor(1);
                if ((Key2 | KeyNum2).up()) programView.SetColor(2);
                if ((Key3 | KeyNum3).up()) programView.SetColor(3);
                if ((Key4 | KeyNum4).up()) programView.SetColor(4);
                if ((Key5 | KeyNum5).up()) programView.SetColor(5);
                if ((Key6 | KeyNum6).up()) programView.SetColor(6);
                if ((Key7 | KeyNum7).up()) programView.SetColor(7);
                if ((Key8 | KeyNum8).up()) programView.SetColor(8);
                if ((Key9 | KeyNum9).up()) programView.SetColor(9);

                if ((KeyS | KeyDown).up()) programView.SetCommand(D);
                if ((KeyD | KeyRight).up()) programView.SetCommand(R);
                if ((KeyW | KeyUp).up()) programView.SetCommand(U);
                if ((KeyA | KeyLeft).up()) programView.SetCommand(L);

                if ((KeyS | KeyD | KeyW | KeyA | KeyDown | KeyRight | KeyUp | KeyLeft).up()) {
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
                        .up()) {
                    programView.Fetch();
                }
            }

            if (KeyBackspace.down()) {
                std::pair<Command, int32> command = programView.GetPrev();
                programView.Pop();
                switch (command.first) {
                    case D: grid.RotateUp(command.second); break;
                    case R: grid.RotateLeft(command.second); break;
                    case U: grid.RotateDown(command.second); break;
                    case L: grid.RotateRight(command.second); break;
                    default: break;
                }
                programView.Fetch();
            }

            if (KeyE.down()) {
                focusX = 0;
                focusY = 0;
            }
        } else {
            if ((KeyS | KeyDown).up()) { focusY = (focusY + 1) % n; }
            if ((KeyD | KeyRight).up()) { focusX = (focusX + 1) % n; }
            if ((KeyW | KeyUp).up()) { focusY = (focusY + n - 1) % n; }
            if ((KeyA | KeyLeft).up()) { focusX = (focusX + n - 1) % n; }

            if ((Key0 | KeyNum0).up()) grid.Set(focusY, focusX, 0);
            if ((Key1 | KeyNum1).up()) grid.Set(focusY, focusX, 1);
            if ((Key2 | KeyNum2).up()) grid.Set(focusY, focusX, 2);
            if ((Key3 | KeyNum3).up()) grid.Set(focusY, focusX, 3);
            if ((Key4 | KeyNum4).up()) grid.Set(focusY, focusX, 4);
            if ((Key5 | KeyNum5).up()) grid.Set(focusY, focusX, 5);
            if ((Key6 | KeyNum6).up()) grid.Set(focusY, focusX, 6);
            if ((Key7 | KeyNum7).up()) grid.Set(focusY, focusX, 7);
            if ((Key8 | KeyNum8).up()) grid.Set(focusY, focusX, 8);
            if ((Key9 | KeyNum9).up()) grid.Set(focusY, focusX, 9);

            if ((KeyEscape | KeyE).down()) {
                focusX = -1;
                focusY = -1;
            }
        }

        if ((KeyShift.pressed() && (KeyEqual_US | KeySemicolon_JIS).up()) || KeyNumAdd.up()) {
            n++;
            grid.Reset(n, n);
        }

        if ((KeyMinus | KeyNumSubtract).up() && n > 2) {
            n--;
            grid.Reset(n, n);
        }

        if (KeyR.down()) grid.Reset(n, n);
    }
}
