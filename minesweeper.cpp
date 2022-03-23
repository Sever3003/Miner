#include "minesweeper.h"
#include <stdlib.h>
#include <queue>
#include <ctime>
Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count)
    : width_(width),
      height_(height),
      mines_count_(mines_count),
      map_(new char*[height_]),
      game_status_(GameStatus::IN_PROGRESS),
      start_time_(std::time(nullptr)),
      count_open_(width_ * height_ - mines_count_) {
    srand(time(0));
    char symbol;
    char insert_symbol;
    size_t count;
    if (mines_count_ > (height_ * width_) / 2) {
        symbol = 'm';
        count = (height_ * width_) - mines_count_;
        insert_symbol = 'e';
    } else {
        symbol = 'e';
        count = mines_count_;
        insert_symbol = 'm';
    }
    for (size_t i = 0; i < height_; ++i) {
        map_[i] = new char[width_];
        for (size_t j = 0; j < width_; ++j) {
            map_[i][j] = symbol;
        }
    }
    while (count) {
        int y_coord = rand() % height_;
        int x_coord = rand() % width_;
        if (map_[y_coord][x_coord] == symbol) {
            map_[y_coord][x_coord] = insert_symbol;
            --count;
        }
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines)
    : width_(width),
      height_(height),
      mines_count_(cells_with_mines.size()),
      map_(new char*[height_]),
      game_status_(GameStatus::IN_PROGRESS),
      start_time_(std::time(nullptr)),
      count_open_(width_ * height_ - mines_count_) {
    srand(time(0));
    for (size_t i = 0; i < height_; ++i) {
        map_[i] = new char[width_];
        for (size_t j = 0; j < width_; ++j) {
            map_[i][j] = 'e';
        }
    }
    for (const auto& cell : cells_with_mines) {
        if (cell.y >= 0 && cell.y < height_ && cell.x >= 0 && cell.x < width_) {
            map_[cell.y][cell.x] = 'm';
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    for (size_t i = 0; i < height_; ++i) {
        delete[] map_[i];
    }
    delete[] map_;
    srand(time(0));
    start_time_ = std::time(nullptr);
    width_ = width;
    height_ = height;
    mines_count_ = mines_count;
    map_ = new char*[height_];
    game_status_ = GameStatus::IN_PROGRESS;
    count_open_ = width_ * height_ - mines_count_;
    char symbol;
    char insert_symbol;
    size_t count;
    if (mines_count_ > (height_ * width_) / 2) {
        symbol = 'm';
        count = (height_ * width_) - mines_count_;
        insert_symbol = 'e';
    } else {
        symbol = 'e';
        count = mines_count_;
        insert_symbol = 'm';
    }
    for (size_t i = 0; i < height_; ++i) {
        map_[i] = new char[width_];
        for (size_t j = 0; j < width_; ++j) {
            map_[i][j] = symbol;
        }
    }
    while (count) {
        int y_coord = rand() % height_;
        int x_coord = rand() % width_;
        if (map_[y_coord][x_coord] == symbol) {
            map_[y_coord][x_coord] = insert_symbol;
            --count;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    for (size_t i = 0; i < height_; ++i) {
        delete[] map_[i];
    }
    delete[] map_;
    srand(time(0));
    start_time_ = std::time(nullptr);
    width_ = width;
    height_ = height;
    mines_count_ = cells_with_mines.size();
    map_ = new char*[height_];
    game_status_ = GameStatus::IN_PROGRESS;
    count_open_ = width_ * height_ - mines_count_;
    for (size_t i = 0; i < height_; ++i) {
        map_[i] = new char[width_];
        for (size_t j = 0; j < width_; ++j) {
            map_[i][j] = 'e';
        }
    }
    for (const auto& cell : cells_with_mines) {
        if (cell.y >= 0 && cell.y < height_ && cell.x >= 0 && cell.x < width_) {
            map_[cell.y][cell.x] = 'm';
        }
    }
}

int64_t Minesweeper::CountMyCell(size_t y, size_t x) {
    size_t count = 0;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (x + i >= 0 && x + i < width_ && y + j >= 0 && y + j < height_) {
                if (map_[y + j][x + i] == 'w' || map_[y + j][x + i] == 'm') {
                    ++count;
                }
            }
        }
    }
    return count;
}
void Minesweeper::OpenCell(const Minesweeper::Cell& cell) {
    if (game_status_ != GameStatus::IN_PROGRESS) {
        return;
    }
    if (!(cell.y >= 0 && cell.y < height_ && cell.x >= 0 && cell.x < width_)) {
        return;
    }
    if (map_[cell.y][cell.x] == 'o' || map_[cell.y][cell.x] == 'w' || map_[cell.y][cell.x] == 'f') {
        return;
    }
    if (map_[cell.y][cell.x] == 'm') {
        game_status_ = GameStatus::DEFEAT;
        return;
    }
    OpenEmptyCells(cell);
    if (count_open_ == 0) {
        game_status_ = GameStatus::VICTORY;
    }
}

void Minesweeper::OpenEmptyCells(const Minesweeper::Cell& cell) {
    std::queue<Cell> queue;
    queue.push(cell);
    map_[cell.y][cell.x] = 'o';
    --count_open_;
    if (CountMyCell(cell.y, cell.x) != 0) {
        return;
    }
    while (!queue.empty()) {
        Cell current = queue.front();
        queue.pop();
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                if (current.x + i >= 0 && current.x + i < width_ && current.y + j >= 0 && current.y + j < height_) {
                    if (map_[current.y + j][current.x + i] == 'e') {
                        if (CountMyCell(current.y + j, current.x + i) == 0) {
                            queue.push({current.x + i, current.y + j});
                        }
                        map_[current.y + j][current.x + i] = 'o';
                        --count_open_;
                    }
                }
            }
        }
    }
}
time_t Minesweeper::GetGameTime() const {
    return std::time(nullptr) - start_time_;
}
Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ != GameStatus::IN_PROGRESS) {
        return;
    }
    if (!(cell.y >= 0 && cell.y < height_ && cell.x >= 0 && cell.x < width_)) {
        return;
    }
    if (map_[cell.y][cell.x] == 'm') {
        map_[cell.y][cell.x] = 'w';
        return;
    }
    if (map_[cell.y][cell.x] == 'f') {
        map_[cell.y][cell.x] = 'e';
        return;
    }
    if (map_[cell.y][cell.x] == 'e') {
        map_[cell.y][cell.x] = 'f';
        return;
    }
    if (map_[cell.y][cell.x] == 'w') {
        map_[cell.y][cell.x] = 'm';
        return;
    }
}

Minesweeper::RenderedField Minesweeper::RenderField() {
    std::vector<std::string> picture;
    picture.resize(height_);
    for (size_t i = 0; i < height_; ++i) {
        picture[i].reserve(width_);
        for (size_t j = 0; j < width_; ++j) {
            if (map_[i][j] == 'w' || map_[i][j] == 'f') {
                picture[i] += '?';
            }
            if (map_[i][j] == 'm') {
                if (game_status_ == GameStatus::DEFEAT) {
                    picture[i] += '*';
                } else {
                    picture[i] += '-';
                }
            }
            if (map_[i][j] == 'e') {
                picture[i] += '-';
            }
            if (map_[i][j] == 'o') {
                if (CountMyCell(i, j) == 0) {
                    picture[i] += '.';
                } else {
                    picture[i] += (CountMyCell(i, j)) + '0';
                }
            }
        }
    }
    return picture;
}

Minesweeper::~Minesweeper() {
    for (size_t i = 0; i < height_; ++i) {
        delete[] map_[i];
    }
    delete[] map_;
}