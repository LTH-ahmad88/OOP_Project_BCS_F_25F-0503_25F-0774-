// Board.cpp
#include "Board.h"
#include <iostream>
#include <conio.h>
using namespace std;

Board::Board() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			grid[i][j] = nullptr;
		}
	}

	for (int i = 0; i < COLS; i++) {
		grid[6][i] = new Pawn(WHITE);
	}
	grid[7][0] = new Rook(WHITE);
	grid[7][1] = new Knight(WHITE);
	grid[7][2] = new Bishop(WHITE);
	grid[7][3] = new Queen(WHITE);
	grid[7][4] = new King(WHITE);
	grid[7][5] = new Bishop(WHITE);
	grid[7][6] = new Knight(WHITE);
	grid[7][7] = new Rook(WHITE);

	for (int i = 0; i < COLS; i++) {
		grid[1][i] = new Pawn(BLACK);
	}
	grid[0][0] = new Rook(BLACK);
	grid[0][1] = new Knight(BLACK);
	grid[0][2] = new Bishop(BLACK);
	grid[0][3] = new Queen(BLACK);
	grid[0][4] = new King(BLACK);
	grid[0][5] = new Bishop(BLACK);
	grid[0][6] = new Knight(BLACK);
	grid[0][7] = new Rook(BLACK);

	enPassantRow = -1;
	enPassantCol = -1;
}

Board::~Board() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			delete grid[i][j];
		}
	}
}

Piece* Board::getPiece(int row, int col) const {
	return grid[row][col];
}

bool Board::movePiece(int cRow, int cCol, int nRow, int nCol, Color currentPlayer) {
	// No piece at source
	if (grid[cRow][cCol] == nullptr) {
		return false;
	}
	// Piece does not belong to current player
	if (grid[cRow][cCol]->getColor() != currentPlayer) {
		return false;
	}
	// Piece movement rules not satisfied
	if (!grid[cRow][cCol]->isValidMove(cRow, cCol, nRow, nCol, grid, enPassantRow, enPassantCol)) {
		return false;
	}
	// Destination has a friendly piece
	if (grid[nRow][nCol] != nullptr && grid[nRow][nCol]->getColor() == currentPlayer) {
		return false;
	}

	// Save en passant state in case move needs to be undone
	int prevEnPassantRow = enPassantRow;
	int prevEnPassantCol = enPassantCol;

	// Check if this move is an en passant capture
	bool isEnPassant = (grid[cRow][cCol]->getSymbol() == 'P'
		&& nRow == enPassantRow && nCol == enPassantCol);

	int capturedPawnRow = -1;
	Piece* enPassantCaptured = nullptr;

	// Remove the captured pawn from the board for en passant
	if (isEnPassant) {
		capturedPawnRow = (currentPlayer == WHITE) ? nRow + 1 : nRow - 1;
		enPassantCaptured = grid[capturedPawnRow][nCol];
		grid[capturedPawnRow][nCol] = nullptr;
	}

	// Temporarily execute the move
	Piece* temp = grid[nRow][nCol];
	grid[nRow][nCol] = grid[cRow][cCol];
	grid[cRow][cCol] = nullptr;

	// Reset en passant then set it if this move is a double pawn push
	enPassantRow = -1;
	enPassantCol = -1;

	if (grid[nRow][nCol]->getSymbol() == 'P' && abs(nRow - cRow) == 2) {
		enPassantRow = (currentPlayer == WHITE) ? nRow + 1 : nRow - 1;
		enPassantCol = nCol;
	}

	// Undo the move if it leaves own king in check
	if (isInCheck(currentPlayer)) {
		grid[cRow][cCol] = grid[nRow][nCol];
		grid[nRow][nCol] = temp;
		if (isEnPassant) {
			grid[capturedPawnRow][nCol] = enPassantCaptured;
		}
		enPassantRow = prevEnPassantRow;
		enPassantCol = prevEnPassantCol;
		return false;
	}

	// Delete captured piece if any
	if (temp != nullptr)
		delete temp;

	// Delete the en passant captured pawn
	if (isEnPassant && enPassantCaptured != nullptr)
		delete enPassantCaptured;

	// Handle pawn promotion when reaching the back rank
	if (grid[nRow][nCol]->getSymbol() == 'P' && (nRow == 0 || nRow == 7)) {
		cout << "\033[?25h";
		cout << "\nPawn Promotion! Choose piece (Q, R, B, N): ";
		char choice;
		cin >> choice;
		cout << "\033[?25l";

		delete grid[nRow][nCol];
		if (choice == 'R' || choice == 'r')
			grid[nRow][nCol] = new Rook(currentPlayer);
		else if (choice == 'B' || choice == 'b')
			grid[nRow][nCol] = new Bishop(currentPlayer);
		else if (choice == 'N' || choice == 'n')
			grid[nRow][nCol] = new Knight(currentPlayer);
		else
			grid[nRow][nCol] = new Queen(currentPlayer);
	}

	return true;
}

