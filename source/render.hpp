#pragma once

/* Include */
#include "shaders.hpp"

/* Rendering */
namespace synthciv::graphics {
    // vertex defines
    namespace vertex_defines {
        const synthciv::u64 x = 0;
        const synthciv::u64 y = 1;
        const synthciv::u64 z = 2;
    }

    // write and advance f32
    synthciv::current write_f32_to_current(synthciv::current _current, synthciv::f32 _float) {
        // write color
        *((synthciv::f32*)_current.progress) = _float;

        // advance
        _current.progress = _current.progress + sizeof(synthciv::f32);

        return _current;
    }

    // vertex
    class vertex {
    public:
        glm::vec3 vertices;

        vertex() {
            vertices[synthciv::graphics::vertex_defines::x] = 0.0f;
            vertices[synthciv::graphics::vertex_defines::y] = 0.0f;
            vertices[synthciv::graphics::vertex_defines::z] = 0.0f;
        }
        vertex(glm::vec3 _vertices) {
            vertices = _vertices;
        }

        // write and advance vertex
        synthciv::current write_vertex_to_current(synthciv::current _current) {
            // write floats
            _current = synthciv::graphics::write_f32_to_current(_current, vertices[synthciv::graphics::vertex_defines::x]);
            _current = synthciv::graphics::write_f32_to_current(_current, vertices[synthciv::graphics::vertex_defines::y]);
            _current = synthciv::graphics::write_f32_to_current(_current, vertices[synthciv::graphics::vertex_defines::z]);

            return _current;
        }
    };

    // render (chunk opengl) vertex
    class chunk_vertex {
    public:
        synthciv::graphics::vertex position;
        synthciv::graphics::vertex texture;

        // constructors
        chunk_vertex(synthciv::graphics::vertex _position, synthciv::graphics::vertex _texture) {
            position = _position;
            texture = _texture;
        }

        // write and advance vertex
        synthciv::current write_chunk_vertex_to_current(synthciv::current _current) {
            // write floats
            _current = position.write_vertex_to_current(_current);
            _current = texture.write_vertex_to_current(_current);

            return _current;
        }
    };

    class renderer {
    public:
        void render_block_face() {
            // allocate square (2 triangles)
            synthciv::buffer temp_space;
            temp_space.allocate(sizeof(synthciv::graphics::chunk_vertex) * 6);

            // setup current
            synthciv::current current;
            current = synthciv::current(temp_space, temp_space.start);

            // write first triangle
            synthciv::graphics::chunk_vertex triangle_A = synthciv::graphics::chunk_vertex(synthciv::graphics::vertex({ -0.5,  }));
            current = 
        }
    };
}
