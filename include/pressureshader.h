#ifndef PRESSURESHADER_H
#define PRESSURESHADER_H

#include "shaderbase.h"

class PressureShader : public ShaderBase
{
    public:
        PressureShader();
        virtual ~PressureShader();

        void init(unsigned int dim, GLuint x_dash_tex_id,
                                float density,     // use as uniform
                                float viscosity,   // use as uniform
                                float dl);         // use as uniform

        GLuint step(float dt, int iterations, bool draw_to_screen = false);

        GLuint getVelCorrTextureID();

    protected:
    private:
        struct Uniforms
        {
            GLuint x_dash;

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
            GLuint x_dash;
        } textures;

        struct Buffers
        {
            GLuint frame;
            GLuint texture;

            GLuint frame2;
            GLuint texture2;

            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;

        // helper vars
        GLuint last_ping_pong_tex;
};

#endif // PRESSURESHADER_H
