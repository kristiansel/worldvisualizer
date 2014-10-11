#include "shallowwatershader.h"

ShallowWaterShader::ShallowWaterShader()
{
    //ctor
}

void ShallowWaterShader::init(unsigned int dim,
                         float* u_v_h,
                         float* height,
                         float gravity,
                         float dl)
{
    // Init its own shader
    // with input uniforms
    ShaderBase::load("shaders/main_shader_vert.glsl", "shaders/shallowwatershader.glsl");

 // set the uniform locations
    uniforms.x_t0 = glGetUniformLocation(getProgramID(), "x_t0");
    glUniform1i(uniforms.x_t0, 0);

    uniforms.height = glGetUniformLocation(getProgramID(), "height");
    glUniform1i(uniforms.height, 1);

    uniforms.g = glGetUniformLocation(getProgramID(), "g");
    uniforms.dl = glGetUniformLocation(getProgramID(), "dl");
    uniforms.di = glGetUniformLocation(getProgramID(), "di");
    uniforms.dt = glGetUniformLocation(getProgramID(), "dt");

    //Generate a draw-to-texture
    glGenTextures(1, &buffers.texture);
    glBindTexture(GL_TEXTURE_2D, buffers.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dim, dim, 0, GL_RGBA, GL_FLOAT, u_v_h);
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

    last_ping_pong_tex = buffers.texture; // initualize framebuffer 1, (start by drawing to 2)

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Init its own textures and load the data
    glGenTextures(1, &textures.height);
    glBindTexture(GL_TEXTURE_2D, textures.height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dim, dim, 0, GL_RED, GL_FLOAT, height);
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
    uniform_vals.g = gravity;

    m_dim = dim;

    std::cout << "finished initializing shallowwater shader\n";
}

GLuint ShallowWaterShader::step(float dt, bool draw_to_screen)
{
    // switch to
    ShaderBase::switchTo();

    // turn off alpha blending
    glDisable(GL_BLEND);

    glUniform1f(uniforms.g, uniform_vals.g);
    glUniform1f(uniforms.dl, uniform_vals.dl);
    glUniform1f(uniforms.di, uniform_vals.di);
    glUniform1f(uniforms.dt, dt);

//    std::cout << "g = " << uniform_vals.g << "\n";
//    std::cout << "dl = " << uniform_vals.dl << "\n";
//    std::cout << "di = " << uniform_vals.di << "\n";
//    std::cout << "dt = " << dt << "\n";

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

    GLuint read_texture; // start from the previous timestep
    GLuint write_buffer; // start by writing to first framebuffer

    // swap read and write
    if (last_ping_pong_tex == buffers.texture)    // previous wrote 1, read 2
    {
        read_texture = buffers.texture;
        write_buffer = buffers.frame2;
    }
    else                                    // previous wrote 2, read 1
    {
        read_texture = buffers.texture2;
        write_buffer = buffers.frame;
    }


    // activate textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, read_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // activate textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures.height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, write_buffer);

    // render
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // stop drawing to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    last_ping_pong_tex = (write_buffer == buffers.frame) ? buffers.texture : buffers.texture2;

    if (draw_to_screen) glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //system("pause");

    return 1;
}

void ShallowWaterShader::writeBackResults(unsigned int dim, float* u_v_h)
{
    glBindTexture(GL_TEXTURE_2D, last_ping_pong_tex);
    glGetTexImage( GL_TEXTURE_2D,
                        0,
                        GL_RGBA,
                        GL_FLOAT,
                        u_v_h);
}

void ShallowWaterShader::stirWater(float rel_x, float rel_y)
{
    float scr_perc = 0.1;

    unsigned int offset_x = (rel_x-scr_perc/2.f)*m_dim;
    unsigned int offset_y = (1-(rel_y+scr_perc/2.f))*m_dim;

    unsigned int width = int(scr_perc*m_dim);
    unsigned int height = width;

    unsigned int n_valsPerCell = 4;
    float* u_v_h = new float [n_valsPerCell*width*height];
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height; j++)
        {
            float* vp_node = &u_v_h[n_valsPerCell*i*width+n_valsPerCell*j];
            vp_node[0] = 0;
            vp_node[1] = 0;
            vp_node[2] = 100;
        }
    }

    glBindTexture(GL_TEXTURE_2D, last_ping_pong_tex); // being read next
    glTexSubImage2D(GL_TEXTURE_2D, 0, offset_x, offset_y, width, height, GL_RGBA, GL_FLOAT, u_v_h);


}



ShallowWaterShader::~ShallowWaterShader()
{
    //dtor
}
