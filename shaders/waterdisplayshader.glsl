# version 330 core

out vec4 x_out; // (u, v, h)
in vec2 texcoord;

uniform sampler2D x_field; // (u, v, h)
uniform sampler2D height; // terrain

void main()
{
    vec4 field = texture(x_field, texcoord);
    float height_sample = texture(height, texcoord).r;

    float h = field.b;

    vec4 col_out;

    vec4 terr_col = col_out = vec4(0.0, (height_sample+5000.f)/10000.f, 0.0, 1.0);

    float froth = sqrt((field.r*field.r + field.g*field.g)/6400.f);
    float ocean_weight = min(max(froth, field.b/500.f), 1.f);



    vec4 ocean_col = vec4(froth, froth, 1.f, 1.0);

    col_out = ocean_weight*ocean_col + (1.f-ocean_weight)*terr_col;

    //col_out = vec4(0.0, 0.0, (height_sample+h)/20.f, 1.0);

    //col_out = field;
    col_out = vec4(field.r/40.f, field.g/40.f, field.b/500.f, 1.0);
    x_out = col_out;
}
