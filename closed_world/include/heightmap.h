#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "closedmap.h"


class HeightMap
{
    public:
        HeightMap(unsigned int size_exponent);
        virtual ~HeightMap();

        enum Interpolation {Nearest, Linear, Quadratic};
        float sample(float x, float y, Interpolation interp = Nearest);
        float sampleI(unsigned int i, unsigned int j, Interpolation interp = Nearest);

    protected:
    private:
        float m_cellSize;
        ClosedMap<float> m_map_data;
};

#endif // HEIGHTMAP_H
