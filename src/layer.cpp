#include "layer.h"

Layer::Layer() :
    m_initialized(false),
    m_dataLoaded(false),
    m_minVal(-1.0),
    m_maxVal(1.0),
    m_cutAboveVal(10000.0),
    m_cutBelowVal(-10000.0),
    m_col({1.0, 1.0, 1.0}),
    m_alpha(1.0)
{
    //ctor
}

Layer::~Layer()
{
    //dtor
}

void Layer::setMax(float max_val)
{
    m_maxVal = max_val;
}

void Layer::setMin(float min_val)
{
    m_minVal = min_val;
}

void Layer::setCutBelow(float in)
{
    m_cutBelowVal = in;
}

void Layer::setCutAbove(float in)
{
    m_cutAboveVal = in;
}
float Layer::getCutBelow()
{
    return m_cutBelowVal;
}

float Layer::getCutAbove()
{
    return m_cutAboveVal;
}

float Layer::getMax()
{
    return m_maxVal;
}

float Layer::getMin()
{
    return m_minVal;
}

void Layer::setColorTransf(float* rgb)
{
    m_col[0] = rgb[0];
    m_col[1] = rgb[1];
    m_col[2] = rgb[2];
}

void Layer::getColorTransf(float* col)
{
    col[0] = m_col[0];
    col[1] = m_col[1];
    col[2] = m_col[2];
}


void Layer::init(unsigned int dimensions)
{
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, dimensions, dimensions, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

    //m_dimensions = dimensions;

    m_initialized = true;
}

bool Layer::isActive()
{
    return m_initialized;
}

void Layer::loadLayerData(unsigned int dim, void* layerData)
{
    if (m_initialized)
    {
        m_dimensions = dim;

        glBindTexture(GL_TEXTURE_2D, m_texture_id);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_dimensions, m_dimensions, 0, GL_DEPTH_COMPONENT, GL_FLOAT, layerData);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dim, dim, 0, GL_RED, GL_FLOAT, layerData);
//        glGenerateMipmap(GL_TEXTURE_2D);    // Needed for texture filtering
        glBindTexture(GL_TEXTURE_2D, 0);

        GLenum errCode;
        errCode = glGetError();
        if (errCode != GL_NO_ERROR) {
            std::cerr << "glGetError()) != GL_NO_ERROR-----TEXTURE------\n";
//            std::cerr << ()
        }
        else
        {
            std::cout << "successfully loaded texture buffer" << m_texture_id << "\n";
            m_dataLoaded = true;
        }
    }

}

void Layer::updateLayerData(void* layerData)
{
    if (m_initialized)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_dimensions, m_dimensions, 0, GL_DEPTH_COMPONENT, GL_FLOAT, layerData);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_dimensions, m_dimensions, 0, GL_RED, GL_FLOAT, layerData);
//        glGenerateMipmap(GL_TEXTURE_2D);    // Needed for texture filtering
        glBindTexture(GL_TEXTURE_2D, 0);

        GLenum errCode;
        errCode = glGetError();
        if (errCode != GL_NO_ERROR) {
            std::cerr << "glGetError()) != GL_NO_ERROR-----TEXTURE------\n";
//            std::cerr << ()
        }
        else
        {
            std::cout << "successfully loaded texture buffer" << m_texture_id << "\n";
            m_dataLoaded = true;
        }
    }

}

GLuint Layer::getTexture()
{
    return m_texture_id;
}

