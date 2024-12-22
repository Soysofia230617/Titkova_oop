#include "ConsoleRender.h"

void ConsoleRenderer::Render(const GameManager& game) {
    game.DisplayFields_();
    game.DisplayAbilities_();
}
