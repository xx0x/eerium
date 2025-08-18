#include "Menu.hpp"
#include "Game.hpp"

int main() {
    Menu menu;
    int choice = menu.run();

    if (choice == 1) {
        Game game;
        if (game.init()) {
            game.run();
        }
    }

    return 0;
}
