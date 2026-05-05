// Pieces.cpp
#include "Pieces.h"

// Piece base class
Piece::Piece(Color c, char s) : color(c), symbol(s) {}

Color Piece::getColor() const {
	return color;
}

char Piece::getSymbol() const {
	return symbol;
}

// Knight
Knight::Knight(Color c) : Piece(c, 'N') {}

bool Knight::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	if ((nRow == cRow - 2 && nCol == cCol + 1) ||
		(nRow == cRow - 1 && nCol == cCol + 2) ||
		(nRow == cRow - 2 && nCol == cCol - 1) ||
		(nRow == cRow - 1 && nCol == cCol - 2) ||
		(nRow == cRow + 2 && nCol == cCol + 1) ||
		(nRow == cRow + 1 && nCol == cCol + 2) ||
		(nRow == cRow + 2 && nCol == cCol - 1) ||
		(nRow == cRow + 1 && nCol == cCol - 2)) {
		return true;
	}
	return false;
}

// King
King::King(Color c) : Piece(c, 'K') {}

bool King::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	if ((nRow == cRow - 1 && nCol == cCol) ||
		(nRow == cRow + 1 && nCol == cCol) ||
		(nRow == cRow && nCol == cCol + 1) ||
		(nRow == cRow && nCol == cCol - 1) ||
		(nRow == cRow + 1 && nCol == cCol + 1) ||
		(nRow == cRow - 1 && nCol == cCol + 1) ||
		(nRow == cRow + 1 && nCol == cCol - 1) ||
		(nRow == cRow - 1 && nCol == cCol - 1)) {
		return true;
	}
	return false;
}

// Pawn
Pawn::Pawn(Color c) : Piece(c, 'P') {}

bool Pawn::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	if (board[cRow][cCol]->getColor() == WHITE) {
		// Move forward one square
		if (board[nRow][nCol] == nullptr && nRow == cRow - 1 && nCol == cCol) {
			return true;
		}
		// Move forward two squares from starting row
		if (board[nRow][nCol] == nullptr && board[cRow - 1][cCol] == nullptr
			&& nRow == cRow - 2 && nCol == cCol && cRow == 6) {
			return true;
		}
		// Diagonal capture of enemy piece
		if (board[nRow][nCol] != nullptr && board[nRow][nCol]->getColor() != WHITE
			&& nRow == cRow - 1 && (nCol == cCol + 1 || nCol == cCol - 1)) {
			return true;
		}
		// En passant capture
		if (enPassantRow == nRow && enPassantCol == nCol
			&& nRow == cRow - 1 && (nCol == cCol + 1 || nCol == cCol - 1)) {
			return true;
		}
	}
	if (board[cRow][cCol]->getColor() == BLACK) {
		// Move forward one square
		if (board[nRow][nCol] == nullptr && nRow == cRow + 1 && nCol == cCol) {
			return true;
		}
		// Move forward two squares from starting row
		if (board[nRow][nCol] == nullptr && board[cRow + 1][cCol] == nullptr
			&& nRow == cRow + 2 && nCol == cCol && cRow == 1) {
			return true;
		}
		// Diagonal capture of enemy piece
		if (board[nRow][nCol] != nullptr && board[nRow][nCol]->getColor() != BLACK
			&& nRow == cRow + 1 && (nCol == cCol + 1 || nCol == cCol - 1)) {
			return true;
		}
		// En passant capture
		if (enPassantRow == nRow && enPassantCol == nCol
			&& nRow == cRow + 1 && (nCol == cCol + 1 || nCol == cCol - 1)) {
			return true;
		}
	}
	return false;
}

// Rook
Rook::Rook(Color c) : Piece(c, 'R') {}

bool Rook::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	if (cRow == nRow) {
		// Moving left
		if (cCol > nCol) {
			for (int i = cCol - 1; i > nCol; i--) {
				if (board[cRow][i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		// Moving right
		if (cCol < nCol) {
			for (int i = cCol + 1; i < nCol; i++) {
				if (board[cRow][i] != nullptr) {
					return false;
				}
			}
			return true;
		}
	}
	if (cCol == nCol) {
		// Moving up
		if (cRow > nRow) {
			for (int i = cRow - 1; i > nRow; i--) {
				if (board[i][cCol] != nullptr) {
					return false;
				}
			}
			return true;
		}
		// Moving down
		if (cRow < nRow) {
			for (int i = cRow + 1; i < nRow; i++) {
				if (board[i][cCol] != nullptr) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

// Bishop
Bishop::Bishop(Color c) : Piece(c, 'B') {}

bool Bishop::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	int rowDiff = abs(nRow - cRow);
	int colDiff = abs(nCol - cCol);

	// Move must be perfectly diagonal
	if (rowDiff == colDiff) {
		// Down-right diagonal
		if (cRow < nRow && cCol < nCol) {
			for (int i = 1; i < rowDiff; i++) {
				if (board[cRow + i][cCol + i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		// Up-left diagonal
		if (cRow > nRow && cCol > nCol) {
			for (int i = 1; i < rowDiff; i++) {
				if (board[cRow - i][cCol - i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		// Up-right diagonal
		if (cRow > nRow && cCol < nCol) {
			for (int i = 1; i < rowDiff; i++) {
				if (board[cRow - i][cCol + i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		// Down-left diagonal
		if (cRow < nRow && cCol > nCol) {
			for (int i = 1; i < rowDiff; i++) {
				if (board[cRow + i][cCol - i] != nullptr) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

// Queen
Queen::Queen(Color c) : Piece(c, 'Q') {}

bool Queen::isValidMove(int cRow, int cCol,
	int nRow, int nCol, Piece* board[ROWS][COLS],
	int enPassantRow, int enPassantCol) {
	Rook rook(getColor());
	Bishop bishop(getColor());
	return rook.isValidMove(cRow, cCol, nRow, nCol, board, -1, -1) ||
		bishop.isValidMove(cRow, cCol, nRow, nCol, board, -1, -1);
}
