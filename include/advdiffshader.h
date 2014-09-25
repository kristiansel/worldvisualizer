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
        void step(float dt, bool draw_to_screen = false);

        GLuint getXdashTextureID();
        GLuint getInitCondTextureID();

    protected:
    private:
        struct Uniforms
        {
            GLuint x_prev;
            GLuint source;

            GLuint rho; // density
            GLuint mu;  // viscosity
            GLuint dl;  // physical grid distance
            GLuint dt;
            GLuint di; // distance in texture coordinate between values
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

        struct Buffers
        {
            GLuint frame;
            GLuint texture;

            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;
};

#endif // ADVDIFFSHADER_H
