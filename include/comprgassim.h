#ifndef COMPRGASSIM_H
#define COMPRGASSIM_H

#include "gasjacobishader.h"
#include "gasvisualshader.h"

class ComprGasSim
{
    public:
        ComprGasSim(unsigned int dim, float* height_data, unsigned int x_res, unsigned int y_res);
        virtual ~ComprGasSim();

        void step(float dt);

        //ClosedMap<float>* getResult() {return &m_result;};
    protected:
        void valBox(float x_min, float x_max, float y_min, float y_max,
                    int i, int j, int dim,
                    float* field, float val_in_box[4]);
    private:
        // Advection/Diffusion shader
        GasJacobiShader m_gasJacobiShader;

        // Display shader
        GasVisualShader m_gasVisualShader;

        struct Param
        {
            float viscosity;
        } m_params;

        struct SimParams
        {
            float dl;
        } m_simParams;

        unsigned int m_timeStep;
        float m_time;

        unsigned int m_dim;
        unsigned int m_xres;
        unsigned int m_yres;

};

#endif // COMPRGASSIM_H
