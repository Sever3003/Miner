#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);
    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField();
    ~Minesweeper();

private:
    size_t width_;
    size_t height_;
    size_t mines_count_;
    char** map_;
    GameStatus game_status_;
    time_t start_time_;
    size_t count_open_;

private:
    void OpenEmptyCells(const Cell& cell);
    int64_t CountMyCell(size_t x, size_t y);
};