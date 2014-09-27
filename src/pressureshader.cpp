#include "pressureshader.h"

PressureShader::PressureShader()
{
    //ctor
}

PressureShader::~PressureShader()
{
    //dtor
}

void PressureShader::init(unsigned int dim,
                         GLuint x_dash_tex_id,
                         float density,
                         float viscosity,
                         float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/pressureshader.glsl");

 // set the uniform locations
    uniforms.x_dash = glGetUniformLocation(getProgramID(), "x_dash");
    glUniform1i(uniforms.x_dash, 0);

    uniforms.rho = glGetUniformLocation(getProgramID(), "rho");
    uniforms.mu = glGetUniformLocation(getProgramID(), "mu");
    uniforms.dl = glGetUniformLocation(getProgramID(), "dl");
    uniforms.di = glGetUniformLocation(getProgramID(), "di");
    uniforms.dt = glGetUniformLocation(getProgramID(), "dt");

    //Generate a draw-to-texture
    glGenTextures(1, &buffers.texture);
    glBindTexture(GL_TEXTURE_2D, buffers.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, 0);
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

    //Generate a draw-to-texture2
    glGenTextures(1, &buffers.texture2);
    glBindTexture(GL_TEXTURE_2D, buffers.texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //generate framebuffer
    glGenFramebuffers(1, &buffers.frame2);
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.frame2);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffers.texture2, 0);

    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    textures.x_dash = x_dash_tex_id;

//    // Init its own textures (no data to load yet)
//    glGenTextures(1, &textures.x_dash);
//    glBindTexture(GL_TEXTURE_2D, textures.x_dash);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//    glBindTexture(GL_TEXTURE_2D, 0);

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

GLuint PressureShader::step(float dt, int iterations, bool draw_to_screen)
{

    // switch to
    ShaderBase::switchTo();

    // turn off alpha blending
    glDisable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);

    glUniform1f(uniforms.rho, uniform_vals.rho);
    glUniform1f(uniforms.mu, uniform_vals.mu);
    glUniform1f(uniforms.dl, uniform_vals.dl);
    glUniform1f(uniforms.di, uniform_vals.di);
    glUniform1f(uniforms.dt, dt);

    GLuint read_texture = textures.x_dash;
    GLuint write_buffer = buffers.frame;

    for (int i = 0; i<iterations; i++)
    {
        // start drawing to framebuffer
        if (i==iterations-1)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        else
            glBindFramebuffer(GL_FRAMEBUFFER, write_buffer);


        // activate textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, read_texture);

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

        // ping_pong
//        if (read_texture == textures.x_dash) // after first draw
//        {
//            read_texture = buffers.texture;
//            write_buffer = buffers.frame2;
//        }
//        else
//        {
        if (read_texture == buffers.texture)    // previous read 1, wrote 2
        {
            read_texture = buffers.texture2;
            write_buffer = buffers.frame;
        }
        else                                    // previous read 2, wrote 1
        {
            read_texture = buffers.texture;
            write_buffer = buffers.frame2;
        }
//        }
    }

    last_ping_pong_tex = read_texture;

    // stop drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (draw_to_screen) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    return last_ping_pong_tex;
}

GLuint PressureShader::getVelCorrTextureID()
{
    return last_ping_pong_tex;
}
