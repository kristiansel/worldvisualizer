#ifndef VELCORRSHADER_H
#define VELCORRSHADER_H

#include "shaderbase.h"

class VelCorrShader : public ShaderBase
{
    public:
        VelCorrShader();
        virtual ~VelCorrShader();

        void init(unsigned int dim, GLuint init_cond_tex,
                                float density,     // use as uniform
                                float viscosity,   // use as uniform
                                float dl);         // use as uniform

        GLuint step(float dt, GLuint source_tex, bool draw_to_screen = false);

        GLuint getSolutionTexID();

    protected:
    private:
        struct Uniforms
        {
            GLuint x_pcorr;

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

        struct Buffers
        {
            GLuint frame;
            GLuint texture;

            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;
};

#endif // VELCORRSHADER_H