bool Board::isInCheck(Color c) {
	int kingRow = -1;
	int kingCol = -1;

	// Find the king's position
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (grid[i][j] != nullptr && grid[i][j]->getColor() == c && grid[i][j]->getSymbol() == 'K') {
				kingRow = i;
				kingCol = j;
			}
		}
	}

	// King not found
	if (kingRow == -1) {
		return false;
	}

	// Check if any enemy piece can reach the king
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (grid[i][j] != nullptr && grid[i][j]->getColor() != c) {
				if (grid[i][j]->isValidMove(i, j, kingRow, kingCol, grid, enPassantRow, enPassantCol)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::isCheckmate(Color c) {
	if (!isInCheck(c)) {
		return false;
	}

	// Try every possible move for every friendly piece
	for (int cRow = 0; cRow < ROWS; cRow++) {
		for (int cCol = 0; cCol < COLS; cCol++) {
			if (grid[cRow][cCol] == nullptr || grid[cRow][cCol]->getColor() != c) {
				continue;
			}
			for (int nRow = 0; nRow < ROWS; nRow++) {
				for (int nCol = 0; nCol < COLS; nCol++) {
					if (!grid[cRow][cCol]->isValidMove(cRow, cCol, nRow, nCol, grid, enPassantRow, enPassantCol))
						continue;
					if (grid[nRow][nCol] != nullptr && grid[nRow][nCol]->getColor() == c)
						continue;

					// Handle en passant in simulation
					bool isEnPassant = (grid[cRow][cCol]->getSymbol() == 'P'
						&& nRow == enPassantRow && nCol == enPassantCol);

					int capturedPawnRow = -1;
					Piece* enPassantCaptured = nullptr;

					if (isEnPassant) {
						capturedPawnRow = (c == WHITE) ? nRow + 1 : nRow - 1;
						enPassantCaptured = grid[capturedPawnRow][nCol];
						grid[capturedPawnRow][nCol] = nullptr;
					}

					// Temporarily make the move
					Piece* temp = grid[nRow][nCol];
					grid[nRow][nCol] = grid[cRow][cCol];
					grid[cRow][cCol] = nullptr;

					bool stillInCheck = isInCheck(c);

					// Undo the move
					grid[cRow][cCol] = grid[nRow][nCol];
					grid[nRow][nCol] = temp;

					if (isEnPassant) {
						grid[capturedPawnRow][nCol] = enPassantCaptured;
					}

					// Found a move that escapes check
					if (!stillInCheck) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool Board::isStalemate(Color c) {
	if (isInCheck(c)) {
		return false;
	}

	// Try every possible move for every friendly piece
	for (int cRow = 0; cRow < ROWS; cRow++) {
		for (int cCol = 0; cCol < COLS; cCol++) {
			if (grid[cRow][cCol] == nullptr || grid[cRow][cCol]->getColor() != c) {
				continue;
			}
			for (int nRow = 0; nRow < ROWS; nRow++) {
				for (int nCol = 0; nCol < COLS; nCol++) {
					if (!grid[cRow][cCol]->isValidMove(cRow, cCol, nRow, nCol, grid, enPassantRow, enPassantCol))
						continue;
					if (grid[nRow][nCol] != nullptr && grid[nRow][nCol]->getColor() == c)
						continue;

					// Handle en passant in simulation
					bool isEnPassant = (grid[cRow][cCol]->getSymbol() == 'P'
						&& nRow == enPassantRow && nCol == enPassantCol);

					int capturedPawnRow = -1;
					Piece* enPassantCaptured = nullptr;

					if (isEnPassant) {
						capturedPawnRow = (c == WHITE) ? nRow + 1 : nRow - 1;
						enPassantCaptured = grid[capturedPawnRow][nCol];
						grid[capturedPawnRow][nCol] = nullptr;
					}

					// Temporarily make the move
					Piece* temp = grid[nRow][nCol];
					grid[nRow][nCol] = grid[cRow][cCol];
					grid[cRow][cCol] = nullptr;

					bool stillInCheck = isInCheck(c);

					// Undo the move
					grid[cRow][cCol] = grid[nRow][nCol];
					grid[nRow][nCol] = temp;

					if (isEnPassant) {
						grid[capturedPawnRow][nCol] = enPassantCaptured;
					}

					// Found at least one legal move so not stalemate
					if (!stillInCheck) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void Board::getValidMoves(int row, int col, bool validMoves[8][8]) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			validMoves[i][j] = false;

	if (grid[row][col] == nullptr) return;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (grid[row][col]->isValidMove(row, col, i, j, grid, enPassantRow, enPassantCol))
				if (grid[i][j] == nullptr || grid[i][j]->getColor() != grid[row][col]->getColor())
					validMoves[i][j] = true;
}

void Board::displayBoard(int cursorRow, int cursorCol, int selRow, int selCol, bool validMoves[8][8]) const {
	cout << "\n    a   b   c   d   e   f   g   h" << endl;
	cout << "  +---+---+---+---+---+---+---+---+" << endl;
	for (int i = 0; i < 8; i++) {
		cout << (8 - i) << " |";
		for (int j = 0; j < 8; j++) {

			// Cursor position shown with blue background
			if (i == cursorRow && j == cursorCol) {
				if (grid[i][j] == nullptr)
					cout << "\033[44m   \033[0m|";
				else
					cout << "\033[44m\033[97m " << grid[i][j]->getSymbol() << " \033[0m|";
			}
			// Selected piece shown with green background
			else if (i == selRow && j == selCol) {
				if (grid[i][j] == nullptr)
					cout << "\033[42m   \033[0m|";
				else
					cout << "\033[42m\033[30m " << grid[i][j]->getSymbol() << " \033[0m|";
			}
			// Valid move squares shown with yellow background
			else if (validMoves[i][j]) {
				if (grid[i][j] == nullptr)
					cout << "\033[43m\033[30m . \033[0m|";
				else
					cout << "\033[43m\033[30m " << grid[i][j]->getSymbol() << " \033[0m|";
			}
			// Empty square
			else if (grid[i][j] == nullptr)
				cout << "   |";
			else {
				// White pieces on light background, black pieces on dark background
				if (grid[i][j]->getColor() == WHITE)
					cout << "\033[47m\033[30m " << grid[i][j]->getSymbol() << " \033[0m|";
				else
					cout << "\033[100m\033[97m " << grid[i][j]->getSymbol() << " \033[0m|";
			}
		}
		cout << " " << (8 - i) << endl;
		cout << "  +---+---+---+---+---+---+---+---+" << endl;
	}
	cout << "    a   b   c   d   e   f   g   h" << endl << endl;
}
