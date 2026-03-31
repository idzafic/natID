#include "Chess.h"

using namespace std;

// ---------------------------------------------------------------------------
int main() {
    //Position pos = {'f', 2, 'e', 2, 'e', 8}; // Example: Kf2, Pe2 vs Ke8
//    Position pos = {'f', 1, 'c', 5, 'e', 8};
    Chess chess(8); //depth = 10
    //initial setup: White: Kf1,Pc2, Black: Ke8
    Chess::Position pos = {'f', 1, 'c', 2, 'e', 8};
    cout << "Initial position:\n";
    chess.printBoard(pos);

    while (true) {
        pos = chess.findBestMove(pos);
        cout << "\nAI (White) moves:\n";
        chess.printBoard(pos);

        do {
            cout << "\nYour move (black king xy): ";
            char ch, y;
            cin >> ch >> y;
            if (ch == 'q')
                return 0;
            int nx = ch - 'a';
            int ny = y - '1';
            if (chess.isBlackMoveLegal(pos, nx, ny)) {
                pos.bkx = nx;
                pos.bky = ny;
                pos.whiteToMove = true;
                break;
            } else {
                cout << "Illegal black move! Try again.\n";
            }
        } while (true);
    }
}

