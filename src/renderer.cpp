#include "renderer.h"

Renderer::Renderer()
{
    //ctor
}

Renderer::~Renderer()
{
    //dtor
}

void Renderer::init(unsigned int scr_width_in, unsigned int scr_height_in)
{
    m_layer.resize(32);

    cout << "Status: Using OpenGL " << glGetString(GL_VERSION) << "\n";

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        cerr << "Error: " << glewGetErrorString(err) << "\n";
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";

    // Set clear values
    //glClearColor(0.f/255.f, 80.f/255.f, 186.f/255.f, 0.0);

    glClearColor(0.f, 0.f, 0.5f, 0.f);
    glClearDepth(1.0);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_mainShader.init();
    std::cout << "got here\n";

    m_x = scr_width_in;
    m_y = scr_height_in;


}

void Renderer::resizeWindow(int w, int h)
{

}

void Renderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto layer : m_layer)
    {
        if (layer.isActive())
        {
            m_mainShader.draw(layer);
        }
    }
//
//    m_mainShader.draw(m_layer);
}

void Renderer::loadLayer(int layer, unsigned int dim, void* layer_data)
{
    Layer* last_layer = &m_layer[layer];

    last_layer->init(m_x);
    last_layer->loadLayerData(dim, layer_data);
//    m_layer.loadLayerData(layer_data);
}

void Renderer::updateLayer(int layer, void* layer_data)
{
    m_layer[layer].updateLayerData(layer_data);
}


void Renderer::unloadLayer(int layer)
{

}

void Renderer::setLayerScale(int layer, float min_val, float max_val)
{

}

void Renderer::maskLayerGreater(int layer, float threshold)
{

}

void Renderer::maskLayerLess(int layer, float threshold)
{

}
