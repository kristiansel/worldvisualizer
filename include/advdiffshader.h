#ifndef ADVDIFFSHADER_H
#define ADVDIFFSHADER_H

#include "shaderbase.h"
#include "closedmap.h"

class AdvDiffShader : public ShaderBase
{
    public:
        AdvDiffShader();
        virtual ~AdvDiffShader();

        void init(unsigned int dim, void* vx_vy_p, // Create the source texture
                                void* source,
                                float density,     // use as uniform
                                float viscosity,   // use as uniform
                                float dl);         // use as uniform
        void step(float dt);

    protected:
    private:
        struct Uniforms
        {
            GLuint x_prev;
            GLuint source;

            GLuint rho;
            GLuint mu;
            GLuint dl;
            GLuint dt;
            GLuint di;
        } uniforms;

        struct ConstUniformValues
        {
            float rho;
            float mu;
            float dl;
            float di;
        } uniform_vals;

        struct Textures
        {
            GLuint vx_vy_p;
            GLuint source;
        } textures;

        struct FrameBuffer
        {
            GLuint frame;
            GLuint texture;
        } buffers;
};

#endif // ADVDIFFSHADER_H
