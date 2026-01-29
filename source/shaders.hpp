#pragma once

/* Include */
// synthciv
#include "basic.hpp"

/* Shaders */
namespace synthciv::graphics {
    // constants
    const synthciv::u64 opengl_error_log_max_length = 2048;

    // one shader
    class shader {
    public:
        // shader data
        GLuint shader_ID;
        std::string code;

        // error data
        std::string opengl_log;
        GLint opengl_log_length;
        GLenum opengl_error_code;

        // constructors
        shader() {
            shader_ID = 0;
            code = "";
            opengl_log = "";
            opengl_log_length = 0;
            opengl_error_code = GL_NO_ERROR;
        }
        // open and compile shader
        void open_shader(std::string _code, synthciv::error* _error, GLenum _shader_type) {
            // create new shader
            shader_ID = glCreateShader(_shader_type);

            // setup temp
            GLchar* temp = (GLchar*)_code.c_str();

            // send source code to gpu
            glShaderSource(shader_ID, 1, (const GLchar* const*)&temp, NULL);

            // compiler the shader
            glCompileShader(shader_ID);

            // check for opengl errors
            glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &opengl_log_length);
            if (opengl_log_length > 0) {
                // create place for opengl to dump log
                synthciv::character log[opengl_error_log_max_length];

                // get opengl error data
                glGetShaderInfoLog(shader_ID, synthciv::graphics::opengl_error_log_max_length, NULL, (GLchar*)&log);
                opengl_error_code = glGetError();

                // convert log error to string
                opengl_log = std::string((char*)&log, opengl_log_length);

                // generate synthciv error
                if (_shader_type == GL_VERTEX_SHADER) {
                    *_error = synthciv::error(true, "\"error\": {\n\t\"reason\": \"OpenGL vertex shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
                } else if (_shader_type == GL_FRAGMENT_SHADER) {
                    *_error = synthciv::error(true, "\"error\": {\n\t\"reason\": \"OpenGL fragment shader did not compile.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
                }
            }

            return;
        }

        // close shader
        void close_shader() {
            glDeleteShader(shader_ID);
        }
    };

    // one shader program
    class shaders {
    public:
        // shader program data
        GLuint program_ID;
        synthciv::graphics::shader vertex_shader;
        synthciv::graphics::shader fragment_shader;

        // error data
        std::string opengl_log;
        GLint opengl_log_length;
        GLenum opengl_error_code;

        // constructors
        shaders() {
            program_ID = 0;
            vertex_shader = synthciv::graphics::shader();
            fragment_shader = synthciv::graphics::shader();
            opengl_log = "";
            opengl_log_length = 0;
            opengl_error_code = GL_NO_ERROR;
        }

        // open shaders
        void open_shaders(synthciv::error* _error, std::string _vertex_shader, std::string _fragment_shader) {
            // compile vertex shader
            vertex_shader.open_shader(_vertex_shader, _error, GL_VERTEX_SHADER);
            if ((*_error).occured) {
                return;
            }

            // compile fragment shader
            fragment_shader.open_shader(_fragment_shader, _error, GL_FRAGMENT_SHADER);
            if ((*_error).occured) {
                return;
            }

            // create shader program
            program_ID = glCreateProgram();

            // setup shader linking
            glAttachShader(program_ID, vertex_shader.shader_ID);
            glAttachShader(program_ID, fragment_shader.shader_ID);

            // link shaders
            glLinkProgram(program_ID);

            // check for opengl errors
            glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &opengl_log_length);
            if (opengl_log_length > 0) {
                // create place for opengl to dump log
                synthciv::character log[opengl_error_log_max_length];
                
                // get opengl error data
                glGetProgramInfoLog(program_ID, synthciv::graphics::opengl_error_log_max_length, NULL, (GLchar*)&log);
                opengl_error_code = glGetError();

                // convert log error to string
                opengl_log = std::string((char*)&log, opengl_log_length);

                // generate synthciv error
                *_error = synthciv::error(true, "\"error\": {\n\t\"reason\": \"OpenGL shaders did not link together.\",\n\t\"OpenGL Error Code\": " + std::to_string((int)opengl_error_code) + ",\n\t\"OpenGL Log\": \"" + opengl_log + "\"\n}");
            }
        }

        // use shader program
        void use_program() {
            glUseProgram(program_ID);
        }

        // close shaders
        void close_shaders() {
            // close individual shaders
            vertex_shader.close_shader();
            fragment_shader.close_shader();

            // close shader program
            glDeleteProgram(program_ID);
        }
    };
}
