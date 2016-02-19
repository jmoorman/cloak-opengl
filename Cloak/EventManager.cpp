#include "stdafx.h"
#include "EventManager.h"


EventManager::EventManager(void)
{
}


EventManager::~EventManager(void)
{
}

void EventManager::handleEvents()
{
    SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
            keyPressed(event.key);
			break;
        case SDL_KEYUP:
            keyReleased(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            mousePressed(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseReleased(event.button);
            break;
        case SDL_MOUSEMOTION:
            mouseMoved(event.motion);
            break;
        case SDL_QUIT:
            quit(event.quit);
            break;
		}
    }
}

void EventManager::keyPressed(const SDL_KeyboardEvent &keyEvent)
{
    switch(keyEvent.keysym.scancode)
    {
    case SDL_SCANCODE_W:
        g_position.z += 10;
        break;
    case SDL_SCANCODE_S:
        g_position.z -= 10;
        break;
    case SDL_SCANCODE_A:
        g_position.x += 10;
        break;
    case SDL_SCANCODE_D:
        g_position.x -= 10;
        break;
    }
    std::cout << "position: (" << g_position.x << ", " << g_position.y << ", " << g_position.z << ")" << std::endl;
}

void EventManager::keyReleased(const SDL_KeyboardEvent &keyEvent)
{

}

void EventManager::mousePressed(const SDL_MouseButtonEvent &mouseEvent)
{

}

void EventManager::mouseReleased(const SDL_MouseButtonEvent &mouseEvent)
{

}

void EventManager::mouseMoved(const SDL_MouseMotionEvent &mouseEvent)
{
    g_deltaYaw += mouseEvent.xrel;
    g_deltaPitch -= mouseEvent.yrel;
    std::cout << "yaw: " << g_deltaYaw << ", pitch: " << g_deltaPitch << std::endl;
}

void EventManager::quit(const SDL_QuitEvent &quitEvent)
{
    g_running = false;
}
