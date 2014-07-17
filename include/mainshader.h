#ifndef MAINSHADER_H
#define MAINSHADER_H

#include "shaderbase.h"
#include "layer.h"

using std::string;
using std::cerr;

class MainShader : public ShaderBase
{
    public:
        MainShader();
        virtual ~MainShader();

        void init();
        void unload();

        void draw(Layer layer);

    protected:
    private:
        struct Uniforms
        {
            GLuint layerTexture;
            GLuint max_val;
            GLuint min_val;
            GLuint cut_below;
            GLuint cut_above;
            GLuint color_transf;
            GLuint alpha;
        } uniforms;

        struct Attributes
        {
            GLuint vertices;
        } attributes;

        struct Buffers
        {
            GLuint vertex_buffer;
        } buffers;
};

#endif // MAINSHADER_H
