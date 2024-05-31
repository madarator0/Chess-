#include <iostream>
#include <vector>
#include <set>


using namespace std;

struct xy {
    int x;
    int y;
    xy(int nx, int ny) : x(nx), y(ny) {}

    bool operator<(const xy& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

class piece;

struct cell {
    piece* lpiece;
    cell() : lpiece(nullptr) {}
};

class piece {
protected:
    xy position;
    char symbol;
    bool isWhite;
    vector<xy> nposition;
public:
    piece(xy pos, char sym, bool white) : position(pos), symbol(sym), isWhite(white) {}

    bool isOutOfBounds(int x, int y) {
        return x < 0 || y < 0 || x >= 8 || y >= 8;
    }

    virtual void move(xy np) {
        position = np;
    }

    virtual void allSteps(vector<vector<cell>>& board) {
        nposition.clear();
    }

    virtual ~piece() {}

    xy getPosition() {
        return position;
    }

    bool getIsWhit() {
        return isWhite;
    }

    char getSymbol() {
        if (isWhite) {
            return toupper(symbol);
        }
        else {
            return tolower(symbol);
        }
    }

    vector<xy> getNp() {
        return nposition;
    }

    void setNp(vector<xy> nNp) {
        nposition = nNp;
    }

    bool whether(xy n) {
        for (xy a : nposition) {
            if (a.x == n.x && a.y == n.y) {
                return true;
            }
        }
        return false;
    }
};

class King : public piece {
public:
    King(int x, int y, bool white) : piece(xy(x, y), 'K', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();
        int directions[8][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

        for (auto dir : directions) {
            int newX = position.x + dir[0];
            int newY = position.y + dir[1];
            if (!isOutOfBounds(newX, newY) &&
                    (!board[newY][newX].lpiece ||
                     (board[newY][newX].lpiece->getIsWhit() != isWhite))) {
                nposition.push_back(xy(newX, newY));
            }
        }
    }

    void filterMoves(const set<xy>& opponentMoves) {
        vector<xy> validMoves;
        for (const auto& move : nposition) {
            if (opponentMoves.find(move) == opponentMoves.end()) {
                validMoves.push_back(move);
            }
        }
        nposition = validMoves;
    }
};

class Queen : public piece {
public:
    Queen(int x, int y, bool white) : piece(xy(x, y), 'Q', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();

        int directions[8][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

        for (auto dir : directions) {
            for (int i = 1; i < 8; ++i) {
                int newX = position.x + dir[0] * i;
                int newY = position.y + dir[1] * i;
                if (isOutOfBounds(newX, newY)) {
                    break;
                }
                if (!board[newY][newX].lpiece) {
                    nposition.push_back(xy(newX, newY));
                }
                else {
                    if (board[newY][newX].lpiece->getIsWhit() != isWhite) {
                        nposition.push_back(xy(newX, newY));
                    }
                    break;
                }
            }
        }
    }
};

class Rook : public piece {
public:
    Rook(int x, int y, bool white) : piece(xy(x, y), 'R', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();

        for (int i = position.y + 1; i < 8; ++i) {
            if (board[i][position.x].lpiece) {
                if (board[i][position.x].lpiece->getIsWhit() != isWhite) {
                    nposition.push_back(xy(position.x, i));
                }
                break;
            }
            nposition.push_back(xy(position.x, i));
        }
        for (int i = position.y - 1; i >= 0; --i) {
            if (board[i][position.x].lpiece) {
                if (board[i][position.x].lpiece->getIsWhit() != isWhite) {
                    nposition.push_back(xy(position.x, i));
                }
                break;
            }
            nposition.push_back(xy(position.x, i));
        }

        for (int i = position.x + 1; i < 8; ++i) {
            if (board[position.y][i].lpiece) {
                if (board[position.y][i].lpiece->getIsWhit() != isWhite) {
                    nposition.push_back(xy(i, position.y));
                }
                break;
            }
            nposition.push_back(xy(i, position.y));
        }
        for (int i = position.x - 1; i >= 0; --i) {
            if (board[position.y][i].lpiece) {
                if (board[position.y][i].lpiece->getIsWhit() != isWhite) {
                    nposition.push_back(xy(i, position.y));
                }
                break;
            }
            nposition.push_back(xy(i, position.y));
        }
    }
};

class Bishop : public piece {
public:
    Bishop(int x, int y, bool white) : piece(xy(x, y), 'B', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();

        int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        for (auto dir : directions) {
            for (int i = 1; i < 8; ++i) {
                int newX = position.x + dir[0] * i;
                int newY = position.y + dir[1] * i;
                if (isOutOfBounds(newX, newY)) {
                    break;
                }
                if (!board[newY][newX].lpiece) {
                    nposition.push_back(xy(newX, newY));
                }
                else {
                    if (board[newY][newX].lpiece->getIsWhit() != isWhite) {
                        nposition.push_back(xy(newX, newY));
                    }
                    break;
                }
            }
        }
    }
};

class Knight : public piece {
public:
    Knight(int x, int y, bool white) : piece(xy(x, y), 'N', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();
        int directions[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
            {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
        };

        for (auto dir : directions) {
            int newX = position.x + dir[0];
            int newY = position.y + dir[1];
            if (!isOutOfBounds(newX, newY) &&
                    (!board[newY][newX].lpiece ||
                     (board[newY][newX].lpiece->getIsWhit() != isWhite))) {
                nposition.push_back(xy(newX, newY));
            }
        }
    }
};

class Pawn : public piece {
public:
    Pawn(int x, int y, bool white) : piece(xy(x, y), 'P', white) {}

    void allSteps(vector<vector<cell>>& board) override {
        nposition.clear();
        int dir = (isWhite) ? 1 : -1;
        if (!isOutOfBounds(position.x, position.y + dir) && !board[position.y + dir][position.x].lpiece) {
            nposition.push_back(xy(position.x, position.y + dir));
            if (position.y == (isWhite ? 1 : 6) && !isOutOfBounds(position.x, position.y + 2 * dir) &&
                    !board[position.y + 2 * dir][position.x].lpiece) {
                nposition.push_back(xy(position.x, position.y + 2 * dir));
            }
        }

        if (!isOutOfBounds(position.x + 1, position.y + dir) &&
                board[position.y + dir][position.x + 1].lpiece && board[position.y + dir][position.x + 1].lpiece->getIsWhit() != isWhite) {
            nposition.push_back(xy(position.x + 1, position.y + dir));
        }
        if (!isOutOfBounds(position.x - 1, position.y + dir) &&
                board[position.y + dir][position.x - 1].lpiece && board[position.y + dir][position.x - 1].lpiece->getIsWhit() != isWhite) {
            nposition.push_back(xy(position.x - 1, position.y + dir));
        }
    }
};