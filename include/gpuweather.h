#ifndef GPUWEATHER_H
#define GPUWEATHER_H

#include "closedmap.h"

#include "advdiffshader.h"
#include "pressureshader.h"
#include "velcorrshader.h"
#include "fluiddisplayshader.h"

class gpuWeather
{
    public:
        gpuWeather(unsigned int dim);
        virtual ~gpuWeather();

        void step(float dt);

        ClosedMap<float>* getResult() {return &m_result;};
    protected:
    private:
        // Advection/Diffusion shader
        AdvDiffShader m_advdiffShader;

        // Pressure shader
        PressureShader m_pressShader;

        // Velocity correction shader
        VelCorrShader m_velCorrShader;


        // Display shader
        FluidDisplayShader m_fluidDispShader;

        ClosedMap<float> m_result;

        struct Param
        {
            ClosedMap<float> S_vx;
            ClosedMap<float> S_vy;
            float density;
            float viscosity;
        } m_params;

        struct SimParams
        {
            float dl;
            float dt;
        } m_simParams;

        unsigned int m_timeStep;
        float m_time;

};

#endif // GPUWEATHER_H
