#ifndef LAYER_H
#define LAYER_H

#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"


class Layer
{
    public:
        Layer();
        virtual ~Layer();

        void setMax(float in);
        void setMin(float in);
        void setCutBelow(float in);
        void setCutAbove(float in);

        float getMax();
        float getMin();
        float getCutBelow();
        float getCutAbove();

        void setColorTransf(float* rgb);
        void getColorTransf(float* col);

        void setAlpha(float alpha) {m_alpha = alpha;};
        float getAlpha() {return m_alpha;};



        bool isActive();

        GLuint getTexture();

        void init(unsigned int dimensions);
        void loadLayerData(unsigned int dim, void* layerData, bool packTo0_1range = false);
        void updateLayerData(void* layerData);

    protected:
    private:
        float m_maxVal;
        float m_minVal;
        float m_cutBelowVal;
        float m_cutAboveVal;
        float m_alpha;

        float m_col[3];

        bool m_active;

        unsigned int m_dimensions;

        bool m_initialized;
        bool m_dataLoaded;

        GLuint m_texture_id;
};

#endif // LAYER_H
