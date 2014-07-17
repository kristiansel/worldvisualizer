# version 330 core

out vec4 frag_color;
in vec2 texcoord;

uniform sampler2D layerTexture;

uniform float max_val;
uniform float min_val;

uniform float cut_below;
uniform float cut_above;

uniform float alpha;

uniform vec3 color_transf;

uniform vec4 high_color = vec4(1.0, 0.5, 0.0, 1.0);
uniform vec4 low_color = vec4(0.0, 0.5, 1.0, 1.0);

void main()
{
    float val = texture(layerTexture, texcoord).r;

    vec4 color;


    float mapped_val = (val>max_val) ? max_val : ((val<min_val) ? min_val : (val-min_val)/(max_val-min_val));
    color.r = 0.5*(1.0-cos(3.14*mapped_val));
    color.b = 0.5*(1.0+cos(3.14*mapped_val));
    color.g = 0.5*(1.0-cos(2*3.14*mapped_val));
    color.a = 1.0;

    color = vec4(color_transf*color.xyz, alpha);

//    color.a = 1.0 - abs((color.r+color.g+color.b)-(1.5-0.5*cos(2*3.14*mapped_val)));

//    if (val>max_val)
//    {
//        color = high_color;
//    }
//    else if (val<min_val)
//    {
//        color = low_color;
//    }
//    else
//    {
//        float wt = (val-min_val)/(max_val-min_val);
//        color = high_color*wt + (1.f-wt)*low_color;
//    }

    if (val>cut_above || val<cut_below)
    {
        color.a = 0.0;
    }



    frag_color = color;
}
