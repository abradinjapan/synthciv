#include "synthciv.hpp"

int main() {
    // run game
    synthciv::game game = synthciv::game();
    game.run();

    // display error
    std::cout << game.error.json << std::endl;

    return 0;
}
