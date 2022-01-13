precision mediump float;

uniform float animationTest;
uniform float circleRadius;

uniform float circleCenterX;
uniform float circleCenterY;

uniform float width;
uniform float height;

uniform float wave;
uniform float waveSpeed;

varying vec4 fColor;
varying vec2 fTexCoord;

uniform sampler2D sampler2d;
uniform float Factor1;

void main() {

	float screenPosition = gl_FragCoord.x;

	float lengthOfColour = 35.0;
	float colourFromLength = 0.5 / lengthOfColour;

	float sharpness = 0.009; //0.01 //0.1 //999.0
	
	float distanceCircle = sqrt(( (circleCenterX - gl_FragCoord.x) * (circleCenterX - gl_FragCoord.x) ) + ( (circleCenterY - gl_FragCoord.y) * (circleCenterY - gl_FragCoord.y) ));

	float waveRadius = 70.0;

	float wavePositionY = (height / 2.0) + (sin((gl_FragCoord.x + (waveSpeed * 3.0)) / 50.0) * sin(waveSpeed * 0.10) * 130.0);
	//float wavePositionY = (height / 2.0) + (sin((gl_FragCoord.x + waveSpeed) / 50.0) * 130.0);
	//float wavePositionY = (WINDOW_HEIGHT / 2.0) + (sin((gl_FragCoord.x + (gl_FragCoord.x * 3.0)) / 50.0) * sin(gl_FragCoord.x * 0.10) * 130.0);

	//float wavePositionX = (width / 2.0) + sin(waveSpeed * 0.1) * 500.0;
	//float wavePositionX = sin(waveSpeed * 2.0) + gl_FragCoord.x;

	float distanceWave = sqrt(( (gl_FragCoord.x - gl_FragCoord.x) * (gl_FragCoord.x - gl_FragCoord.x) ) + ( (wavePositionY - gl_FragCoord.y) * (wavePositionY - gl_FragCoord.y) ));
	//float distanceWave = sqrt(( (wavePositionX - gl_FragCoord.x) * (wavePositionX - gl_FragCoord.x) ) + ( (wavePositionY - gl_FragCoord.y) * (wavePositionY - gl_FragCoord.y) ));

	vec4 texColor = texture2D(sampler2d, fTexCoord);
	vec4 combinedColor = fColor * texColor;

	float grayCombined = (texColor.r + texColor.g + texColor.b) / 3.0;

	vec4 resultColor;

/*
	//Pattern
	float r = abs(cos(gl_FragCoord.x * 0.1));
	float g = abs(cos(gl_FragCoord.y * 0.1));
	float b = abs(sin(gl_FragCoord.x + gl_FragCoord.y));
*/
/*
	//Half & half
	float r = abs(gl_FragCoord.x > 400.0 ? 0.25 : 0.75);
	float g = abs(gl_FragCoord.x > 400.0 ? 0.25 : 0.75);
	float b = abs(gl_FragCoord.x > 400.0 ? 0.25 : 0.75);
*/
/*
	//Trippy
	float r = abs(sin(gl_FragCoord.x * gl_FragCoord.y));
	float g = abs(sin(gl_FragCoord.x * gl_FragCoord.y));
	float b = abs(sin(gl_FragCoord.x * gl_FragCoord.y));
*/
/*
	//Trippy with colour
	float r = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 0.0));
	float g = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 1.0));
	float b = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 2.0));
*/
/*
	//Soft rainbow
	float r = abs(sin(screenPosition * sharpness + 0.0));
	float g = abs(sin(screenPosition * sharpness + 1.0));
	float b = abs(sin(screenPosition * sharpness + 2.0));
*/
/*
	Trippy with soft colour
	float r = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 0.0));
	float g = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 1.0));
	float b = abs(sin(gl_FragCoord.x * gl_FragCoord.y + 2.0));
*/
/*	
	//Trippy like stars
	float r = (sin( ( (gl_FragCoord.x * gl_FragCoord.y) * colourFromLength) + 0.0) / 2.0);
	float g = (sin( ( (gl_FragCoord.x * gl_FragCoord.y) * colourFromLength) + 1.0) / 2.0);
	float b = (sin( ( (gl_FragCoord.x * gl_FragCoord.y) * colourFromLength) + 2.0) / 2.0);
*/
/*
	//Lecturer
	float r = cos ( screenPosition * 0.03 + 0.0) * 0.5 + 0.5;
	float g = cos ( screenPosition * 0.03 + 2.0) * 0.5 + 0.5;
	float b = cos ( screenPosition * 0.03 + 4.0) * 0.5 + 0.5;
*/
/*
	//StaticRainbow
	float r = (cos ( (screenPosition * colourFromLength) + 0.0) / 2.0) + 0.5;
	float g = (cos ( (screenPosition * colourFromLength) + 4.0) / 2.0) + 0.5;
	float b = (cos ( (screenPosition * colourFromLength) + 8.0) / 2.0) + 0.5;
*/	
/*
	//Moving Rainbow
	float r = (cos ( ( (screenPosition * colourFromLength) + animationTest ) + 0.0) / 2.0) + 0.5;
	float g = (cos ( ( (screenPosition * colourFromLength) + animationTest ) + 4.0) / 2.0) + 0.5;
	float b = (cos ( ( (screenPosition * colourFromLength) + animationTest ) + 8.0) / 2.0) + 0.5;
*/
/*
	//Trippy with moving soft colour
	float r = abs(sin(gl_FragCoord.x * gl_FragCoord.y * animationTest + 0.0));
	float g = abs(sin(gl_FragCoord.x * gl_FragCoord.y * animationTest + 1.0));
	float b = abs(sin(gl_FragCoord.x * gl_FragCoord.y * animationTest + 2.));
*/
/*
	//Circle
	float r = distanceCircle <= circleRadius ? 0.3 : 1.0;
	float g = distanceCircle <= circleRadius ? 0.5 : 1.0;
	float b = distanceCircle <= circleRadius ? 0.7 : 1.0;
*/
/*
	//Grayscale
	float r = grayCombined; // + Factor1
	float g = grayCombined; // + Factor1
	float b = grayCombined; // + Factor1
*/
/*
	//Wave
	float r = distanceWave <= waveRadius ? 0.3 : 1.0;
	float g = distanceWave <= waveRadius ? 0.5 : 1.0;
	float b = distanceWave <= waveRadius ? 0.7 : 1.0;
*/
/*
	//Moving wave
	float r = mix(0.0, 1.0, 1.0 - ((1.0 / waveRadius) * distanceWave)) + (texColor.r);
	float g = mix(0.0, 1.0, 1.0 - ((1.0 / waveRadius) * distanceWave)) + (texColor.g);
	float b = mix(1.0, 1.0, 1.0 - ((1.0 / waveRadius) * distanceWave)) + (texColor.b);
*/

	resultColor.r = r;
	resultColor.g = g;
	resultColor.b = b;

	gl_FragColor = resultColor;

	//Use texture color only
	//gl_FragColor = texture2D(sampler2d, fTexCoord);

}