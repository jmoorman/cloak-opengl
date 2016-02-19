#pragma once

#include "stdafx.h"

//get rid of these globals asap
extern bool g_running;
extern float g_deltaYaw;
extern float g_deltaPitch;
extern glm::vec3 g_position;

class EventManager
{
public:
    EventManager(void);
    ~EventManager(void);

    void handleEvents();

private:
    //basic input handlers
    void keyPressed(const SDL_KeyboardEvent &keyEvent);
    void keyReleased(const SDL_KeyboardEvent &keyEvent);
    void mousePressed(const SDL_MouseButtonEvent &mouseEvent);
    void mouseReleased(const SDL_MouseButtonEvent &mouseEvent);
    void mouseMoved(const SDL_MouseMotionEvent &mouseEvent);
    void quit(const SDL_QuitEvent &quitEvent);
};

