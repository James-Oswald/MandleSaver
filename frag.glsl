#version 330 core

precision highp float;

in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec2 WindowSize;
uniform float time;


int maxIterations = 100;


vec3 palette(float t){
	vec3 c1 = vec3(0, 7, 100);
	vec3 c2 = vec3(2, 107, 203);
	vec3 c3 = vec3(237, 255, 255);
	vec3 c4 = vec3(255, 170, 0);
	float x = 1.0 / 3.0;
	if (t < x) return mix(c1, c2, t/x);
	else if (t < 2.0 * x) return mix(c2, c3, (t - x)/x);
	else if (t < 3.0 * x) return mix(c3, c4, (t - 2.0*x)/x);
	return c4;
}

vec2 squareImaginary(vec2 number){
	return vec2(
		pow(number.x,2)-pow(number.y,2),
		2*number.x*number.y
	);
}

float hypot(vec2 z) {
  float x = abs(z.x);
  float y = abs(z.y);
  float t = min(x, y);
  x = max(x, y);
  t = t / x;
  //return x * sqrt(1.0 + t * t);
  return (z.x == 0.0 && z.y == 0.0) ? 0.0 : x * sqrt(1.0 + t * t);
}

vec2 cpow(vec2 z, float x){
	float r = hypot(z);
	float theta = atan(z.y, z.x) * x;
	return vec2(cos(theta), sin(theta)) * pow(r, x);
}

vec3 iterateMandelbrot(vec2 coord){
	vec2 z = vec2(0, 0);
	for(int i = 0; i < maxIterations; i++){
		z = cpow(z, 10 * sin(0.5 * time) + 10) + coord;
		if(length(z) > 2) 
            return palette(float(i)/float(maxIterations));
	}
	return vec3(0, 0, 0);
}

void main() {
    vec2 pos = mat2(3.5/WindowSize.x, 0, 0, 2/WindowSize.y) * gl_FragCoord.xy + vec2(sin(time) + -2.5, cos(time)-1);
    FragColor = vec4(iterateMandelbrot(pos), 1.0);
}
