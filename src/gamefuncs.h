#include <ctime>
#include <fstream>
#include <iostream>

// board configurations
char board1[5][5] = {{'r', 'r', 'r', 'r', 'r'},
                     {'r', 'r', 'r', 'r', 'r'},
                     {'r', 'r', 'e', 'g', 'g'},
                     {'g', 'g', 'g', 'g', 'g'},
                     {'g', 'g', 'g', 'g', 'g'}};
char board2[5][5] = {{'r', 'r', 'r', 'r', 'r'},
                     {'r', 'r', 'r', 'r', 'r'},
                     {'g', 'g', 'e', 'r', 'r'},
                     {'g', 'g', 'g', 'g', 'g'},
                     {'g', 'g', 'g', 'g', 'g'}};
char board[5][5] = {{'r', 'r', 'r', 'r', 'r'},
                    {'r', 'r', 'r', 'r', 'r'},
                    {'e', 'e', 'e', 'e', 'e'},
                    {'g', 'g', 'g', 'g', 'g'},
                    {'g', 'g', 'g', 'g', 'g'}};
// gloabal variables
char boardchoice = 0;
char p1 = 'g', p2 = 'r', player = 0;
int turn = 0;
int activei = 5, activej = 5;
char winner = 0;
char turnstring[99] = {0};
// file stream (this isn't the right place, who cares)
fstream gamestate;

// save game state
void saveState()
{
    gamestate.open("data/gamestate.dat", ios::out);
    char bead = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            bead = board[i][j];
            if (bead >= 'E' && bead <= 'R')
                bead += 32;
            gamestate << bead;
        }
    if (turn % 2)
        gamestate << p2 << p1 << player;
    else
        gamestate << p1 << p2 << player;
    gamestate.close();
}

void clearSavedGame()
{
    gamestate.open("data/gamestate.dat", ios::out);
    gamestate << "rrrrrrrrrrrregggggggggggggr";
    gamestate.close();
    winner = 0;
}

// load saved game
int loadGame()
{
    turn = 0;
    gamestate.open("data/gamestate.dat", ios::in);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            board[i][j] = gamestate.get();
    p1 = gamestate.get();
    p2 = gamestate.get();
    player = gamestate.get();
    gamestate.close();
    return 1;
}

// determine first player
void toss()
{
    p1 = 'g', p2 = 'r';
    srand((unsigned)time(0));
    if (rand() % 2)
    {
        p1 = 'r';
        p2 = 'g';
    }
}

// Initialize board placement depending on user choice
void chooseBoard()
{
    winner = 0;
    toss();
    if (boardchoice == 'l')
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                board[i][j] = board1[i][j];
    else if (boardchoice == 'r')
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                board[i][j] = board2[i][j];
    turn = 0;
}

// clears empty squares user COULD move into, along with counters previously selected for moving
void clearEmptySquares()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == 'E' || board[i][j] == 'G' || board[i][j] == 'R')
                board[i][j] += 32;
}

// display valid moves
void displayValidMoves(char player, int i, int j)
{
    if (board[i][j] == player)
    {
        board[i][j] -= 32;
        // up
        if (i && board[i - 1][j] == 'e')
            board[i - 1][j] = 'E';
        else if (i > 1 && board[i - 1][j] != player && board[i - 2][j] == 'e')
            board[i - 2][j] = 'E';
        // up-right
        if (i && j < 4 && board[i - 1][j + 1] == 'e' && (i % 2 == j % 2))
            board[i - 1][j + 1] = 'E';
        else if (i > 1 && j < 3 && board[i - 1][j + 1] != player && board[i - 2][j + 2] == 'e' && (i % 2 == j % 2))
            board[i - 2][j + 2] = 'E';
        // right
        if (j < 4 && board[i][j + 1] == 'e')
            board[i][j + 1] = 'E';
        else if (j < 3 && board[i][j + 1] != player && board[i][j + 2] == 'e')
            board[i][j + 2] = 'E';
        // down-right
        if (i < 4 && j < 4 && board[i + 1][j + 1] == 'e' && (i % 2 == j % 2))
            board[i + 1][j + 1] = 'E';
        else if (i < 3 && j < 3 && board[i + 1][j + 1] != player && board[i + 2][j + 2] == 'e' && (i % 2 == j % 2))
            board[i + 2][j + 2] = 'E';
        // down
        if (i < 4 && board[i + 1][j] == 'e')
            board[i + 1][j] = 'E';
        else if (i < 3 && board[i + 1][j] != player && board[i + 2][j] == 'e')
            board[i + 2][j] = 'E';
        // down-left
        if (i < 4 && j && board[i + 1][j - 1] == 'e' && (i % 2 == j % 2))
            board[i + 1][j - 1] = 'E';
        else if (i < 4 && j > 1 && board[i + 1][j - 1] != player && board[i + 2][j - 2] == 'e' && (i % 2 == j % 2))
            board[i + 2][j - 2] = 'E';
        // left
        if (j && board[i][j - 1] == 'e')
            board[i][j - 1] = 'E';
        else if (j > 1 && board[i][j - 1] != player && board[i][j - 2] == 'e')
            board[i][j - 2] = 'E';
        // up-left
        if (i && j && board[i - 1][j - 1] == 'e' && (i % 2 == j % 2))
            board[i - 1][j - 1] = 'E';
        else if (i > 1 && j > 1 && board[i - 1][j - 1] != player && board[i - 2][j - 2] == 'e' && (i % 2 == j % 2))
            board[i - 2][j - 2] = 'E';
    }
}

// carry out a turn
void moveBead(char player, int si, int sj, int di, int dj)
{
    if (board[di][dj] == 'E')
    {
        board[di][dj] = player;
        board[si][sj] = 'e';
        // conditionals to check if opponent bead was removed
        // up
        if (si - di == 2 && sj == dj && board[si - 1][sj] != player)
            board[si - 1][sj] = 'e';
        // up-right
        else if (si - di == 2 && dj - sj == 2 && board[si - 1][sj + 1] != player)
            board[si - 1][sj + 1] = 'e';
        // right
        else if (si == di && dj - sj == 2 && board[si][sj + 1] != player)
            board[si][sj + 1] = 'e';
        // down-right
        else if (di - si == 2 && dj - sj == 2 && board[si + 1][sj + 1] != player)
            board[si + 1][sj + 1] = 'e';
        // down
        else if (di - si == 2 && sj == dj && board[si + 1][sj] != player)
            board[si + 1][sj] = 'e';
        // down-left
        else if (di - si == 2 && sj - dj == 2 && board[si + 1][sj - 1] != player)
            board[si + 1][sj - 1] = 'e';
        // left
        else if (di == si && sj - dj == 2 && board[si][sj - 1] != player)
            board[si][sj - 1] = 'e';
        // up-left
        else if (si - di == 2 && sj - dj == 2 && board[si - 1][sj - 1] != player)
            board[si - 1][sj - 1] = 'e';
        turn++;
        saveState();
    }
}
