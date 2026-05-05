# OOP_Project_BCS_F_25F-0503_25F-0774-
# Chess Game in C++

A two-player console-based Chess Game built in C++ using Object-Oriented Programming principles.
Developed as a semester project for the OOP course at NUCES Faisalabad, Spring 2026.

## Features

- Full 8x8 chessboard rendered in the console with color highlights
- All six chess pieces with correct movement rules
- Check and checkmate detection
- Stalemate detection
- En passant capture
- Pawn promotion
- Arrow key navigation with visual cursor
- Valid move highlighting for selected pieces

## How to Compile

Open the project in Visual Studio or any C++ IDE and build the solution.
Or compile manually using g++:

g++ src/main.cpp src/Pieces.cpp src/Board.cpp -o chess

## How to Run

Run the compiled executable from the terminal:

./chess

Note: This project uses conio.h and ANSI escape codes. It is intended to run on Windows.

## Controls

| Key        | Action                        |
|------------|-------------------------------|
| Arrow Keys | Move cursor around the board  |
| Enter      | Select piece or confirm move  |
| ESC        | Deselect current piece        |

## Project Structure

src/
  Pieces.h       - Piece base class and all derived piece class declarations
  Pieces.cpp     - Move logic implementation for all pieces
  Board.h        - Board class declaration
  Board.cpp      - Board logic including move execution and game state
  main.cpp       - Game loop and input handling

report/
  ChessGameReport.docx - Project report

## OOP Concepts Used

- Encapsulation: Private data members in Piece class accessed via getters
- Inheritance: All piece classes derive from the Piece base class
- Polymorphism: isValidMove() overridden in each derived class
- Abstraction: Piece is an abstract class with a pure virtual function
- Composition: Board class contains a 2D array of Piece pointers

## Course Info

- Course: Object-Oriented Programming
- Institution: NUCES Faisalabad-Chiniot Campus
- Instructor: Mr. Hannan Farooq
- Lab Instructor: Ms. Zainab Noor
- Session: Spring 2026
