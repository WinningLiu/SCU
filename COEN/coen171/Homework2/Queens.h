# ifndef QUEENS_H
# define QUEENS_H
# include <cstdlib>


// An abstract chess piece that cannot be instantiated.

class Piece {
protected:
    int _row, _column;

public:
    int row() const {
	return _row;
    }

    int column() const {
	return _column;
    }

    void place(int row, int column) {
	_row = row;
	_column = column;
    }

    virtual bool menaces(const Piece *p) const = 0;
};


// Your pieces go here
class rook: public Piece
{
    public:
        virtual bool menaces(const Piece *p) const;
};
class bishop: public Piece
{
    public:
        virtual bool menaces(const Piece *p) const;
};
class knight: public Piece
{
    public:
        virtual bool menaces(const Piece *p) const;
};
class queen: public rook, public bishop
{
    public:
        virtual bool menaces(const Piece *p) const;
};
class amazon: public queen, public knight
{
    public:
        virtual bool menaces(const Piece *p) const;
};
# endif /* QUEENS_H */