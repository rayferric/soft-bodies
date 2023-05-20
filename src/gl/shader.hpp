// THIS IS OLD AND BADLY WRITTEN CODE; NOT FOR REFERENCE

#pragma once

#include <math/math.hpp>
#include <util/util.hpp>

namespace soft_bodies::gl {

class shader {
private:
    unsigned int program;

    unsigned int create_shader(const char *code, unsigned int type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE) {
            int size;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
            GLchar info[size];
            glGetShaderInfoLog(shader, size, nullptr, &info[0]);
            glDeleteShader(shader);
            SOFT_BODIES_ASSERT(false, std::string{"Shader compilation error: "} + info);
        }
        return shader;
    }
public:
    shader(const std::string &vert, const std::string &frag) {
        unsigned int vertex = create_shader(vert.c_str(), GL_VERTEX_SHADER);
        unsigned int fragment = create_shader(frag.c_str(), GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        int status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if(status == GL_FALSE) {
            int size;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
            GLchar info[size];
            glGetProgramInfoLog(program, size, nullptr, &info[0]);
            glDetachShader(program, vertex);
            glDetachShader(program, fragment);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(program);
            SOFT_BODIES_ASSERT(false, std::string{"Shader linking error: "} + info);
        }
        glValidateProgram(program);
        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void bind() {
        glUseProgram(program);
    }
    int get_uniform_location(const std::string &name) {
        return glGetUniformLocation(program, name.c_str());
    }

    static void set_uniform(int location, int value) {
        glUniform1i(location, value);
    }

    static void set_uniform(int location, float value) {
        glUniform1f(location, value);
    }

    static void set_uniform(int location, math::fvec2 value) {
        glUniform2f(location, value.x, value.y);
    }

    static void set_uniform(int location, math::fvec3 value) {
        glUniform3f(location, value.x, value.y, value.z);
    }

    static void set_uniform(int location, math::fvec4 value) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    static void set_uniform(int location, math::fmat2 value) {
        glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
    }

    static void set_uniform(int location, math::fmat3 value) {
        glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
    }

    ~shader() {
        glDeleteProgram(program);
    }
};

}
