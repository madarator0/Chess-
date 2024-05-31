#include "players.hpp"

class board {
private:
    vector<vector<cell>> boards;
    players player1;
    players player2;
    players* attacking;
    players* protecting;
public:
    board() : player1(true), player2(false), boards(8, vector<cell>(8)),
        attacking(&player1), protecting(&player2) {
        for (auto& a : player1.getAllPieces()) {
            boards[a->getPosition().y][a->getPosition().x].lpiece = a;
        }
        for (auto& a : player2.getAllPieces()) {
            boards[a->getPosition().y][a->getPosition().x].lpiece = a;
        }
    }

    void print() {
        cout << "  ";
        for (char c = 'a'; c <= 'h'; ++c) {
            cout << " " << c << "  ";
        }
        cout << endl;
        cout << " +";
        for (int i = 0; i < 8; ++i) {
            cout << "---+";
        }
        cout << endl;

        for (int i = 0; i < 8; i++) {
            cout << i + 1 << "|";
            for (int j = 0; j < 8; ++j) {
                if (boards[i][j].lpiece != nullptr) {
                    cout << " " << boards[i][j].lpiece->getSymbol() << " |";
                }
                else if ((i + j) % 2 == 0) {
                    cout << " # |";
                }
                else {
                    cout << "   |";
                }
            }
            cout << i + 1 << endl << " +";
            for (int k = 0; k < 8; ++k) {
                cout << "---+";
            }
            cout << endl;
        }

        cout << "  ";
        for (char c = 'a'; c <= 'h'; ++c) {
            cout << " " << c << "  ";
        }
        cout << endl;

        cout << "Current turn: ";
        if (attacking->isWhitePlayer()) {
            cout << "White" << endl;
        }
        else {
            cout << "Black" << endl;
        }
    }

    bool isCheck(players* player, players* opponent) {
        piece* king = player->getAllPieces()[0];
        vector<piece*> AllPieces = opponent->getAllPieces();
        for (piece* piece : AllPieces) {
            piece->allSteps(boards);
            for (xy np : piece->getNp()) {
                if (np.x == king->getPosition().x && np.y == king->getPosition().y) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isCheckmate(players* player, players* opponent) {
        piece* king = player->getAllPieces()[0]; // Assuming the first piece is always the King
        king->allSteps(boards); // Get all possible moves for the king
        vector<xy> kingMoves = king->getNp();

        // Collect all possible moves of the opponent
        set<xy> opponentMoves;
        for (auto& piece : opponent->getAllPieces()) {
            piece->allSteps(boards);
            vector<xy> pieceMoves = piece->getNp();
            for (const auto& move : pieceMoves) {
                opponentMoves.insert(move);
            }
        }

        // Check if any of the king's possible moves are safe
        for (const auto& move : kingMoves) {
            if (opponentMoves.find(move) == opponentMoves.end()) {
                // If any move is not in the opponent's move set, it's a safe move
                return false;
            }
        }

        // If no safe moves are found, it's checkmate
        return true;
    }

    void printNp(vector<xy> nposition) {
        for (auto& a : nposition) {
            if (boards[a.y][a.x].lpiece == nullptr ||
                    boards[a.y][a.x].lpiece->getSymbol() != 'k' &&
                    boards[a.y][a.x].lpiece->getSymbol() != 'K') {
                char r = a.x + 'a';
                cout << r << " ";
            }
        }
        cout << endl;
        for (auto& a : nposition) {
            if (boards[a.y][a.x].lpiece == nullptr ||
                    boards[a.y][a.x].lpiece->getSymbol() != 'k' &&
                    boards[a.y][a.x].lpiece->getSymbol() != 'K') {
                cout << a.y + 1 << " ";
            }
        }
        cout << endl;
    }

    void step() {
        while (true) {
            print();
            piece* chosenPiece = attacking->choose(boards);
            chosenPiece->allSteps(boards);

            if (King* king = dynamic_cast<King*>(chosenPiece)) {
                set<xy> opponentMoves;
                for (auto& piece : protecting->getAllPieces()) {
                    piece->allSteps(boards);
                    for (const auto& move : piece->getNp()) {
                        opponentMoves.insert(move);
                    }
                }
                king->filterMoves(opponentMoves);
            }

            printNp(chosenPiece->getNp());

            cout << "Choose a new position (e.g., e 5): ";
            char col;
            int row;
            cin >> col >> row;

            int newX = col - 'a';
            int newY = row - 1;

            if (!chosenPiece->whether(xy(newX, newY)) ||
                    (boards[newY][newX].lpiece &&
                     (boards[newY][newX].lpiece->getSymbol() == 'k' ||
                      boards[newY][newX].lpiece->getSymbol() == 'K'))) {
                cout << "Invalid move. Please choose a valid cell.\n";
                continue;
            }

            piece* targetPiece = boards[newY][newX].lpiece;
            if (targetPiece) {
                protecting->removePiece(targetPiece);
                delete targetPiece;
            }

            boards[chosenPiece->getPosition().y][chosenPiece->getPosition().x].lpiece = nullptr;
            boards[newY][newX].lpiece = chosenPiece;
            chosenPiece->move(xy(newX, newY));
            break;
        }
    }

    bool game() {
        step();
        if (isCheck(protecting, attacking)) {
            cout << "Check! " << (attacking->isWhitePlayer() ? "White" : "Black") << " wins!\n";
            if (isCheckmate(protecting, attacking)) {
                cout << "Checkmate! " << (attacking->isWhitePlayer() ? "White" : "Black") << " wins!\n";
                return false;
            }
        }
        swap(attacking, protecting);
        return true;
    }
};