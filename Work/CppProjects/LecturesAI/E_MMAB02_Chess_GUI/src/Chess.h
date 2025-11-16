#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <td/Types.h>
#include <gui/Canvas.h>
#include <thread>
#include <mutex>
#include <gui/Thread.h>
#include <thread/Thread.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <gui/Application.h>

constexpr size_t N = 8;

class Chess
{
public:
    enum class UserMove : td::BYTE {None=0, ClickedBK, DragBK};
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
        , bpx(-1)
        , bpy(-1)
        , whiteToMove(true)
        {}
    };
private:
    Position _state = {'f', 1, 'c', 2, 'c', 4};
    gui::Image _imgBoard;
    gui::Image _blackKing;
    gui::Image _whiteKing;
    gui::Image _whitePawn;
    gui::Image _whiteQueen;
    std::thread _workingThread;
    std::mutex _mutex;
    gui::Rect _boardPlacement;
    gui::Size _boardImgSize;
    gui::Point _ptOrig;
    gui::Point _dragStartPoint;
    gui::Point _currentDragPoint;
    gui::Size _cellSize;
    const float _originXMargin = 0.045f;   //relative to biard width, after fine-tuning
    const float _originYMargin = 0.045f;    //relative to board height
    UserMove _userMove = UserMove::None;
    bool _playing = false;
    bool _stopPlaying = false;
    bool _calcBoardPlacement = true;
    bool _blackKingDragging = false;
    
    gui::Size _viewSize;
    int _depth;
public:

