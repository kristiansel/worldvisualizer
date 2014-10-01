#include "gasjacobishader.h"

GasJacobiShader::GasJacobiShader()
{
    //ctor
}

void GasJacobiShader::init(unsigned int dim,
                         float* vx_vy_p_rho,
                         float* source,
                         float viscosity,
                         float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/gasjacobishader.glsl");

 // set the uniform locations
    uniforms.x_t1k0 = glGetUniformLocation(getProgramID(), "x_t1k0");
    glUniform1i(uniforms.x_t1k0, 0);

    uniforms.x_t0kN = glGetUniformLocation(getProgramID(), "x_t0kN");
    glUniform1i(uniforms.x_t0kN, 1);

    uniforms.source = glGetUniformLocation(getProgramID(), "source");
    glUniform1i(uniforms.source, 2);

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

    //Initialize previous time-step solution texture
    glGenTextures(1, &textures.x_t0kN);
    glBindTexture(GL_TEXTURE_2D, textures.x_t0kN);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, vx_vy_p_rho);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    //generate framebuffer for the final frame (stores previous time step solution)
    glGenFramebuffers(1, &buffers.final_frame);
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.final_frame);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures.x_t0kN, 0);

    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "glCheckFramebufferStatus original image: error %p", status);
        std::cout<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Init its own textures and load the data
    glGenTextures(1, &textures.source);
    glBindTexture(GL_TEXTURE_2D, textures.source);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, source);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    uniform_vals.mu = viscosity;

    std::cout << "finished initializing gas jacobi shader\n";
}

GLuint GasJacobiShader::step(float dt, int iterations, bool draw_to_screen)
{
    // switch to
    ShaderBase::switchTo();

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.frame);

    // turn off alpha blending
    glDisable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);

    glUniform1f(uniforms.mu, uniform_vals.mu);
    glUniform1f(uniforms.dl, uniform_vals.dl);
    glUniform1f(uniforms.di, uniform_vals.di);
    glUniform1f(uniforms.dt, dt);

    // activate textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures.x_t0kN);

    // activate textures
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures.source);

    // bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertex_buffer);
    glVertexAttribPointer(
        0,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );

    GLuint read_texture = textures.x_t0kN; // start from the previous timestep
    GLuint write_buffer = buffers.frame; // start by writing to first framebuffer

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
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // swap read and write
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
    }


    // Store this field for referencing in next time step
    glBindFramebuffer(GL_FRAMEBUFFER, buffers.final_frame);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // stop drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (draw_to_screen) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);









//    GLuint read_texture = textures.x_dash;
//    GLuint write_buffer = buffers.frame;
//
//    for (int i = 0; i<iterations; i++)
//    {
//        // start drawing to framebuffer
//        if (i==iterations-1)
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        else
//            glBindFramebuffer(GL_FRAMEBUFFER, write_buffer);
//
//
//        // activate textures
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, read_texture);
//
//        // activate textures
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, textures.source);
//        // render
//        glBindBuffer(GL_ARRAY_BUFFER, buffers.vertex_buffer);
//        glVertexAttribPointer(
//            0,  // attribute
//            2,                  // number of elements per vertex, here (x,y)
//            GL_FLOAT,           // the type of each element
//            GL_FALSE,           // take our values as-is
//            0,                  // no extra data between each position
//            0                   // offset of first element
//        );
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//        // ping_pong
////        if (read_texture == textures.x_dash) // after first draw
////        {
////            read_texture = buffers.texture;
////            write_buffer = buffers.frame2;
////        }
////        else
////        {
//        if (read_texture == buffers.texture)    // previous read 1, wrote 2
//        {
//            read_texture = buffers.texture2;
//            write_buffer = buffers.frame;
//        }
//        else                                    // previous read 2, wrote 1
//        {
//            read_texture = buffers.texture;
//            write_buffer = buffers.frame2;
//        }
////        }
//    }
//
//    last_ping_pong_tex = read_texture;
//
//    // stop drawing to framebuffer
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    if (draw_to_screen) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    return last_ping_pong_tex;

    return 1;
}


GasJacobiShader::~GasJacobiShader()
{
    //dtor
}
