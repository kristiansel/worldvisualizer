#include "velcorrshader.h"

VelCorrShader::VelCorrShader()
{
    //ctor
}

VelCorrShader::~VelCorrShader()
{
    //dtor
}

void VelCorrShader::init(unsigned int dim, GLuint init_cond_tex,
                         float density,
                         float viscosity,
                         float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/velcorrshader.glsl");

 // set the uniform locations
    uniforms.x_pcorr = glGetUniformLocation(getProgramID(), "x_pcorr");
    glUniform1i(uniforms.x_pcorr, 0);

    uniforms.rho = glGetUniformLocation(getProgramID(), "rho");
    uniforms.mu = glGetUniformLocation(getProgramID(), "mu");
    uniforms.dl = glGetUniformLocation(getProgramID(), "dl");
    uniforms.di = glGetUniformLocation(getProgramID(), "di");
    uniforms.dt = glGetUniformLocation(getProgramID(), "dt");


    //Generate a draw-to-texture
//    glGenTextures(1, &buffers.texture);
//    glBindTexture(GL_TEXTURE_2D, buffers.texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //generate framebuffer
    glGenFramebuffers(1, &buffers.frame);
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.frame);
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffers.texture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, init_cond_tex, 0);

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

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

    // Set uniform values
    uniform_vals.di = 1.f/((float)(dim)); // calculate the texco offset of one grid-cell
    uniform_vals.dl = dl;
    uniform_vals.rho = density;
    uniform_vals.mu = viscosity;
}

GLuint VelCorrShader::step(float dt, GLuint source_tex, bool draw_to_screen)
{
    // start drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.frame);

    // switch to
    ShaderBase::switchTo();

    // Send uniforms
    //glUniform1i(uniforms.x_prev, 0);
    //glUniform1i(uniforms.source, 1);

    glUniform1f(uniforms.rho, uniform_vals.rho);
    glUniform1f(uniforms.mu, uniform_vals.mu);
    glUniform1f(uniforms.dl, uniform_vals.dl);
    glUniform1f(uniforms.di, uniform_vals.di);
    glUniform1f(uniforms.dt, dt);


    // activate textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source_tex);

    // turn off alpha blending
    glDisable(GL_BLEND);

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

    // stop drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (draw_to_screen) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    return buffers.texture;
}

GLuint VelCorrShader::getSolutionTexID()
{
    return buffers.texture;
}
