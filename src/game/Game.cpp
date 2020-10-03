#include <iostream>
#include "../../lib/glm/glm.hpp"

#include "./Constants.h"
#include "./Game.h"

glm::vec2 projectilePosition = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVelocity = glm::vec2(20.0f, 20.0f);

Game::Game()
{
    this->isRunning = false;
}

Game::~Game()
{
}

bool Game::IsRunning() const
{
    return this->isRunning;
}

void Game::Initialize(int windowWidth, int windowHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    // Create Window
    const char *windowTitle = NULL;
    const int xWindowPosition = SDL_WINDOWPOS_CENTERED;
    const int yWindowPosition = SDL_WINDOWPOS_CENTERED;
    const Uint32 windowFlags = SDL_WINDOW_BORDERLESS;
    window = SDL_CreateWindow(
        windowTitle,
        xWindowPosition,
        yWindowPosition,
        windowWidth,
        windowHeight,
        windowFlags);

    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    // Create Renderer
    const int renderingDriverIndex = -1;
    const Uint32 rendererFlags = 0;
    renderer = SDL_CreateRenderer(window, renderingDriverIndex, rendererFlags);

    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    isRunning = true;
    return;
}

void Game::ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
        {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::Update()
{
    // Sleep the execution until we reach the target frame time in milliseconds
    const int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    // Only call delay if we are too fast to process this frame
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    const int currentTicks = SDL_GetTicks();

    const float TIME_NORMAL = 1000.0f; // milliseconds
    const float MAX_DELTA_TIME = 50.0f / TIME_NORMAL; // milliseconds relative to time normal

    // The difference in ticks from last frame converted to seconds
    float deltaTime = (currentTicks - ticksLastFrame) / TIME_NORMAL;

    // Clamp delta time to a maximum value
    deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = currentTicks;

    // Update game objects position by delta time
    projectilePosition = glm::vec2(
        projectilePosition.x + (projectileVelocity.x * deltaTime),
        projectilePosition.y + (projectileVelocity.y * deltaTime)
    );
}

void Game::Render()
{
    // Render Background
    SDL_SetRenderDrawColor(
        renderer, 
        21, 21, 21, // very dark gray
        SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer); // clear the back buffer and render the last drawn color as the background 

    int projectileWidth = 10;
    int projectileHeight = 10;
    SDL_Rect projectile {
        (int) projectilePosition.x,
        (int) projectilePosition.y,
        projectileWidth,
        projectileHeight
    };

    SDL_SetRenderDrawColor(
        renderer, 
        SDL_ALPHA_OPAQUE, 
        SDL_ALPHA_OPAQUE, 
        SDL_ALPHA_OPAQUE, 
        SDL_ALPHA_OPAQUE
    );
    SDL_RenderFillRect(renderer, &projectile);

    // Swap front and back buffers
    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}