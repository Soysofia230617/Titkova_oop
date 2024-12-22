#ifndef BATTLESHIP_GAMEDISPLAY_H
#define BATTLESHIP_GAMEDISPLAY_H
#include "GameManager.h"

template<typename Renderer>
class GameDisplay {
public:
    GameDisplay(Renderer& renderer) : renderer_(renderer) {}
    void Update(const GameManager& gameManager) {
        renderer_.Render(gameManager);
    }

private:
    Renderer& renderer_;
};

#endif //BATTLESHIP_GAMEDISPLAY_H
