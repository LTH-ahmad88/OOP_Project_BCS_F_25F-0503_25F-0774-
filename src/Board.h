// Board.h
#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"

// Board class - manages the grid, piece movement, and game state
class Board {
	Piece* grid[ROWS][COLS];
	// Tracks the square where en passant capture is possible
	int enPassantRow;
	int enPassantCol;
public:
	// Initializes board and places all pieces in starting positions
	Board();

	// Frees all dynamically allocated pieces
	~Board();

	// Returns the piece at the given position
	Piece* getPiece(int row, int col) const;

	// Validates and executes a move, handles en passant and pawn promotion
	bool movePiece(int cRow, int cCol, int nRow, int nCol, Color currentPlayer);

	// Returns true if the given color's king is under attack
	bool isInCheck(Color c);

	// Returns true if the given color has no legal moves and is in check
	bool isCheckmate(Color c);

	// Returns true if the given color has no legal moves but is not in check
	bool isStalemate(Color c);

	// Fills validMoves array with all legal destination squares for the piece at row, col
	void getValidMoves(int row, int col, bool validMoves[8][8]);

	// Renders the board with cursor, selection, and valid move highlights using ANSI colors
	void displayBoard(int cursorRow, int cursorCol, int selRow, int selCol, bool validMoves[8][8]) const;
};

#endif
