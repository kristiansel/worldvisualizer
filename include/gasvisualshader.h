#ifndef GASVISUALSHADER_H
#define GASVISUALSHADER_H

#include "shaderbase.h"

class GasVisualShader : public ShaderBase
{
    public:
        GasVisualShader();
        virtual ~GasVisualShader();

        void init(unsigned int dim, float dl);
    protected:
    private:
};

#endif // GASVISUALSHADER_H
