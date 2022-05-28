# Checkers

## Description
Checkers, or draughts, is a strategy board game played by two players. There are many variants of checkers. This project implements a simple program that reads, prints, and plays a variant of the game. This project was a university assignment designed to test the understanding of dynamic memory and linked data structures and extend program design, testing, and debugging skills. It also serves as a glimpse into artificial Intelligence and tree search algorithms.

<p width="100%" align="center">
<img src="https://user-images.githubusercontent.com/94183388/170808819-352ab85e-ea8f-4ac0-800b-ebe8a330c0e1.png" width="800" height="250">
</p>

### Setup 
An 8x8 chessboard with 12 black and 12 white pieces initially positioned as shown in Figure 1a.
### Gameplay
Each player plays all pieces of the same color. Black open the game by making a move, then white
make a move, and then players alternate their turns. In a single turn, the player either makes a move or capture.
For example, the arrow in Figure 1a denotes an opening move of the black piece from cell G6 to cell F5.
### Moves
A piece may move to an empty cell diagonally forward (toward the opponent; north for black and south
for white) one square. The arrows in Figure 1b show all the legal moves of black and white pieces.
### Towers
When a piece reaches the furthest row (the top row for black and the bottom row for white), it becomes
a tower (a pile of pieces). The only move of the white piece at cell D7 in Figure 1b promotes it to the tower. A
tower may move to an empty cell diagonally, both forward and backward, one square. The arrows in Figure 1c
show all the legal moves of black and white towers.
### Captures
To capture a piece or a tower of the opponent, a piece or a tower of the player jumps over it and lands
in a straight diagonal line on the other side. This landing cell must be empty. When a piece or tower is captured,
it is removed from the board. Only one piece or tower may be captured in a single jump, and, in our variant of
the game, only one jump is allowed in a single turn. Hence, if another capture is available after the first jump, it
cannot be taken in this turn. Also, in our variant of the game, if a player can make a move or a capture, they may
decide which of the two to complete. A piece always jumps forward (toward the opponent), while a tower can
jump forward and backward. The arrows in Figure 1d show all the legal captures for both players.
### Game end
A player wins the game if it is the opponent’s turn and they cannot take action, move or capture,
either because no their pieces and towers are left on the board or because no legal move or capture is possible.

## Reading and printing 
Initially, program reads input and prints the initial setup and all legal actions. 

<p width="100%" align="center">
<img src="https://user-images.githubusercontent.com/94183388/170809411-23df207f-969d-4911-b6d9-d186bb11def9.png" width="800" height="250">
</p>

For example, lines 1–21 report the board configuration and specify the initial setup from Figure 1a. Characters ‘b’ and ‘w’ are used to denote black and white pieces, respectively. Then, lines 22–42 print the first move specified in the first line of the input. The output of each action starts with the delimiter line of 37 ‘=’ characters; see line 22. The next two lines print information about the action taken and the cost of the board; see lines 23 and 24. The
cost of a board is computed as b + 3B − w − 3W, where b, w, B, and W are, respectively, the number of black
pieces, white pieces, black towers, and white towers on the board; that is, a tower costs three pieces. Then, the program prints the board configuration that results from the action. 

If an illegal action is encountered in the input, the program will select and print one of six error messages. The program will terminate immediately after printing the error message.

