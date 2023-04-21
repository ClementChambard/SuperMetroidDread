#ifndef GAME
#define GAME

#include <iostream>
#include <math.h>

#include <Engine.hpp>

class Game : public NSEngine::IEngine {

public:
    Game();
    ~Game() override {}

    void on_create() override;
    void on_update() override;
    void on_render() override;
    void on_destroy() override;

};

#endif
