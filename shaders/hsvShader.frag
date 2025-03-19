#version 330 core
in vec2 TexCoords;   // Input texture coordinates from vertex shader
out vec4 FragColor;   // Final output color

uniform sampler2D image;
uniform float hueAdjust;
uniform float satAdjust;
uniform float valAdjust;

vec3 RGBtoHSV(vec3 c) {
    float cMax = max(c.r, max(c.g, c.b));
    float cMin = min(c.r, min(c.g, c.b));
    float delta = cMax - cMin;

    float h = 0.0;
    if (delta > 0.00001) {
        if (cMax == c.r) {
            h = mod((c.g - c.b) / delta, 6.0);
        } else if (cMax == c.g) {
            h = ((c.b - c.r) / delta) + 2.0;
        } else {
            h = ((c.r - c.g) / delta) + 4.0;
        }
        h *= 60.0; // Convert to degrees
    }

    float s = (cMax > 0.0) ? (delta / cMax) : 0.0;
    float v = cMax;

    return vec3(h, s, v);
}


vec3 HSVtoRGB(vec3 hsv) {
    float h = hsv.x / 60.0;
    float s = hsv.y;
    float v = hsv.z;

    int i = int(floor(h)) % 6;
    float f = h - floor(h);

    float p = v * (1.0 - s);
    float q = v * (1.0 - f * s);
    float t = v * (1.0 - (1.0 - f) * s);

    if (i == 0) return vec3(v, t, p);
    if (i == 1) return vec3(q, v, p);
    if (i == 2) return vec3(p, v, t);
    if (i == 3) return vec3(p, q, v);
    if (i == 4) return vec3(t, p, v);
    return vec3(v, p, q);
}

void main(void)
{

    vec3 color = texture(image, TexCoords).rgb;

    vec3 hsv = RGBtoHSV(color);

    // Adjust Hue, Saturation, and Value
    hsv.x = mod(hsv.x + hueAdjust, 360.0); // Wrap hue around 360 degrees
    hsv.y = clamp(hsv.y * satAdjust, 0.0, 2.0); // Saturation multiplier
    hsv.z = clamp(hsv.z * valAdjust, 0.0, 2.0); // Value multiplier

    // Convert back to RGB
    vec3 result = HSVtoRGB(hsv);
    FragColor = vec4(result, 1.0);
}
