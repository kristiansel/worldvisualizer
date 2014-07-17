#ifndef WEATHER_H
#define WEATHER_H

#include "closedmap.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

class Weather
{
    public:
        struct Field
        {
            ClosedMap<float> v_x;
            ClosedMap<float> v_y;
            ClosedMap<float> p;
        };

        Weather(ClosedMap<float>& height_map, unsigned int dim);
        virtual ~Weather();

        void step();

        ClosedMap<float>* getVx() {return &m_previous->v_x;};
        ClosedMap<float>* getVy() {return &m_previous->v_y;};
        ClosedMap<float>* getP() {return &m_previous->p;};
        ClosedMap<float>* getDiv() {return &m_calc.div;};
        ClosedMap<float>* getConc() {return &m_transp.conc;};

        Field* field() {return &m_field;};
    protected:
    public:
        // private:
        Weather();
        unsigned int m_size;

        // Field variables
        Field m_field;
        Field m_temp;

        Field* m_previous;
        Field* m_next;

        struct Calc
        {
            ClosedMap<float> div;
            float max_div;
        } m_calc;

        struct Transported
        {
            ClosedMap<float> conc;
        } m_transp;


        struct Param
        {
            ClosedMap<float>* h_g;
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

        float d_dx(ClosedMap<float>& var, unsigned int i, unsigned int j);
        float d_dy(ClosedMap<float>& var, unsigned int i, unsigned int j);

        float d2_dx2(ClosedMap<float>& var, unsigned int i, unsigned int j);
        float d2_dy2(ClosedMap<float>& var, unsigned int i, unsigned int j);



};

#endif // WEATHER_H
