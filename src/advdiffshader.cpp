#include "advdiffshader.h"

AdvDiffShader::AdvDiffShader()
{
    //ctor
}

AdvDiffShader::~AdvDiffShader()
{
    //dtor
}

void AdvDiffShader::init(unsigned int dim, void* vx_vy_p, void* source, float density, float viscosity, float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/advdiffshader.glsl");

 // set the uniform locations
    uniforms.x_prev = glGetUniformLocation(getProgramID(), "x_prev");
    glUniform1i(uniforms.x_prev, 0);

    uniforms.source = glGetUniformLocation(getProgramID(), "source");
    glUniform1i(uniforms.source, 1);

    uniforms.rho = glGetUniformLocation(getProgramID(), "rho");
    uniforms.mu = glGetUniformLocation(getProgramID(), "mu");
    uniforms.dl = glGetUniformLocation(getProgramID(), "dl");
    uniforms.di = glGetUniformLocation(getProgramID(), "di");
    uniforms.dt = glGetUniformLocation(getProgramID(), "dt");


    //Generate a draw-to-texture
    glGenTextures(1, &buffers.texture);
    glBindTexture(GL_TEXTURE_2D, buffers.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, dim, dim, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //generate framebuffer
    glGenFramebuffers(1, &buffers.frame);
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.frame);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffers.texture, 0);

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Init its own textures and load the data
    glGenTextures(1, &textures.vx_vy_p);
    glBindTexture(GL_TEXTURE_2D, textures.vx_vy_p);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, dim, dim, 0, GL_RGB, GL_FLOAT, vx_vy_p);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void AdvDiffShader::step(float dt)
{
    // switch to

    // Send uniforms
    glUniform1i(uniforms.x_prev, 0);
    glUniform1i(uniforms.source, 1);


    // render
}

