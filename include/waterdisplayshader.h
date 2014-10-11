#ifndef WATERDISPLAYSHADER_H
#define WATERDISPLAYSHADER_H

#include "shaderbase.h"

class WaterDisplayShader : public ShaderBase
{
    public:
        WaterDisplayShader();
        virtual ~WaterDisplayShader();

        void init(GLuint height_texture);

        void draw(GLuint solution_tex);
    protected:
    private:
        struct Uniforms
        {
            GLuint x_field;
            GLuint height;
        } uniforms;

        struct Buffers
        {
            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;

        struct Textures
        {
            GLuint height;
        } textures;
};

#endif // WATERDISPLAYSHADER_H
