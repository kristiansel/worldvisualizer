#ifndef CLOSEDWORLD_H
#define CLOSEDWORLD_H

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "closedmap.h"
#include "weathermap.h"


class ClosedWorld
{
    public:
        ClosedWorld(unsigned int size_exponent);
        virtual ~ClosedWorld();

        unsigned int getDimension();


        ClosedMap<float>* getHeightMap() {return &m_height_map;};
        float* getHeightData() {return m_height_map.getDataPtr();};
        float* getRadiationData(float t);

        WeatherMap m_weatherMap;
        ClosedMap<float> m_height_map;

        // calculated
        enum Coords {Absolute, Normalized};
        float IncidentRadiation(float x, float y, float t, Coords coords = Normalized);

        void stepWorld(); // step one day
        void getYearMonthDay(int &year, int &month, int &day);

    protected:
    private:
        // actual cell
        unsigned int m_size_exponent;
        unsigned int m_dimension;
        unsigned int m_seed;
        float m_cellSize;



        const int daysInYear = 360;
        const int daysInMonth = 30;

        int m_dayNum;

//        ClosedMap<float> m_height_map;

};

#endif // CLOSEDWORLD_H
