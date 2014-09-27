#include "fluiddisplayshader.h"

FluidDisplayShader::FluidDisplayShader()
{
    //ctor
}

FluidDisplayShader::~FluidDisplayShader()
{
    //dtor
}

void FluidDisplayShader::init(unsigned int dim, float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/fluiddisplayshader.glsl");

 // set the uniform locations
    uniforms.x_field = glGetUniformLocation(getProgramID(), "x_field");
    glUniform1i(uniforms.x_field, 0);

    uniforms.di = glGetUniformLocation(getProgramID(), "di");
    uniforms.dl = glGetUniformLocation(getProgramID(), "dl");

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

    uniform_vals.di = 1.f/((float)(dim));
    uniform_vals.dl = dl;
}

void FluidDisplayShader::draw(GLuint fluid_field_tex1)
{
    // start drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // switch to
    ShaderBase::switchTo();

    // uniforms
    glUniform1f(uniforms.di, uniform_vals.di);
    glUniform1f(uniforms.dl, uniform_vals.dl);

    // activate textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fluid_field_tex1);

    // render
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
