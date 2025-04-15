struct Position {
    int x, y;
    bool operator==(const Position& other) const {
        return this->x == other.x && this->y == other.y;
    }
};

// This is a Piece,
// the base class for all pieces
class Piece {
private:
    bool _is_white;

public:
    virtual bool isValidMove(Position src, Position dst) = 0;
    bool Color() { return _is_white; }

    Piece(bool is_white) : _is_white(is_white) {}
    ~Piece() {}
};

class King : public virtual Piece {
public:
    bool isValidMove(Position src, Position dst) override;
    King(bool is_white = true) : Piece(is_white) {}
    ~King() {}
};

// class Queen : public Piece {
// public:
//     bool isValidMove(tuple<int, int>, tuple<int, int>) override;
//     Queen(bool is_white = true) : Piece(is_white) {}
// };

class Rook : public virtual Piece {
public:
    bool isValidMove(Position src, Position dst) override;
    Rook(bool is_white = true) : Piece(is_white) {}
    ~Rook() {}
};

// class Knight : public Piece {
// public:
//     bool isValidMove(tuple<int, int>, tuple<int, int>) override;
//     Knight(bool is_white = true) : Piece(is_white) {}
// };
//
// class Bishop : public Piece {
// public:
//     bool isValidMove(tuple<int, int>, tuple<int, int>) override;
//     Bishop(bool is_white = true) : Piece(is_white) {}
// };
//
// class Pawn : public Piece {
// public:
//     bool isValidMove(tuple<int, int>, tuple<int, int>) override;
//     Pawn(bool is_white = true) : Piece(is_white) {}
// };
