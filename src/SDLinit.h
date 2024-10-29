#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// button dimensions
const int BUTTON_WIDTH = 240;
const int BUTTON_HEIGHT = 100;

// Texture wrapper class
class LTexture
{
public:
    // Initializes variables
    LTexture();

    // Dealocate memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Creates image from text
    bool loadFromRenderedText(TTF_Font *gFont, std::string textureText, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;
    // Image dimensions
    int mWidth;
    int mHeight;
};

// The window we render to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Fonts used
TTF_Font *gTitleFont = NULL;
TTF_Font *gBodyFont = NULL;

// Start,quit,12beads,chooseboard text
LTexture g12BeadsText, gStartText, gLoadText, gChooseBoardText;
// win/start text
LTexture gGreenStartsText, gRedStartsText, gGreenWinsText, gRedWinsText;

// Spritesheet texture
LTexture gSpritesheetTexture;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// Spritesheet parts (I could have used it more efficiently with less spacing between sprites, but whatever)
SDL_Rect gSpriteClips[7];

// Starts up SDL and creates window
bool init()
{
    // Initialization flag
    bool success = true;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL video could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("12 Beads Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // create renderer
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Hardware-accelerated Vsync-enabled renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // Initialize PNG Loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                else
                {
                    // initialize SDL_ttf
                    if (TTF_Init() == -1)
                    {
                        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                        success = false;
                    }
                }
            }
        }
    }

    return success;
}

// LTexture class methods definitions fleshed out here
LTexture::LTexture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture()
{
    // Deallocate
    free();
}
bool LTexture::loadFromFile(std::string path)
{
    // Get rid of preexisting texture
    free();
    // The final texture
    SDL_Texture *newTexture = NULL;
    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Create texture
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from surface from %s! SDL error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
    }
    // return success bool
    mTexture = newTexture;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(TTF_Font *gFont, std::string textureText, SDL_Color textColor)
{
    // Get rid of any preexisting textures
    free();

    // Render text surface
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    else
    {
        // Create texture from surface
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
            printf("Unable to create texture from rendered text! SDL Error: %s/n", SDL_GetError());
        else
        {
            // Get dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        // free old surface
        SDL_FreeSurface(textSurface);
    }
    // return success
    return mTexture != NULL;
}
void LTexture::free()
{
    // Free preexisting texture if any
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::render(int x, int y, SDL_Rect *clip)
{
    // Set position to render and then render
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}
int LTexture::getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}

// Loads media
bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Open font and reder text
    gTitleFont = TTF_OpenFont("data/notofont/Roboto-Regular.ttf", 48);
    gBodyFont = TTF_OpenFont("data/notofont/Roboto-Regular.ttf", 40);
    // defining colors
    SDL_Color whiteTextColor = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color blackTextColor = {0, 0, 0, 0xFF};
    //  load fonts in respective sizes
    if (gTitleFont == NULL)
    {
        printf("Failed to load title font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        // Render text on success
        if (!g12BeadsText.loadFromRenderedText(gTitleFont, "12 Beads", whiteTextColor))
        {
            printf("Failed to load title text!\n");
            success = false;
        }
        if (!gRedStartsText.loadFromRenderedText(gTitleFont, "Red starts", blackTextColor))
        {
            printf("Failed to load red starting text!\n");
            success = false;
        }
        if (!gGreenStartsText.loadFromRenderedText(gTitleFont, "Green Starts", blackTextColor))
        {
            printf("Failed to load green starting text!\n");
            success = false;
        }
        if (!gRedWinsText.loadFromRenderedText(gTitleFont, "Red wins", blackTextColor))
        {
            printf("Failed to load red winner text!\n");
            success = false;
        }
        if (!gGreenWinsText.loadFromRenderedText(gTitleFont, "Green wins", blackTextColor))
        {
            printf("Failed to green winner choice text!\n");
            success = false;
        }
    }
    if (gBodyFont == NULL)
    {
        printf("Failed to load body font! SDL_ttf Error: %s/n", TTF_GetError());
        success = false;
    }
    else
    {
        // Render body text on success
        if (!gStartText.loadFromRenderedText(gBodyFont, "New game", blackTextColor))
        {
            printf("Failed to render start text texture!\n");
            success = false;
        }
        if (!gLoadText.loadFromRenderedText(gBodyFont, "Continue", blackTextColor))
        {
            printf("Failed to load quit text texture!\n");
            success = false;
        }
        if (!gChooseBoardText.loadFromRenderedText(gBodyFont, "Choose bead arrangement", whiteTextColor))
        {
            printf("Failed to load board choice text!\n");
            success = false;
        }
    }
    // spritesheet loading
    if (!gSpritesheetTexture.loadFromFile("data/sprites.svg"))
    {
        printf("Failed to load spritesheet!\n");
        success = false;
    }
    else
    {
        // board image with red beads of the middle rank on the left files
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 200;
        gSpriteClips[0].h = 200;
        // same as above, but red beads on the right
        gSpriteClips[1].x = 250;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 200;
        gSpriteClips[1].h = 200;
        // red bead
        gSpriteClips[2].x = 500;
        gSpriteClips[2].y = 0;
        gSpriteClips[2].w = 40;
        gSpriteClips[2].h = 40;
        // green bead
        gSpriteClips[3].x = 550;
        gSpriteClips[3].y = 0;
        gSpriteClips[3].w = 40;
        gSpriteClips[3].h = 40;
        // red bead (selected)
        gSpriteClips[4].x = 600;
        gSpriteClips[4].y = 0;
        gSpriteClips[4].w = 50;
        gSpriteClips[4].h = 50;
        // green bead (selected)
        gSpriteClips[5].x = 650;
        gSpriteClips[5].y = 0;
        gSpriteClips[5].w = 50;
        gSpriteClips[5].h = 50;
        // empty space player's selected bead can move into
        gSpriteClips[6].x = 700;
        gSpriteClips[6].y = 0;
        gSpriteClips[6].w = 50;
        gSpriteClips[6].h = 50;
    }

    return success;
}

// Frees media and shuts down SDL
void close()
{
    // might write code for a 'dialog' box that asks if the user is sure they want to quit

    // destroy spritesheet
    gSpritesheetTexture.free();

    // free text texture
    g12BeadsText.free();
    gStartText.free();
    gLoadText.free();
    gChooseBoardText.free();
    gRedStartsText.free();
    gGreenStartsText.free();
    gRedWinsText.free();
    gGreenWinsText.free();

    // Free fonts
    TTF_CloseFont(gTitleFont);
    gBodyFont = NULL;
    TTF_CloseFont(gBodyFont);
    gBodyFont = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
