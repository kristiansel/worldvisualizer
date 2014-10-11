#include "waterdisplayshader.h"

WaterDisplayShader::WaterDisplayShader()
{
    //ctor
}

WaterDisplayShader::~WaterDisplayShader()
{
    //dtor
}

void WaterDisplayShader::init(GLuint height_texture)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/waterdisplayshader.glsl");

    // set the uniform locations
    uniforms.x_field = glGetUniformLocation(getProgramID(), "x_field");
    glUniform1i(uniforms.x_field, 0);

    // set the uniform locations
    uniforms.height = glGetUniformLocation(getProgramID(), "height");
    glUniform1i(uniforms.height, 1);

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

    textures.height = height_texture;
}

void WaterDisplayShader::draw(GLuint fluid_field_tex1)
{
    // start drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // switch to
    ShaderBase::switchTo();

    // activate textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fluid_field_tex1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // activate textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures.height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
