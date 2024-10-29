#include <stdio.h>
#include <SDL.h>
#include "gamefuncs.h"
#include <fstream>

// constant to let program know where the user is
int menustate = 0;

// definition of useful coordinates
// on the main screen
int startRectx = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
int startRecty = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2;
int loadRectx = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
int loadRecty = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2 + 160;
int leftRectx = (SCREEN_WIDTH - 400) / 3;
int leftRecty = SCREEN_HEIGHT - 200 - (SCREEN_WIDTH - 400) / 3;
int rightRectx = (SCREEN_WIDTH - 400) / 3 * 2 + 200;
int rightRecty = SCREEN_HEIGHT - 200 - (SCREEN_WIDTH - 400) / 3;

// definition of rectangles
SDL_Rect startRect = {startRectx, startRecty, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Rect quitRect = {loadRectx, loadRecty, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Rect leftRect = {leftRectx, leftRecty, 200, 200};
SDL_Rect rightRect = {rightRectx, rightRecty, 200, 200};

void generateMainMenu()
{
    // clear screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    //  render "Start" box
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &startRect);
    // render "Quit" box
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &quitRect);
    // render text
    g12BeadsText.render((SCREEN_WIDTH - g12BeadsText.getWidth()) / 2, 50);
    gStartText.render((SCREEN_WIDTH - gStartText.getWidth()) / 2, startRecty + (BUTTON_HEIGHT - gStartText.getHeight()) / 2);
    gLoadText.render((SCREEN_WIDTH - gLoadText.getWidth()) / 2, loadRecty + (BUTTON_HEIGHT - gStartText.getHeight()) / 2);
}

// draw board choice menu
void chooseBoardScreen()
{
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    gChooseBoardText.render((SCREEN_WIDTH - gChooseBoardText.getWidth()) / 2, BUTTON_HEIGHT);
    gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 3, SCREEN_HEIGHT - 200 - (SCREEN_WIDTH - 400) / 3, &gSpriteClips[0]);
    gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 3 * 2 + 200, SCREEN_HEIGHT - 200 - (SCREEN_WIDTH - 400) / 3, &gSpriteClips[1]);
}

// draw board with shapes
void generateBoard()
{
    // set black background color, and clear screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    // draw sub-base square
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_Rect boardSubBaseRect = {(SCREEN_WIDTH - 450) / 2, (SCREEN_HEIGHT - 450) / 2, 450, 450};
    SDL_RenderFillRect(gRenderer, &boardSubBaseRect);
    // draw base square
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect boardBaseRect = {(SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2, 400, 400};
    SDL_RenderFillRect(gRenderer, &boardBaseRect);
    // set draw color black, and draw lines
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    // diagonals
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2, (SCREEN_WIDTH - 400) / 2 + 400, (SCREEN_HEIGHT - 400) / 2 + 400);
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2 + 400, (SCREEN_HEIGHT - 400) / 2, (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2 + 400);
    // diamond
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2 + 200, (SCREEN_WIDTH - 400) / 2 + 200, (SCREEN_HEIGHT - 400) / 2);
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2 + 200, (SCREEN_HEIGHT - 400) / 2, (SCREEN_WIDTH - 400) / 2 + 400, (SCREEN_HEIGHT - 400) / 2 + 200);
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2 + 400, (SCREEN_HEIGHT - 400) / 2 + 200, (SCREEN_WIDTH - 400) / 2 + 200, (SCREEN_HEIGHT - 400) / 2 + 400);
    SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2 + 200, (SCREEN_HEIGHT - 400) / 2 + 400, (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2 + 200);
    // vertical lines
    for (int i = 100; i <= 300; i += 100)
        SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2 + i, (SCREEN_HEIGHT - 400) / 2, (SCREEN_WIDTH - 400) / 2 + i, (SCREEN_HEIGHT - 400) / 2 + 400);
    // horizontal lines
    for (int i = 100; i <= 300; i += 100)
        SDL_RenderDrawLine(gRenderer, (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2 + i, (SCREEN_WIDTH - 400) / 2 + 400, (SCREEN_HEIGHT - 400) / 2 + i);
}

// show winner color
void showWinner()
{
    if (winner == 'r')
        gRedWinsText.render((SCREEN_WIDTH - gRedWinsText.getWidth()) / 2, (SCREEN_HEIGHT - gRedWinsText.getHeight()) / 2);
    else if (winner == 'g')
        gGreenWinsText.render((SCREEN_WIDTH - gGreenWinsText.getWidth()) / 2, (SCREEN_HEIGHT - gGreenWinsText.getHeight()) / 2);
}

// put beads on screen
void displayBeads()
{
    bool redExists = false, greenExists = false;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (board[i][j] == 'r')
            {
                gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 2 + j * 100 - 20, (SCREEN_HEIGHT - 400) / 2 + i * 100 - 20, &gSpriteClips[2]);
                redExists = true;
            }
            else if (board[i][j] == 'g')
            {
                gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 2 + j * 100 - 20, (SCREEN_HEIGHT - 400) / 2 + i * 100 - 20, &gSpriteClips[3]);
                greenExists = true;
            }
            else if (board[i][j] == 'R')
            {
                gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 2 + j * 100 - 25, (SCREEN_HEIGHT - 400) / 2 + i * 100 - 25, &gSpriteClips[4]);
                redExists = true;
            }
            else if (board[i][j] == 'G')
            {
                gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 2 + j * 100 - 25, (SCREEN_HEIGHT - 400) / 2 + i * 100 - 25, &gSpriteClips[5]);
                greenExists = true;
            }
            else if (board[i][j] == 'E')
                gSpritesheetTexture.render((SCREEN_WIDTH - 400) / 2 + j * 100 - 25, (SCREEN_HEIGHT - 400) / 2 + i * 100 - 25, &gSpriteClips[6]);
        }
    // show starting player
    if (!turn)
    {
        if (p1 == 'r')
            gRedStartsText.render((SCREEN_WIDTH - gRedStartsText.getWidth()) / 2, (SCREEN_HEIGHT - gRedStartsText.getHeight()) / 2);
        else
            gGreenStartsText.render((SCREEN_WIDTH - gGreenStartsText.getWidth()) / 2, (SCREEN_HEIGHT - gGreenStartsText.getHeight()) / 2);
    }
    if (!greenExists)
        winner = 'r';
    else if (!redExists)
        winner = 'g';
    if (winner)
        showWinner();
}
