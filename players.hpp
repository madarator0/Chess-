#include "piece.hpp"


class players {
private:
    vector<piece*> pieces;
    bool isWhite;
public:
    players(bool white) : isWhite(white) {
        if (isWhite) {
            pieces.push_back(new King(4, 0, true));
            pieces.push_back(new Queen(3, 0, true));
            pieces.push_back(new Rook(0, 0, true));
            pieces.push_back(new Rook(7, 0, true));
            pieces.push_back(new Bishop(2, 0, true));
            pieces.push_back(new Bishop(5, 0, true));
            pieces.push_back(new Knight(1, 0, true));
            pieces.push_back(new Knight(6, 0, true));
            for (int i = 0; i < 8; ++i) {
                pieces.push_back(new Pawn(i, 1, true));
            }
        }
        else {
            pieces.push_back(new King(4, 7, false));
            pieces.push_back(new Queen(3, 7, false));
            pieces.push_back(new Rook(0, 7, false));
            pieces.push_back(new Rook(7, 7, false));
            pieces.push_back(new Bishop(2, 7, false));
            pieces.push_back(new Bishop(5, 7, false));
            pieces.push_back(new Knight(1, 7, false));
            pieces.push_back(new Knight(6, 7, false));
            for (int i = 0; i < 8; ++i) {
                pieces.push_back(new Pawn(i, 6, false));
            }
        }
    }

    ~players() {
        for (auto p : pieces) {
            delete p;
        }
    }

    void removePiece(piece* p) {
        for (auto it = pieces.begin(); it != pieces.end(); ++it) {
            if (*it == p) {
                pieces.erase(it);
                break;
            }
        }
    }

    vector<piece*> getAllPieces() const {
        return pieces;
    }

    bool isWhitePlayer() const {
        return isWhite;
    }

    piece* choose(vector<vector<cell>>& board) {
        cout << "Choose a piece (e.g., e 5): ";
        char col;
        int row;
        cin >> col >> row;

        int x = col - 'a';
        int y = row - 1;

        if (x < 0 || x >= 8 || y < 0 || y >= 8) {
            cout << "Invalid input. Please choose a valid cell.\n";
            return choose(board);
        }

        if (board[y][x].lpiece == nullptr) {
            cout << "The cell is empty. Choose a cell with a piece.\n";
            return choose(board);
        }

        if (board[y][x].lpiece->getIsWhit() != isWhite) {
            cout << "This piece is not yours. Choose your piece.\n";
            return choose(board);
        }

        return board[y][x].lpiece;
    }
};