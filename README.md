# The_Jungle_Game

Realization of a board game in C (windows only), on the chess models. In collaboration with @Weefle. 

We propose to realize the "jungle game" between two humans playing in turn
role on the same PC.
Supports
1) A 9x7 square chessboard representing the jungle, some of which have
a particular status (shaded in Figure 1).
o West Lake (B4, B5, B6, C4, C5, C6)
o The East Lake (E4, E5, E6, F4, F5, F6)
o The three traps of the blue camp (C9, D8, E9)
o The three traps of the red camp (C1, D2, E1)
o The blue sanctuary (D9)
o The Red Sanctuary (D1)
  2) 16 animal-pieces (8 red, 8 blue) organized according to the hierarchy
next: elephant, lion, tiger, panther, dog, wolf, cat and rat. 

One of the players takes the reds (lines 1, 2 and 3), the other takes the blue ones (lines
7, 8, 9). In text mode, pawns are represented by the following letters:

- E for elephant
- L for lion
- T for tiger
- P for panther
- C for dogs
- O for wolf
- H for cat
- R for rat

Game Mechanism

- Each player, in turn, moves one of the pieces from one square to the
times, horizontally or vertically.

- When moving, a piece can take an opponent's piece
equal or less by putting oneself in his place. The part taken is removed from the
game. However, the rat can take the elephant.

- Only the rat can move in the lakes but it cannot capture
the opposing elephant passing from the lake to the shore.

- Lions and tigers can jump directly from one bank to the other
vertically or horizontally unless a rat (friend or opponent) is
in the jump line.

- When a piece is in an opponent's trap, its fighting force
momentarily becomes zero but retains the ability to
move.

-Coins cannot enter their own sanctuary. 

The project consists more specifically in modeling the game in text mode. The pawns will be letters.
Moving parts will be done here only by entering coordinates.

<h4><u><b>Set your Cmake WORKING_DIRECTORY variable at the root of the project<b><u> :<h4>
>WORKING_DIRECTORY=%USER%/your_folder/The_Jungle_Game/

