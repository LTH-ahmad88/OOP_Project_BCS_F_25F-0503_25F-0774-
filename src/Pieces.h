// Pieces.h
#ifndef PIECES_H
#define PIECES_H

#include <iostream>
using namespace std;

enum Color {
	WHITE, BLACK
};
enum Size {
	ROWS = 8, COLS = 8
};

// Base class for all chess pieces
class Piece {
	Color color;
	char symbol;
public:
	Piece(Color c, char s);
	virtual ~Piece() {}

	// Pure virtual function - each piece must define its own move rules
	virtual bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) = 0;

	// Returns the color of the piece
	Color getColor() const;

	// Returns the symbol representing the piece
	char getSymbol() const;
};

// Knight piece - moves in an L shape
class Knight : public Piece {
public:
	Knight(Color c);

	// Checks all 8 possible L-shaped moves
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

// King piece - moves one square in any direction
class King : public Piece {
public:
	King(Color c);

	// Checks all 8 adjacent squares
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

// Pawn piece - moves forward, captures diagonally, supports en passant
class Pawn : public Piece {
public:
	Pawn(Color c);

	// Handles forward move, double move from start, diagonal capture, and en passant
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

// Rook piece - moves in straight lines horizontally or vertically
class Rook : public Piece {
public:
	Rook(Color c);

	// Checks horizontal and vertical paths and ensures no pieces are blocking
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

// Bishop piece - moves diagonally any number of squares
class Bishop : public Piece {
public:
	Bishop(Color c);

	// Checks all four diagonal directions and ensures no pieces are blocking
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

// Queen piece - combines rook and bishop movement
class Queen : public Piece {
public:
	Queen(Color c);

	// Delegates to rook and bishop logic
	bool isValidMove(int cRow, int cCol,
		int nRow, int nCol, Piece* board[ROWS][COLS],
		int enPassantRow, int enPassantCol) override;
};

#endif
