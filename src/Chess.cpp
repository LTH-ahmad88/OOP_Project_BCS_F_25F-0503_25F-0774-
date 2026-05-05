// main.cpp
#include "Board.h"
#include <conio.h>

int main() {
	Board b;
	Color currentPlayer = WHITE;
	int cursorRow = 7;
	int cursorCol = 0;
	int selRow = -1;
	int selCol = -1;
	bool pieceSelected = false;
	bool validMoves[ROWS][COLS] = { false };

	// Hide cursor to reduce flicker
	cout << "\033[?25l";

	while (!b.isCheckmate(currentPlayer) && !b.isStalemate(currentPlayer)) {
		// Move to top of terminal instead of clearing to reduce flicker
		cout << "\033[H";
		cout << (currentPlayer == WHITE ? "White" : "Black") << "'s Turn          " << endl;

		if (b.isInCheck(currentPlayer))
			cout << (currentPlayer == WHITE ? "White" : "Black") << " is in Check!     " << endl;
		else
			cout << "                                   " << endl;

		cout << "Use Arrow Keys to move, Enter to Select/Move, ESC to deselect" << endl;

		b.displayBoard(cursorRow, cursorCol, selRow, selCol, validMoves);

		int key = _getch();

		// Arrow keys send 224 followed by the direction code
		if (key == 224) {
			key = _getch();
			if (key == 72 && cursorRow > 0) cursorRow--;
			if (key == 80 && cursorRow < 7) cursorRow++;
			if (key == 75 && cursorCol > 0) cursorCol--;
			if (key == 77 && cursorCol < 7) cursorCol++;
		}
		// Enter key - select or move
		else if (key == 13) {
			if (!pieceSelected) {
				Piece* p = b.getPiece(cursorRow, cursorCol);
				// Select piece if it belongs to current player
				if (p != nullptr && p->getColor() == currentPlayer) {
					selRow = cursorRow;
					selCol = cursorCol;
					pieceSelected = true;
					b.getValidMoves(selRow, selCol, validMoves);
				}
			}
			else {
				Piece* p = b.getPiece(cursorRow, cursorCol);
				// Reselect if clicking another friendly piece
				if (p != nullptr && p->getColor() == currentPlayer) {
					selRow = cursorRow;
					selCol = cursorCol;
					b.getValidMoves(selRow, selCol, validMoves);
				}
				// Move to a valid destination square
				else if (validMoves[cursorRow][cursorCol]) {
					if (b.movePiece(selRow, selCol, cursorRow, cursorCol, currentPlayer)) {
						// Switch turns
						currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
						selRow = -1;
						selCol = -1;
						pieceSelected = false;
						for (int i = 0; i < ROWS; i++)
							for (int j = 0; j < COLS; j++)
								validMoves[i][j] = false;
					}
				}
				// Clicked invalid square - deselect
				else {
					selRow = -1;
					selCol = -1;
					pieceSelected = false;
					for (int i = 0; i < ROWS; i++)
						for (int j = 0; j < COLS; j++)
							validMoves[i][j] = false;
				}
			}
		}
		// ESC key - deselect current piece
		else if (key == 27) {
			selRow = -1;
			selCol = -1;
			pieceSelected = false;
			for (int i = 0; i < ROWS; i++)
				for (int j = 0; j < COLS; j++)
					validMoves[i][j] = false;
		}
	}

	// Show final board state and announce result
	cout << "\033[H";
	b.displayBoard(cursorRow, cursorCol, -1, -1, validMoves);

	if (b.isCheckmate(currentPlayer))
		cout << (currentPlayer == WHITE ? "White" : "Black") << " is in Checkmate! Game Over!" << endl;
	else
		cout << "Stalemate! It's a Draw!" << endl;

	// Restore cursor
	cout << "\033[?25h";

	return 0;
}
