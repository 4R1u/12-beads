// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
// #include "combined.h"
#include "SDLinit.h"
#include "gameuifuncs.h"

int main(int argc, char *args[])
{
    if (!init())
        printf("Failed to initialize!\n");
    else
    {
        // Load media
        if (!loadMedia())
            printf("Failed to load media!");
        else
        {
            // event handler
            SDL_Event e;
            // main loop flag
            bool quit = false;
            generateMainMenu();
            while (quit == false)
            {
                // Handle events on queue
                while (SDL_PollEvent(&e))
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                        quit = true;
                    // user presses key
                    else if (e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDLK_ESCAPE)
                        {
                            // navigate "out"
                            if (menustate == 1)
                            {
                                generateMainMenu();
                                menustate = 0;
                            }
                            else if (menustate == 2)
                            {
                                chooseBoardScreen();
                                menustate = 1;
                            }
                        }
                    }
                    // navigate "in"
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        // Get mouse position
                        int x = 0, y = 0;
                        SDL_GetMouseState(&x, &y);
                        // navigate "in"
                        if (menustate == 0)
                        {
                            // compare x,y to Start button
                            if (x >= startRectx && x <= startRectx + BUTTON_WIDTH && y >= startRecty && y <= startRecty + BUTTON_HEIGHT)
                            {
                                chooseBoardScreen();
                                menustate = 1;
                            }
                            // continue button
                            else if (x >= loadRectx && x <= loadRectx + BUTTON_WIDTH && y >= loadRecty && y <= loadRecty + BUTTON_HEIGHT)
                            {
                                menustate = 2;
                                generateBoard();
                                if (!loadGame())
                                {
                                    chooseBoardScreen();
                                    menustate = 1;
                                }
                            }
                        }
                        else if (menustate == 1)
                        {
                            // compare x,y to left board
                            if (x >= leftRectx && x <= leftRectx + 200 && y >= leftRecty && y <= leftRecty + 200)
                            {
                                menustate = 2;
                                boardchoice = 'l';
                                generateBoard();
                                chooseBoard();
                            }
                            // compare x,y to right board
                            else if (x >= rightRectx && x <= rightRectx + 200 && y >= rightRecty && y <= rightRecty + 200)
                            {
                                menustate = 2;
                                boardchoice = 'r';
                                generateBoard();
                                chooseBoard();
                            }
                        }
                        else if (menustate == 2)
                        {
                            if (winner)
                            {
                                clearSavedGame();
                                generateMainMenu();
                                menustate = 0;
                            }
                            // go through the loop
                            int j = 0;
                            player = p1;
                            for (int i = 0; i < 5; i++)
                                for (j = 0; j < 5; j++)
                                {
                                    // determine coordinates of each position a counter could be in a loop, and check if mouse click was detected inside any
                                    if (x >= ((SCREEN_WIDTH - 400) / 2 + j * 100 - 25) && x <= ((SCREEN_WIDTH - 400) / 2 + j * 100 + 25) && y >= ((SCREEN_HEIGHT - 400) / 2 + i * 100 - 25) && y <= ((SCREEN_HEIGHT - 400) / 2 + i * 100 + 25))
                                    {
                                        if (turn % 2)
                                            player = p2;
                                        if (board[i][j] == player)
                                        {
                                            clearEmptySquares();
                                            displayValidMoves(player, i, j);
                                            activei = i, activej = j;
                                        }
                                        else if (board[i][j] == 'E')
                                        {
                                            moveBead(player, activei, activej, i, j);
                                            clearEmptySquares();
                                        }
                                        break;
                                    }
                                }
                            if (menustate == 2)
                                generateBoard();
                        }
                    }
                }
                if (menustate == 2)
                    displayBeads();
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}