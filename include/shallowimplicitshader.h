#ifndef SHALLOWWATERSHADER_H
#define SHALLOWWATERSHADER_H

#include "shaderbase.h"

class ShallowImplicitShader : public ShaderBase
{
    public:
        ShallowImplicitShader();
        virtual ~ShallowImplicitShader();

        void init(unsigned int dim,
                         float* u_v_h,
                         float* height_data,
                         float gravity,
                         float dl);

        GLuint step(float dt, bool draw_to_screen);

        void stirWater(float rel_x, float rel_y);

        GLuint getHeightTex() {return textures.height;};

        void writeBackResults(unsigned int dim, float* u_v_h);

    protected:
    private:
        struct Uniforms
        {
            GLuint x_t1k0;
            GLuint x_t0;
            GLuint height;

            GLuint g;  // gravity
            GLuint dl; // physical grid distance
            GLuint dt; // timestep
            GLuint di; // distance in texture coordinate between values
        } uniforms;

        struct ConstUniformValues
        {
            float g;
            float dl;
            float di;
        } uniform_vals;

        struct Textures
        {
            GLuint height;
        } textures;

        struct Buffers
        {
            GLuint frame;
            GLuint texture;

            GLuint frame2;
            GLuint texture2;

            GLuint frame3;
            GLuint texture3;

            // Vertex buffer
            GLuint vertex_buffer;
        } buffers;

        // helper vars
        //GLuint last_iteration_texture;
        GLuint last_timestep_texture;

        unsigned int m_dim;
};

#endif // SHALLOWWATERSHADER_H
