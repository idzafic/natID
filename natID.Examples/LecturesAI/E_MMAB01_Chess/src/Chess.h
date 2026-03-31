#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <td/Types.h>

class Chess
{
    int _depth;
public:
    struct Position {
        td::INT2 wkx, wky; // White King
        td::INT2 wpx, wpy; // White Pawn (-1,-1) if none
        td::INT2 bkx, bky; // Black King
        td::INT2 bpx, bpy; // Black Pawn (-1,-1) if none
        td::INT2 whiteToMove;

        Position(char wkCol, td::INT2 wkRow, char wpCol, td::INT2 wpRow, char bkCol, td::INT2 bkRow)
        : wkx(wkCol - 'a'), wky(wkRow - 1)
        , wpx(wpCol == '-' ? -1 : wpCol - 'a')
        , wpy(wpRow == 0 ? -1 : wpRow - 1)
        , bkx(bkCol - 'a'), bky(bkRow - 1)
        , bpx(-1), bpy(-1), whiteToMove(true)
        {}
    };
private:
    
    bool isInside(int x, int y) { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    bool sameSquare(int x1, int y1, int x2, int y2) { return x1 == x2 && y1 == y2; }


    bool kingsAdjacent(const Position& pos) {
        return abs(pos.wkx - pos.bkx) <= 1 && abs(pos.wky - pos.bky) <= 1;
    }

    bool isPawnProtected(int kx, int ky, int px, int py) {
        return abs(kx - px) <= 1 && abs(ky - py) <= 1;
    }

    bool isLegal(const Position& pos) {
        if (!isInside(pos.wkx, pos.wky) || !isInside(pos.bkx, pos.bky))
            return false;
        if (pos.wpx != -1 && !isInside(pos.wpx, pos.wpy))
            return false;
        if (pos.bpx != -1 && !isInside(pos.bpx, pos.bpy))
            return false;

        if (sameSquare(pos.wkx, pos.wky, pos.bkx, pos.bky))
            return false;
        if (kingsAdjacent(pos))
            return false;
        if (pos.wpx != -1 && sameSquare(pos.wkx, pos.wky, pos.wpx, pos.wpy))
            return false;
        if (pos.bpx != -1 && sameSquare(pos.bkx, pos.bky, pos.bpx, pos.bpy))
            return false;
        return true;
    }

    // ---------------------------------------------------------------------------
    // Evaluation: combine material, advancement, king cooperation, and stalemate
    // ---------------------------------------------------------------------------

    int evaluate(const Position& pos) {
        // Check legality and basic draw
        if (kingsAdjacent(pos))
            return 0;

        int score = 0;

        // -------------------------
        // Material
        // -------------------------
    //    if (pos.wpx != -1)
    //        score += 100;
    //    if (pos.bpx != -1)
    //        score -= 100;

        // -------------------------
        // Pawn advancement
        // -------------------------
        if (pos.wpx != -1)
            score += pos.wpy * 30;
        if (pos.bpx != -1)
            score -= (7 - pos.bpy) * 30;

        // -------------------------
        // King–pawn coordination
        // -------------------------
    //    if (pos.wpx != -1) {
    //        int dist = abs(pos.wkx - pos.wpx) + abs(pos.wky - pos.wpy);
    //        score -= dist * 10; // small penalty for being far
    //    }
        
        // -------------------------
        // Pawn safety: distance rule
        // -------------------------
        if (pos.wpx != -1) {
            bool bProtected = isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy);
            int distWK_P = abs(pos.wkx - pos.wpx) + abs(pos.wky - pos.wpy); // White King → White Pawn
            int distBP_P = abs(pos.bkx - pos.wpx) + abs(pos.bky - pos.wpy); // Black King → White Pawn

            // If pawn is unprotected, it must be closer to white king than black king
            if (!bProtected && distWK_P > distBP_P) {
                score -= 5000; // large penalty for unsafe pawn
            }

            // Optional: small bonus if pawn is safely supported
            if (bProtected) {
                score += 100; // already in your code
            }
        }

        
        // King proximity to own pawn
//        if (pos.wpx != -1)
//        {
//            auto dx = abs(pos.wkx - pos.wpx);
//            if (dx)
//                --dx;
//            auto dy = abs(pos.wky - pos.wpy);
//            if (dy)
//                --dy;
//            
//            if (dx > 0 || dy > 0)
//                score -= ((dx + dy) * 10000);
//        }
        
    //    if (pos.bpx != -1) {
    //        int dist = abs(pos.bkx - pos.bpx) + abs(pos.bky - pos.bpy);
    //        score += dist * 10; // black king far from pawn = good for white
    //    }

        // -------------------------
        // Pawn protection bonus
        // -------------------------
//        if (pos.wpx != -1 && isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy))
//            score += 2000; // solid support
//        else if (pos.wpx != -1)
//            score -= 2000; // unprotected pawn = risky

        // -------------------------
        // Promotion logic
        // -------------------------
        if (pos.wpx != -1 && pos.wpy == 7) {
            // Check protection and enemy king distance
            int distBK = std::min(abs(pos.bkx - pos.wpx), abs(pos.bky - pos.wpy));
            bool safe = isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy) && (distBK > 1);
            if (safe)
                score += 100000; // winning promotion
            else
                score -= 5000; // unprotected promoted pawn (lost)
        }
        if (pos.bpx != -1 && pos.bpy == 0) {
            int distWK = abs(pos.wkx - pos.bpx) + abs(pos.wky - pos.bpy);
            bool safe = isPawnProtected(pos.bkx, pos.bky, pos.bpx, pos.bpy) && distWK > 1;
            if (safe)
                score -= 100000;
            else
                score += 5000;
        }

