#pragma once

/* Include */
// C++
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// external
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

/* Basics */
namespace synthciv {
    // basic types
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;
    typedef float f32;
    typedef double f64;
    typedef void* address;
    typedef bool boolean;

    // character defines
    typedef synthciv::u8 character;

    // error
    class error {
    public:
        bool occured;
        std::string json;

        // constructors
        error() {
            occured = false;
            json = "\"error\": {\n\t\"reason\": \"No problem was detected.\"\n}";
        }
        error(bool _occured, std::string _json) {
            occured = _occured;
            json = _json;
        }
    };

    // buffer defines
    typedef synthciv::u64 length;
    class buffer {
    public:
        synthciv::address start;
        synthciv::address end;

        // constructors
        buffer() {
            start = 0;
            end = 0;
        }
        buffer(synthciv::address _start, synthciv::address _end) {
            start = _start;
            end = _end;
        }

        // length of buffer
        synthciv::length length() {
            return (synthciv::length)((synthciv::u64)end - (synthciv::u64)start + 1);
        }

        // open
        void allocate(synthciv::length _allocation_size) {
            start = malloc(_allocation_size);
            if (start != 0) {
                end = (synthciv::address)((synthciv::u64)start + _allocation_size - 1);
            } else {
                end = 0;
            }
        }

        // close
        void deallocate() {
            if (start != 0) {
                free(start);
            }
        }

        // check if buffer is empty
        synthciv::boolean is_empty() {
            return start == 0 && end == 0;
        }
    };

    class current {
    public:
        synthciv::buffer range;
        synthciv::address progress;

        current() {
            range = synthciv::buffer();
            progress = 0;
        }
        current(synthciv::buffer _range, synthciv::address _progress) {
            range = _range;
            progress = _progress;
        }
    };

    // file loader
    class file_loader {
    public:
        synthciv::error error;

        // load file into std::string
        std::string load_text_file(std::string _file_path) {
            // null init error
            error = synthciv::error();

            // open file stream
            std::ifstream file(_file_path);

            // check for error
            if (!file) {
                // setup error
                error = synthciv::error(true, "\"error\": {\n\t\"reason\": \"Text file could not be loaded / found.\",\n\t\"file_path\": \"" + _file_path + "\"\n}");

                return "";
            }

            // read file into string
            std::ostringstream stringstream;
            stringstream << file.rdbuf();
            return stringstream.str();
        }
    };

    // world data
    namespace world {
        typedef synthciv::f32 axis;
        typedef synthciv::world::axis x;
        typedef synthciv::world::axis y;
        typedef synthciv::world::axis z;
    }
}
