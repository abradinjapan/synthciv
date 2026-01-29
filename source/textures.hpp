#pragma once

/* Include */
#include "basic.hpp"
#include "shaders.hpp"

namespace synthciv::graphics {
    // pixel
    typedef synthciv::u8 color;
    class pixel {
        // write and advance color
        synthciv::current write_color_to_current(synthciv::current _current, synthciv::graphics::color _color) {
            // write color
            *((synthciv::graphics::color*)_current.progress) = _color;

            // advance
            _current.progress = _current.progress + sizeof(synthciv::graphics::color);

            return _current;
        }

    public:
        synthciv::graphics::color red;
        synthciv::graphics::color green;
        synthciv::graphics::color blue;
        synthciv::graphics::color alpha;

        // constructors
        pixel() {
            red = 0;
            green = 0;
            blue = 0;
            alpha = 0;
        }
        pixel(synthciv::graphics::color _red, synthciv::graphics::color _green, synthciv::graphics::color _blue, synthciv::graphics::color _alpha) {
            red = _red;
            green = _green;
            blue = _blue;
            alpha = _alpha;
        }

        // write to buffer
        synthciv::current write_pixel_to_current(synthciv::current _current) {
            // write colors
            _current = write_color_to_current(_current, red);
            _current = write_color_to_current(_current, green);
            _current = write_color_to_current(_current, blue);
            _current = write_color_to_current(_current, alpha);

            return _current;
        }
    };

    // textures
    typedef synthciv::u16 texture_dimension;
    typedef synthciv::graphics::texture_dimension texture_x;
    typedef synthciv::graphics::texture_dimension texture_y;

    // texture cpu handle
    class face {
    public:
        std::vector<synthciv::graphics::pixel> pixels;
        synthciv::graphics::texture_x width;
        synthciv::graphics::texture_y height;
    };

    // texture faces
    typedef synthciv::u64 face_count;
    class faces {
    public:
        // opengl side
        GLuint handle;

        // cpu side
        std::vector<synthciv::graphics::face> faces_data;
        synthciv::graphics::texture_x max_width;
        synthciv::graphics::texture_y max_height;

        // constructors
        faces() {
            handle = 0;
            faces_data = std::vector<synthciv::graphics::face>();
            max_width = 0;
            max_height = 0;
        }

        // calculate max buffer size
        synthciv::length calculate_opengl_buffer_size() {
            return (synthciv::length)(faces_data.size() * max_width * max_height);
        }

        // serialize texture faces (ASSUMES ALL FACES ARE SAME PIXEL COUNT!!!)
        synthciv::buffer serialize_faces() {
            // calculate and preallocate buffer
            synthciv::buffer output;
            output.allocate(calculate_opengl_buffer_size());
            if (output.is_empty()) {
                return output;
            }

            // setup current
            synthciv::current current = synthciv::current(output, output.start);

            // write textures to opengl
            // for each texture
            for (synthciv::length face_index = 0; face_index < faces_data.size(); face_index++) {
                // for each face
                for (synthciv::length pixel_index = 0; pixel_index < faces_data[face_index].pixels.size(); pixel_index++) {
                    // write pixel
                    current = faces_data[face_index].pixels[pixel_index].write_pixel_to_current(current);
                }
            }

            // return buffer
            return output;
        }
    };

    // all game texture types
    typedef enum game_texture_type {
        // types
        block_faces = 0,

        // count
        COUNT,
    } game_texture_type;

    // texture target types
    const GLenum game_texture_type_block_faces_target_type = GL_TEXTURE_2D_ARRAY;

    // all game textures
    class game_textures {
    public:
        synthciv::graphics::faces block_faces;

        // constructors
        game_textures() {
            block_faces = synthciv::graphics::faces();
        }

        // bind & activate textures
        void bind_specific_texture(synthciv::graphics::game_texture_type _type, synthciv::graphics::shaders _shaders) {
            // set active texture
            glActiveTexture(GL_TEXTURE0 + (GLenum)_type);

            // bind
            switch (_type) {
            case synthciv::graphics::game_texture_type::block_faces:
                // bind texture buffer
                glBindTexture(synthciv::graphics::game_texture_type_block_faces_target_type, block_faces.handle);

                break;
            }

            // set active texture unit in opengl shader uniform
            glUniform1i(glGetUniformLocation(_shaders.program_ID, "GLOBAL_current_texture_unit"), GL_TEXTURE0 + (GLenum)_type);
        }

        // unbind current texture
        void unbind_specific_texture(synthciv::graphics::game_texture_type _type) {
            // unbind by type
            switch (_type) {
            case synthciv::graphics::game_texture_type::block_faces:
                // bind texture buffer
                glBindTexture(synthciv::graphics::game_texture_type_block_faces_target_type, 0);

                break;
            }
        }

        // send textures to opengl
        void serialize_and_send_textures(synthciv::graphics::game_texture_type _type, synthciv::graphics::faces _faces, synthciv::graphics::shaders _shaders) {
            // setup cpu side data for block faces
            synthciv::buffer opengl_faces_buffer = _faces.serialize_faces();
            if (opengl_faces_buffer.is_empty()) {
                return;
            }

            // create texture array
            glCreateTextures(_type, 1, (GLuint*)&(opengl_faces_buffer.start));

            // setup opengl side for block faces
            bind_specific_texture(synthciv::graphics::game_texture_type::block_faces, _shaders);
            glTexParameteri(synthciv::graphics::game_texture_type_block_faces_target_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(synthciv::graphics::game_texture_type_block_faces_target_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(synthciv::graphics::game_texture_type_block_faces_target_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(synthciv::graphics::game_texture_type_block_faces_target_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage3D(synthciv::graphics::game_texture_type_block_faces_target_type, 0, GL_RGBA8, block_faces.max_width, block_faces.max_height, block_faces.faces_data.size(), 0, GL_RGBA /* Do not change to GL_RGBA8, crashes */, GL_UNSIGNED_BYTE, opengl_faces_buffer.start);
            unbind_specific_texture(synthciv::graphics::game_texture_type::block_faces);

            // close temp buffer
            opengl_faces_buffer.deallocate();
        }
    };
}
