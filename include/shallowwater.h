#ifndef SHALLOWWATER_H
#define SHALLOWWATER_H

//#define SHALLOW_EXPLICIT
#define SHALLOW_IMPLICIT

#ifdef SHALLOW_EXPLICIT
#include "shallowwatershader.h"
#endif

#ifdef SHALLOW_IMPLICIT
#include "shallowimplicitshader.h"
#endif

#include "waterdisplayshader.h"



class ShallowWater
{
    public:
        ShallowWater(unsigned int dim, float* height_data, unsigned int x_res, unsigned int y_res);
        virtual ~ShallowWater();

        void step(float dt);

        void stirWater(float rel_x, float rel_y);

    protected:
        void valBox(float x_min, float x_max, float y_min, float y_max,
                    int i, int j, int dim,
                    float* field, float val_in_box[4]);
    private:
#ifdef SHALLOW_EXPLICIT
        // Advection/Diffusion shader
        ShallowWaterShader m_shallowWaterShader;
#endif

#ifdef SHALLOW_IMPLICIT
        // Advection/Diffusion shader
        ShallowImplicitShader m_shallowWaterShader;
#endif

//        // Display shader
        WaterDisplayShader m_waterDisplayShader;

        struct Param
        {
            float gravity;
        } m_params;

        struct SimParams
        {
            float dl;
        } m_simParams;

        unsigned int m_timeStep;
        float m_time;
        int last_kicked;

        unsigned int m_dim;
        unsigned int m_xres;
        unsigned int m_yres;

};

#endif // SHALLOWWATER_H