        // -------------------------
        // Opposition / kings too close
        // -------------------------
        if (kingsAdjacent(pos))
            score -= 1000;

        return score;
    }

    bool willBeAttacked(const Position& pos, int nx, int ny) {
        // Check black king adjacency
        if (abs(pos.bkx - nx) <= 1 && abs(pos.bky - ny) <= 1)
            return true;

        // Check black pawn capture squares
        if (pos.bpx != -1) {
            if ((pos.bpy - 1 == ny) && (abs(pos.bpx - nx) == 1)) // black pawn can capture
                return true;
        }

        return false;
    }
    
    
    // ---------------------------------------------------------------------------
    // Generate all legal white and black moves (no-op excluded)
    // ---------------------------------------------------------------------------
    std::vector<Position> generateMoves(const Position& pos) {
        std::vector<Position> moves;
        const td::INT2 dirs[8][2] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };

        if (pos.whiteToMove) {
            // -----------------
            // White Pawn moves
            // -----------------
            if (pos.wpx != -1 && pos.wpy < 7) {
                // one square ahead must be empty (no king or pawn)
                bool blocked1 = sameSquare(pos.wpx, pos.wpy + 1, pos.bkx, pos.bky) ||
                                sameSquare(pos.wpx, pos.wpy + 1, pos.bpx, pos.bpy);
                if (!blocked1) {
                    int nx = pos.wpx;
                    int ny = pos.wpy + 1;

                    // Check if pawn would be protected by white king
                    bool protectedByWK = abs(pos.wkx - nx) <= 1 && abs(pos.wky - ny) <= 1;

                    // Check if pawn would be attacked by black king or pawn
                    bool attackedByBK = abs(pos.bkx - nx) <= 1 && abs(pos.bky - ny) <= 1;
                    bool attackedByBP = (pos.bpx != -1) && (pos.bpy - 1 == ny) && (abs(pos.bpx - nx) == 1);

                    // Distance condition if pawn is unprotected
                    int distWK_P = abs(pos.wkx - nx) + abs(pos.wky - ny);
                    int distBK_P = abs(pos.bkx - nx) + abs(pos.bky - ny);

                    if (protectedByWK || (!attackedByBK && !attackedByBP && distWK_P <= distBK_P)) {
                        Position np = pos;
                        np.wpy += 1;
                        np.whiteToMove = false;
                        if (isLegal(np))
                            moves.push_back(np);
                    }
                }

                // two squares ahead from rank 2 (wpy == 1)
                if (pos.wpy == 1) {
                    bool blocked2a = sameSquare(pos.wpx, pos.wpy + 1, pos.bkx, pos.bky) ||
                                     sameSquare(pos.wpx, pos.wpy + 1, pos.bpx, pos.bpy);
                    bool blocked2b = sameSquare(pos.wpx, pos.wpy + 2, pos.bkx, pos.bky) ||
                                     sameSquare(pos.wpx, pos.wpy + 2, pos.bpx, pos.bpy);
                    if (!blocked2a && !blocked2b) {
                        int nx = pos.wpx;
                        int ny = pos.wpy + 2;
                        bool protectedByWK = abs(pos.wkx - nx) <= 1 && abs(pos.wky - ny) <= 1;
                        bool attackedByBK = abs(pos.bkx - nx) <= 1 && abs(pos.bky - ny) <= 1;
                        bool attackedByBP = (pos.bpx != -1) && (pos.bpy - 1 == ny) && (abs(pos.bpx - nx) == 1);
                        int distWK_P = abs(pos.wkx - nx) + abs(pos.wky - ny);
                        int distBK_P = abs(pos.bkx - nx) + abs(pos.bky - ny);

                        if (protectedByWK || (!attackedByBK && !attackedByBP && distWK_P <= distBK_P)) {
                            Position np2 = pos;
                            np2.wpy += 2;
                            np2.whiteToMove = false;
                            if (isLegal(np2))
                                moves.push_back(np2);
                        }
                    }
                }
            }

            // -----------------
            // White King moves
            // -----------------
            for (auto& d : dirs) {
                Position np = pos;
                np.wkx += d[0];
                np.wky += d[1];
                np.whiteToMove = false;

                if (!isInside(np.wkx, np.wky)) continue;
                if (sameSquare(np.wkx, np.wky, pos.wkx, pos.wky)) continue;
                if (pos.wpx != -1 && sameSquare(np.wkx, np.wky, pos.wpx, pos.wpy))
                    continue;

                // White pawn safety: distance condition
                if (pos.wpx != -1) {
                    bool protectedByWK = abs(np.wkx - pos.wpx) <= 1 && abs(np.wky - pos.wpy) <= 1;
                    int distWK_P = abs(np.wkx - pos.wpx) + abs(np.wky - pos.wpy);
                    int distBK_P = abs(pos.bkx - pos.wpx) + abs(pos.bky - pos.wpy);
                    if (!protectedByWK && distWK_P > distBK_P)
                        continue; // skip king move leaving pawn too vulnerable
                }

                if (isLegal(np))
                    moves.push_back(np);
            }

            // remove identical positions
            moves.erase(remove_if(moves.begin(), moves.end(),
                                  [&](const Position& m) {
                                      return m.wkx == pos.wkx && m.wky == pos.wky &&
                                             m.wpx == pos.wpx && m.wpy == pos.wpy;
                                  }),
                        moves.end());
        } else {
            // -----------------
            // Black King moves
            // -----------------
            for (auto& d : dirs) {
                Position np = pos;
                np.bkx += d[0];
                np.bky += d[1];
                np.whiteToMove = true;

                if (!isInside(np.bkx, np.bky)) continue;
                if (sameSquare(np.bkx, np.bky, pos.bkx, pos.bky)) continue;
                if (isLegal(np))
                    moves.push_back(np);
            }

            // -----------------
            // Black Pawn moves
            // -----------------
            if (pos.bpx != -1 && pos.bpy > 0) {
                bool blocked1 = sameSquare(pos.bpx, pos.bpy - 1, pos.wkx, pos.wky) ||
                                sameSquare(pos.bpx, pos.bpy - 1, pos.wpx, pos.wpy);
                if (!blocked1) {
                    Position np = pos;
                    np.bpy -= 1;
                    np.whiteToMove = true;
                    if (isLegal(np))
                        moves.push_back(np);
                }

                // two squares from initial rank (rank 7 = bpy == 6)
                if (pos.bpy == 6) {
                    bool blocked2a = sameSquare(pos.bpx, pos.bpy - 1, pos.wkx, pos.wky) ||
                                     sameSquare(pos.bpx, pos.bpy - 1, pos.wpx, pos.wpy);
                    bool blocked2b = sameSquare(pos.bpx, pos.bpy - 2, pos.wkx, pos.wky) ||
                                     sameSquare(pos.bpx, pos.bpy - 2, pos.wpx, pos.wpy);
                    if (!blocked2a && !blocked2b) {
                        Position np2 = pos;
                        np2.bpy -= 2;
                        np2.whiteToMove = true;
                        if (isLegal(np2))
                            moves.push_back(np2);
                    }
                }
            }
        }

        return moves;
    }


    bool isPromotion(const Position& p) {
        if (p.wpx != -1 && p.wpy == 7) {
            bool safe = isPawnProtected(p.wkx, p.wky, p.wpx, p.wpy) &&
                        (abs(p.bkx - p.wpx) + abs(p.bky - p.wpy) > 1);
            return safe;
        }
        if (p.bpx != -1 && p.bpy == 0) {
            bool safe = isPawnProtected(p.bkx, p.bky, p.bpx, p.bpy) &&
                        (abs(p.wkx - p.bpx) + abs(p.wky - p.bpy) > 1);
            return safe;
        }
        return false;
    }

    bool isStalemate(const Position& pos) {
        auto moves = generateMoves(pos);
        if (moves.empty()) return true;
        return false;
    }

    // ---------------------------------------------------------------------------
    int alphaBeta(Position pos, int depth, int alpha, int beta, bool maximizingPlayer) {
        std::vector<Position> moves = generateMoves(pos);
    //    if (depth == 0 || moves.empty())
    //        return evaluate(pos);
        
        // Terminal conditions
            if (isPromotion(pos))
                return (pos.wpy == 7 ? 100000 : -100000);
        
            if (isStalemate(pos))
            {
                // If it's black's turn and no legal moves -> black is stalemated (win for white)
                return pos.whiteToMove ? -100000 : 100000;
            }

            if (depth == 0)
                return evaluate(pos);
        

        if (maximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (auto& m : moves) {
                int eval = alphaBeta(m, depth - 1, alpha, beta, false);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break;
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (auto& m : moves) {
                int eval = alphaBeta(m, depth - 1, alpha, beta, true);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) break;
            }
            return minEval;
        }
    }

