#include "GameManager.h"
#include "CommandProvider.h"
#include "GameController.h"

int main() {
    GameManager game;
    game.Init(); 

    CommandProvider provider("commands.txt");

    ConsoleRenderer renderer;
    GameDisplay<ConsoleRenderer> display(renderer);
    GameController<CommandProvider> controller(game, provider, display);

    display.Update(game);

    controller.Run();

    return 0;
}
