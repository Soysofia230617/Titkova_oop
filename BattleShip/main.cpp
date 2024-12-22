#include "GameManager.h"
#include "CommandProvider.h"
#include "GameController.h"

int main() {
    GameManager game;
    game.Init(); // Инициализируем поле и корабли

    CommandProvider provider("commands.txt");

    ConsoleRenderer renderer;
    GameDisplay<ConsoleRenderer> display(renderer);
    GameController<CommandProvider> controller(game, provider, display);

    // Отобразим поле один раз перед началом цикла ввода команд
    display.Update(game);

    // Запускаем цикл обработки команд
    controller.Run();

    return 0;
}
