#ifndef RENDERER_H
#define RENDERER_H

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "mainshader.h"

using namespace std;

class Renderer
{
    public:
        Renderer();
        virtual ~Renderer();

        void init(unsigned int scr_width_in, unsigned int scr_height_in);
        void resizeWindow(int w, int h);
        void draw();

        void loadLayer(int layer, unsigned int dim, void* layer_data);
        void updateLayer(int layer, void* layer_data);
        void unloadLayer(int layer);

        Layer* getLayer(int layer_num) {return &m_layer[layer_num];};

        void setLayerScale(int layer, float min_val, float max_val);
        void maskLayerGreater(int layer, float threshold);
        void maskLayerLess(int layer, float threshold);

    protected:
    private:
        MainShader m_mainShader;

        unsigned int m_x;
        unsigned int m_y;

        std::vector<Layer> m_layer;
};

#endif // RENDERER_H
