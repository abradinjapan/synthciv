#pragma once

// synthciv
#include "basic.hpp"
#include "window.hpp"
#include "shaders.hpp"

/* Game Loop */
namespace synthciv {
    class game {
        // game state
        synthciv::graphics::window game_window;
        synthciv::boolean is_running;

        // shaders
        synthciv::graphics::shaders chunk_shaders;

        // file loaders
        synthciv::file_loader file_loader;

    public:
        // error
        synthciv::error error;

        game() {
            // game state
            game_window = synthciv::graphics::window();
            is_running = false;

            // shaders
            chunk_shaders = synthciv::graphics::shaders();

            // file loaders
            file_loader = synthciv::file_loader();

            // error
            error = synthciv::error();
        }

    private:
        // compile shader program
        synthciv::graphics::shaders compile_shaders(std::string vertex_shader_file_path, std::string fragment_shader_file_path) {
            synthciv::graphics::shaders output;

            // load files
            std::string vertex = file_loader.load_text_file(vertex_shader_file_path);
            if (file_loader.error.occured) {
                error = file_loader.error;
                return synthciv::graphics::shaders();
            }
            std::string fragment = file_loader.load_text_file(fragment_shader_file_path);
            if (file_loader.error.occured) {
                error = file_loader.error;
                return synthciv::graphics::shaders();
            }

            // compile shaders
            output.open_shaders(&error, vertex, fragment);
            return output;
        }

    public:
        // game loop
        void run() {
            // game start
            std::cout << "Running new game!" << std::endl;

            // force x11 over wayland
            setenv("SDL_VIDEODRIVER", "x11", 1);

            // init SDL3
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
                // setup error
                error = synthciv::error(true, "\"error\": {\"reason\": \"SDL3 did not initialize.\"}");

                printf("%s", (const char*)SDL_GetError());
                printf("\n");

                return;
            }

            // start runner
            is_running = true;

            // open game window
            game_window = synthciv::graphics::window();
            error = game_window.open(synthciv::graphics::window_styling("Synthciv", 800, 600));
            if (error.occured) {
                return;
            }

            // open shaders
            chunk_shaders = compile_shaders("./source/shaders/chunks/vertex.glsl", "./source/shaders/chunks/fragment.glsl");
            if (error.occured) {
                return;
            }

            // temp event
            SDL_Event temp_event;

            // game loop
            while (is_running) {
                // check for quit
                while (SDL_PollEvent(&temp_event)) {
                    switch (temp_event.type) {
                    case SDL_EVENT_QUIT:
                        // stop game
                        is_running = false;

                        break;
                    default:
                        break;
                    }
                }

                // next frame
                game_window.next_frame();
            }

            // close game
            game_window.close();
            SDL_Quit();

            // thank player
            std::cout << "Thanks for playing Synthciv!" << std::endl;

            return;
        }
    };
}
