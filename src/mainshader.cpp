#include "mainshader.h"

MainShader::MainShader()
{
    //ctor
}

MainShader::~MainShader()
{
    //dtor
}

void MainShader::init()
{
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/main_shader_frag.glsl");

 // set the uniform locations
    uniforms.layerTexture = glGetUniformLocation(getProgramID(), "layerTexture");
    glUniform1i(uniforms.layerTexture, 0);            // ALWAYS CHANNEL 0

    uniforms.max_val = glGetUniformLocation(getProgramID(), "max_val");
    uniforms.min_val = glGetUniformLocation(getProgramID(), "min_val");
    uniforms.cut_below = glGetUniformLocation(getProgramID(), "cut_below");
    uniforms.cut_above = glGetUniformLocation(getProgramID(), "cut_above");
    uniforms.alpha = glGetUniformLocation(getProgramID(), "alpha");

    uniforms.color_transf = glGetUniformLocation(getProgramID(), "color_transf");

    glEnableVertexAttribArray(0);

    GLfloat fbo_vertices[] =
    {
        -1, -1,
        1, -1,
        -1,  1,
        1,  1,
    };
    glGenBuffers(1, &buffers.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MainShader::unload()
{
//    ShaderBase::unload();
}

void MainShader::draw(Layer layer)
{
    // switch to main shader
    ShaderBase::switchTo();

    // Activate and bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, layer.getTexture());

    glUniform1f(uniforms.max_val, layer.getMax());
    glUniform1f(uniforms.min_val, layer.getMin());
    glUniform1f(uniforms.cut_above, layer.getCutAbove());
    glUniform1f(uniforms.cut_below, layer.getCutBelow());
    glUniform1f(uniforms.alpha, layer.getAlpha());

    float color[3];
    layer.getColorTransf(color);

    glUniform3fv(uniforms.color_transf, 1, color);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertex_buffer);
    glVertexAttribPointer(
        0,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