public:
    Chess(int depth)
    : _depth(depth)
    {}
    
    bool isBlackMoveLegal(const Position& pos, int nx, int ny) {
        if (!isInside(nx, ny))
            return false;

        // King moves only one square in any direction
        if (abs(nx - pos.bkx) > 1 || abs(ny - pos.bky) > 1)
            return false;

        // Cannot stay on same square
        if (sameSquare(nx, ny, pos.bkx, pos.bky))
            return false;

        // Cannot move onto own pawn
        if (pos.bpx != -1 && sameSquare(nx, ny, pos.bpx, pos.bpy))
            return false;

        // Cannot move next to the white king
        if (abs(nx - pos.wkx) <= 1 && abs(ny - pos.wky) <= 1)
            return false;

        // Cannot move into a square attacked by white pawn
        if (pos.wpx != -1) {
            if ((nx == pos.wpx - 1 && ny == pos.wpy + 1) ||
                (nx == pos.wpx + 1 && ny == pos.wpy + 1))
                return false;
        }

        // Cannot move onto the white pawn (would be capture, not allowed)
        if (pos.wpx != -1 && sameSquare(nx, ny, pos.wpx, pos.wpy))
            return false;

        return true;
    }
    
    
    void printBoard(const Position& pos) {
        char board[8][8];
        for (int y = 0; y < 8; ++y)
            for (int x = 0; x < 8; ++x)
                board[y][x] = '.';

        // White King
        board[pos.wky][pos.wkx] = 'K';

        // White Pawn / Queen
        if (pos.wpx != -1) {
            if (pos.wpy == 7)
                board[pos.wpy][pos.wpx] = 'Q'; // promoted pawn
            else
                board[pos.wpy][pos.wpx] = 'P';
        }

        // Black King
        board[pos.bky][pos.bkx] = 'k';

        // Black Pawn / Queen
        if (pos.bpx != -1) {
            if (pos.bpy == 0)
                board[pos.bpy][pos.bpx] = 'q'; // promoted pawn
            else
                board[pos.bpy][pos.bpx] = 'p';
        }

        std::cout << "\n    a b c d e f g h\n";
        std::cout << "   -----------------\n";
        for (int y = 7; y >= 0; --y) {
            std::cout << y + 1 << " | ";
            for (int x = 0; x < 8; ++x)
                std::cout << board[y][x] << " ";
            std::cout << "| " << y + 1 << "\n";
        }
        std::cout << "   -----------------\n";
        std::cout << "    a b c d e f g h\n";
    }

    
    Position findBestMove(Position pos) {
        std::vector<Position> moves = generateMoves(pos);
        Position best = pos;
        int bestScore = std::numeric_limits<int>::min();

        for (auto& m : moves) {
            int score = alphaBeta(m, _depth - 1,
                                  std::numeric_limits<int>::min(),
                                  std::numeric_limits<int>::max(), false);
            if (score > bestScore) {
                bestScore = score;
                best = m;
            }
        }

        std::cout << "Best evaluation: " << bestScore << std::endl;
        return best;
    }
};

