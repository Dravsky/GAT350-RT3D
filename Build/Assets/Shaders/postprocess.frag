#version 430

#define INVERT_MASK		(1 << 0)
#define GRAYSCALE_MASK	(1 << 1)
#define COLORTINT_MASK	(1 << 2)
#define GRAIN_MASK		(1 << 3)
#define SCANLINE_MASK	(1 << 4)
#define RAVE_MASK		(1 << 5)

in  layout(location = 1) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tint;
uniform float time = 0;
uniform float graininess;
uniform float scanlinewidth;
uniform float scanlineintensity;

layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3( 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b ), color.a );
}

vec4 colortint(in vec4 color)
{
	return vec4(tint, 1);
}

vec4 grain(in vec4 color)
{
    float randR = sin(dot(gl_FragCoord.xy * time, vec2(21, 42)));
    float randG = sin(dot(gl_FragCoord.xy * time, vec2(77, 66)));
    float randB = sin(dot(gl_FragCoord.xy * time, vec2(69, 420)));

    color.rgb += vec3(randR, randG, randB) * graininess;

    return color;
}

vec4 scanline(in vec4 color)
{
    float scanline = mod((ftexcoord.y + (time * 0.5) ) * scanlinewidth, 1.0);
    scanline = smoothstep(1.0 - scanlineintensity, 1.0, scanline) * 0.5 + 0.5;

    return color * scanline;
}

vec4 rave(in vec4 color)
{
	return vec4(color.r - sin(time * 2), color.g - sin(time * 3), color.b - sin(time * 4), 1);
}

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))		postprocess = invert(basecolor);
	if (bool(params & GRAYSCALE_MASK))	postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK))	postprocess = colortint(postprocess);
	if (bool(params & GRAIN_MASK))		postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK))	postprocess = scanline(postprocess);
	if (bool(params & RAVE_MASK))		postprocess = rave(postprocess);

	ocolor = mix(basecolor, postprocess, blend);
}