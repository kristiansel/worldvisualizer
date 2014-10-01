#ifndef GASJACOBISHADER_H
#define GASJACOBISHADER_H

#include "shaderbase.h"

class GasJacobiShader : public ShaderBase
{
    public:
        GasJacobiShader();
        virtual ~GasJacobiShader();

        void init(unsigned int dim,
                         float* vx_vy_p_rho,
                         float* source,
                         float viscosity,
                         float dl);

        GLuint step(float dt, int iterations, bool draw_to_screen);

    protected:
    private:
        struct Uniforms
        {
            GLuint x_t1k0;
            GLuint x_t0kN;
            GLuint source;

            GLuint mu;  // viscosity
            GLuint dl;  // physical grid distance
            GLuint dt;
            GLuint di; // distance in texture coordinate between values
        } uniforms;

        struct ConstUniformValues
        {
            float mu;
            float dl;
            float di;
        } uniform_vals;

        struct Textures
        {
            GLuint x_t0kN;
            GLuint source;
        } textures;

        struct Buffers
        {
            GLuint frame;
            GLuint texture;

            GLuint frame2;
            GLuint texture2;

            // used to save the final (previous solution)
            GLuint final_frame;

            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;

        // helper vars
        GLuint last_ping_pong_tex;
};

#endif // GASJACOBISHADER_H