private:
    
    bool isInside(int x, int y) { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    bool sameSquare(int x1, int y1, int x2, int y2) { return x1 == x2 && y1 == y2; }


    bool kingsAdjacent(const Position& pos) {
        return math::abs(pos.wkx - pos.bkx) <= 1 && math::abs(pos.wky - pos.bky) <= 1;
    }

    bool isPawnProtected(int kx, int ky, int px, int py) {
        return math::abs(kx - px) <= 1 && math::abs(ky - py) <= 1;
    }

    bool isLegal(const Position& pos)
    {
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
        if (pos.wpx != -1)
        {
            bool bProtected = isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy);
            int distWK_P = math::abs(pos.wkx - pos.wpx) + math::abs(pos.wky - pos.wpy); // White King → White Pawn
            int distBP_P = math::abs(pos.bkx - pos.wpx) + math::abs(pos.bky - pos.wpy); // Black King → White Pawn

            // If pawn is unprotected, it must be closer to white king than black king
            if (!bProtected && distWK_P >= distBP_P) {
                score -= 5000; // large penalty for unsafe pawn
            }

            // Optional: small bonus if pawn is safely supported
            if (bProtected) {
                score += 100; // already in your code
            }
        }

        // -------------------------
        // Promotion logic
        // -------------------------
        if (pos.wpx != -1 && pos.wpy == 7) {
            // Check protection and enemy king distance
            int distBK = std::min(math::abs(pos.bkx - pos.wpx), math::abs(pos.bky - pos.wpy));
            bool safe = isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy) && (distBK > 1);
            if (safe)
                score += 100000; // winning promotion
            else
                score -= 5000; // unprotected promoted pawn (lost)
        }
        if (pos.bpx != -1 && pos.bpy == 0) {
            int distWK = math::abs(pos.wkx - pos.bpx) + math::abs(pos.wky - pos.bpy);
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
        if (math::abs(pos.bkx - nx) <= 1 && math::abs(pos.bky - ny) <= 1)
            return true;

        // Check black pawn capture squares
        if (pos.bpx != -1) {
            if ((pos.bpy - 1 == ny) && (math::abs(pos.bpx - nx) == 1)) // black pawn can capture
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
                bool blocked1 = sameSquare(pos.wpx, pos.wpy + 1, pos.bkx, pos.bky) || sameSquare(pos.wpx, pos.wpy + 1, pos.bpx, pos.bpy);
                if (!blocked1) {
                    int nx = pos.wpx;
                    int ny = pos.wpy + 1;

                    // Check if pawn would be protected by white king
                    bool protectedByWK = math::abs(pos.wkx - nx) <= 1 && math::abs(pos.wky - ny) <= 1;

                    // Check if pawn would be attacked by black king or pawn
                    bool attackedByBK = math::abs(pos.bkx - nx) <= 1 && math::abs(pos.bky - ny) <= 1;
                    bool attackedByBP = (pos.bpx != -1) && (pos.bpy - 1 == ny) && (math::abs(pos.bpx - nx) == 1);

                    // Distance condition if pawn is unprotected
                    int distWK_P = math::abs(pos.wkx - nx) + math::abs(pos.wky - ny);
                    int distBK_P = math::abs(pos.bkx - nx) + math::abs(pos.bky - ny);

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
                    bool blocked2a = sameSquare(pos.wpx, pos.wpy + 1, pos.bkx, pos.bky) || sameSquare(pos.wpx, pos.wpy + 1, pos.bpx, pos.bpy);
                    bool blocked2b = sameSquare(pos.wpx, pos.wpy + 2, pos.bkx, pos.bky) || sameSquare(pos.wpx, pos.wpy + 2, pos.bpx, pos.bpy);
                    if (!blocked2a && !blocked2b) {
                        int nx = pos.wpx;
                        int ny = pos.wpy + 2;
                        bool protectedByWK = math::abs(pos.wkx - nx) <= 1 && math::abs(pos.wky - ny) <= 1;
                        bool attackedByBK = math::abs(pos.bkx - nx) <= 1 && math::abs(pos.bky - ny) <= 1;
                        bool attackedByBP = (pos.bpx != -1) && (pos.bpy - 1 == ny) && (math::abs(pos.bpx - nx) == 1);
                        int distWK_P = math::abs(pos.wkx - nx) + math::abs(pos.wky - ny);
                        int distBK_P = math::abs(pos.bkx - nx) + math::abs(pos.bky - ny);

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
                    bool protectedByWK = math::abs(np.wkx - pos.wpx) <= 1 && math::abs(np.wky - pos.wpy) <= 1;
                    int distWK_P = math::abs(np.wkx - pos.wpx) + math::abs(np.wky - pos.wpy);
                    int distBK_P = math::abs(pos.bkx - pos.wpx) + math::abs(pos.bky - pos.wpy);
                    if (!protectedByWK && distWK_P > distBK_P)
                        continue; // skip king move leaving pawn too vulnerable
                }

                if (isLegal(np))
                    moves.push_back(np);
            }

            // remove identical positions
            moves.erase(remove_if(moves.begin(), moves.end(),
                                  [&](const Position& m) {
                                      return m.wkx == pos.wkx && m.wky == pos.wky &&  m.wpx == pos.wpx && m.wpy == pos.wpy;
                                  }),
                        moves.end());
        }
        else
        {
            // -----------------
            // Black King moves
            // -----------------
            for (auto& d : dirs)
            {
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
            if (pos.bpx != -1 && pos.bpy > 0)
            {
                bool blocked1 = sameSquare(pos.bpx, pos.bpy - 1, pos.wkx, pos.wky) || sameSquare(pos.bpx, pos.bpy - 1, pos.wpx, pos.wpy);
                if (!blocked1)
                {
                    Position np = pos;
                    np.bpy -= 1;
                    np.whiteToMove = true;
                    if (isLegal(np))
                        moves.push_back(np);
                }

                // two squares from initial rank (rank 7 = bpy == 6)
                if (pos.bpy == 6)
                {
                    bool blocked2a = sameSquare(pos.bpx, pos.bpy - 1, pos.wkx, pos.wky) ||
                                     sameSquare(pos.bpx, pos.bpy - 1, pos.wpx, pos.wpy);
                    bool blocked2b = sameSquare(pos.bpx, pos.bpy - 2, pos.wkx, pos.wky) ||
                                     sameSquare(pos.bpx, pos.bpy - 2, pos.wpx, pos.wpy);
                    if (!blocked2a && !blocked2b)
                    {
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

    bool isPromotion(const Position& p)
    {
        if (p.wpx != -1 && p.wpy == 7)
        {
            bool safe = isPawnProtected(p.wkx, p.wky, p.wpx, p.wpy) &&
                        (math::abs(p.bkx - p.wpx) + math::abs(p.bky - p.wpy) > 1);
            return safe;
        }
        if (p.bpx != -1 && p.bpy == 0)
        {
            bool safe = isPawnProtected(p.bkx, p.bky, p.bpx, p.bpy) &&
                        (math::abs(p.wkx - p.bpx) + math::abs(p.wky - p.bpy) > 1);
            return safe;
        }
        return false;
    }

    bool isStalemate(const Position& pos) {
        auto moves = generateMoves(pos);
        if (moves.empty()) return true;
        return false;
    }


    //Alpha-beta with better terminal detection
    int alphaBeta(Position pos, int depth, int alpha, int beta, bool maximizingPlayer)
    {
        // Check if white pawn was captured
        if (pos.wpx == -1) {
            return -100000; // Black won by capturing pawn
        }

        // Check for promotion
        if (pos.wpx != -1 && pos.wpy == 7) {
            int distBK = abs(pos.bkx - pos.wpx) + abs(pos.bky - pos.wpy);
            bool bProtected = isPawnProtected(pos.wkx, pos.wky, pos.wpx, pos.wpy);
            if (bProtected && distBK > 1) {
                return 100000; // White wins
            }
        }
        
        // Check for stalemate
        std::vector<Position> moves = generateMoves(pos);
        if (moves.empty())
        {
            // No legal moves - stalemate
            // If it's white's turn and no moves, black stalemated white (unlikely but check)
            // If it's black's turn and no moves, white stalemated black
            return pos.whiteToMove ? -100000 : 100000;
        }

        // Depth limit reached
        if (depth == 0) {
            return evaluate(pos);
        }

        if (maximizingPlayer) {
            int maxEval = math::minVal<int>();
            for (auto& m : moves) {
                int eval = alphaBeta(m, depth - 1, alpha, beta, false);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha)
                    break; // Beta cutoff
            }
            return maxEval;
        }
        else {
            int minEval = math::maxVal<int>();
            for (auto& m : moves) {
                int eval = alphaBeta(m, depth - 1, alpha, beta, true);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha)
                    break; // Alpha cutoff
            }
            return minEval;
        }
    }

    void getCellRect(gui::Rect& rect,  td::INT2 x, td::INT2 y) const
    {
        gui::Point tl(_ptOrig);
        tl.x += x*_cellSize.width;
        tl.y -= (y) * _cellSize.height;
        rect.left = tl.x;
        rect.top = tl.y;
        rect.right = tl.x + _cellSize.width;
        rect.bottom = tl.y + _cellSize.height;
    }
    
    td::Point<td::INT2> getCellCoordinate(const gui::Point& cursorPoint) const
    {
        td::INT2 x = td::INT2((cursorPoint.x - _ptOrig.x) / _cellSize.width);
        td::INT2 y = td::INT2((_ptOrig.y - cursorPoint.y) / _cellSize.height) + 1;
        td::Point<td::INT2> coord(x, y);
        return coord;
    }
    
public:
    Chess(int depth)
    : _imgBoard(":board")
    , _blackKing(":blackKing")
    , _whiteKing(":whiteKing")
    , _whitePawn(":whitePawn")
    , _whiteQueen(":whiteQueen")
    , _depth(depth)
    {
        reset();
        _imgBoard.getSize(_boardImgSize);
    }
    
    bool isBlackMoveLegal(const Position& pos, int nx, int ny)
    {
        if (!isInside(nx, ny))
            return false;

        // King moves only one square in any direction
        if (math::abs(nx - pos.bkx) > 1 || math::abs(ny - pos.bky) > 1)
            return false;

        // Cannot stay on same square
        if (sameSquare(nx, ny, pos.bkx, pos.bky))
            return false;

        // Cannot move onto own pawn
        if (pos.bpx != -1 && sameSquare(nx, ny, pos.bpx, pos.bpy))
            return false;

        // Cannot move next to the white king
        if (math::abs(nx - pos.wkx) <= 1 && math::abs(ny - pos.wky) <= 1)
            return false;

        // Cannot move into a square attacked by white pawn
        if (pos.wpx != -1)
        {
            if ((nx == pos.wpx - 1 && ny == pos.wpy + 1) || (nx == pos.wpx + 1 && ny == pos.wpy + 1))
                return false;
        }

        // Cannot move onto the white pawn (would be capture, not allowed)
        if (pos.wpx != -1 && sameSquare(nx, ny, pos.wpx, pos.wpy))
            return false;

        return true;
    }
    
    void findBestMove(const gui::thread::MainThreadSharedFunction1& callBack)
    {
        std::vector<Position> moves = generateMoves(_state);
        Position best = _state;
        int bestScore = math::minVal<int>();

        for (auto& m : moves)
        {
            if (_stopPlaying)
            {
                td::INT4 iMsg = td::INT4(Message::Stop);
                td::Variant varMsg(iMsg);
                gui::thread::asyncExecInMainThread(callBack, varMsg);
                _playing = false;
                _stopPlaying = false;
                return;
            }
            int score = alphaBeta(m, _depth - 1, math::minVal<int>(), math::maxVal<int>(), false);
            if (score > bestScore)
            {
                bestScore = score;
                best = m;
            }
        }
        _state = best;
        _state.whiteToMove = false;
        td::INT4 iMsg = td::INT4(Message::AIMove);
        if (_state.wpy == 7) //promoted to queen
        {
            iMsg = td::INT4(Message::StopOnGoal);
            _playing = false;
        }
        td::Variant varMsg(iMsg);
        gui::thread::asyncExecInMainThread(callBack, varMsg);
        
        mu::dbgLog("Best evaluation: %d", bestScore);
    }
    
    void lock()
    {
        _mutex.lock();
    }
    
    void unlock()
    {
        _mutex.unlock();
    }
    
    void reset(bool setToInitialState = false)
    {
        _stopPlaying = false;
        if (setToInitialState)
            _state = {'f', 1, 'c', 2, 'c', 4};
    }
    
    void draw()
    {
        gui::Point pt(0,0);
        gui::Rect r(pt, _viewSize);
        if (_calcBoardPlacement)
            _imgBoard.draw(r, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center, &_boardPlacement);
        else
            _imgBoard.draw(r);
        
        gui::CoordType origX = _boardPlacement.left + _originXMargin * _boardPlacement.width();
        gui::CoordType origY = _boardPlacement.bottom - _originYMargin * _boardPlacement.height();
        
        gui::CoordType matrixRight = _boardPlacement.right - _originXMargin * _boardPlacement.width();
        gui::CoordType matrixTop = _boardPlacement.top + _originYMargin * _boardPlacement.height();
        
        gui::Rect rMatrix(origX, matrixTop, matrixRight, origY);
        gui::Shape::drawRect(rMatrix, td::ColorID::White, 2.5f);  // use white
        
        gui::CoordType cellWidth = (matrixRight - origX) / N;
        gui::CoordType cellHeight = (origY - matrixTop) / N;
        
        
        _ptOrig = {origX, origY - cellHeight};
        _cellSize = {cellWidth, cellHeight};
        //draw pieces
        gui::Rect rect;
        if (_state.bkx >= 0)
        {
            getCellRect(rect,_state.bkx, _state.bky);
            if (_userMove == UserMove::DragBK)
            {
                auto pt = _currentDragPoint - _dragStartPoint;
                rect += pt;
            }
            _blackKing.draw(rect);
        }
        
        if (_state.wkx >= 0)
        {
            getCellRect(rect, _state.wkx, _state.wky);
            _whiteKing.draw(rect);
        }
        
        if (_state.wpx >= 0)
        {
            getCellRect(rect, _state.wpx, _state.wpy);
            if (_state.wpy == 7)    //if promoted draw white queen
                _whiteQueen.draw(rect);
            else
                _whitePawn.draw(rect);
        }
    }
    
    std::thread& getThread()
    {
        return _workingThread;
    }
    
    void stopPlaying()
    {
        if (!_state.whiteToMove)
        {
            _playing = false;
            return;
        }
        _stopPlaying = true;
    }
    
    bool isPlaying() const
    {
        return _playing;
    }
    
    void updateModelSize(const gui::Size& newSize)
    {
        _calcBoardPlacement = true;
        _viewSize = newSize;
    }
    
    //just black king is move-able (should be enhanced)
    bool canDragPiece(const gui::Point& cursorPoint)
    {
        if (_playing)
        {
            if (_state.whiteToMove)
                return false;
        }
        
        auto cell =  getCellCoordinate(cursorPoint);
        if (cell.x == _state.bkx && cell.y == _state.bky)
        {
            _userMove = UserMove::ClickedBK;
            _dragStartPoint = cursorPoint;
            return true;
        }
        _userMove = UserMove::None;
        return false;
    }
    
    bool finishUserMove(const gui::Point& cursorPoint)
    {
        if (_userMove == UserMove::DragBK)
        {
            auto cell =  getCellCoordinate(cursorPoint);
            if (isBlackMoveLegal(_state, cell.x, cell.y))
            {
                //place king here
                _state.bkx = cell.x;
                _state.bky = cell.y;
                _state.whiteToMove = true;
            }
            //keep king on original place
            _userMove = UserMove::None;
            return true;
           
        }
        _userMove = UserMove::None;
        return false;
    }
    
    bool testDrag(const gui::Point& cursorPoint)
    {
        if (_userMove == UserMove::ClickedBK)
        {
            _currentDragPoint = cursorPoint;
            _userMove = UserMove::DragBK;
            return true;
        }
        else if (_userMove == UserMove::DragBK)
        {
            _currentDragPoint = cursorPoint;
            return true;
        }
        _userMove = UserMove::None;
        return false;
    }
    
    void start()
    {
        if (!_playing)
            _playing = true;
        _state.whiteToMove = true;
    }
    
    bool canAIMakeMove() const
    {
        return _playing && _state.whiteToMove;
    }
    
    void moveAI(gui::thread::MainThreadSharedFunction1 callBack)
    {
        //for each new AI move, fire new thread
        _workingThread = std::thread(&Chess::findBestMove, this, callBack);
    }
    
};


