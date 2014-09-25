#ifndef FLUIDDISPLAYSHADER_H
#define FLUIDDISPLAYSHADER_H


#include "shaderbase.h"

class FluidDisplayShader : public ShaderBase
{
    public:
        FluidDisplayShader();
        virtual ~FluidDisplayShader();

        void init();

        void draw(GLuint fluid_field_tex1/*, GLuint fluid_field_tex2*/); // second texture for later use
    protected:
    private:
        struct Uniforms
        {
            GLuint x_field;
        } uniforms;

        struct Buffers
        {
            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;
};

#endif // FLUIDDISPLAYSHADER_H
