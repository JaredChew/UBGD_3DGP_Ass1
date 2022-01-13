#define WINDOW_WIDTH 2000.0
#define WINDOW_HEIGHT 1600.0

#define CIRCLULAR_WAVE_INTENSITY 30.0
#define CIRCLULAR_WAVE_ROTATE_SPEED_MUL 10.0
#define CIRCLULAR_WAVE_FREQUENCY_SMOOTHER 30.0 //50.0
#define CIRCLULAR_WAVE_START_POS_X WINDOW_WIDTH / 2.0
#define CIRCLULAR_WAVE_START_POS_Y WINDOW_HEIGHT * 0.35/// 2.0
#define CIRCLULAR_WAVE_RADIUS 0.0001 * (WINDOW_WIDTH * WINDOW_HEIGHT)
#define CIRCLULAR_WAVE_HOLLOW_RADIUS 0.7 * CIRCLULAR_WAVE_RADIUS

#define CIRCLE_AMOUNT 5
#define CIRCLE_RADIUS 100.0
#define CIRCLE_BASE_DISTANCE 270.0
#define CIRCLE_CENTER_X WINDOW_WIDTH / 2.0
#define CIRCLE_CENTER_Y WINDOW_HEIGHT * 0.45//WINDOW_HEIGHT / 2.0

#define WAVE_HEIGHT 10.0
#define WAVE_INTENSITY 30.0
#define WAVE_MOVE_SPEED_MUL 10.0
#define WAVE_FREQUENCY_SMOOTHER 30.0 //50.0
#define WAVE_START_POS_Y WINDOW_HEIGHT * 0.1
//#define WAVE_START_POS_X WINDOW_WIDTH / 2.0

#define INVERT_START_X WINDOW_WIDTH / 2.0
#define INVERT_START_Y WINDOW_HEIGHT / 2.0

#define BACKGROUND_EFFECT_SHARPNESS 0.07

precision mediump float;

varying vec2 fTexCoord;

uniform sampler2D sampler2d;

uniform float averageAudioBeat;
uniform float averageBaseBeat;
uniform float averageHighBeat;
uniform float averageMidBeat;

uniform float circleRotation;

uniform float incrementingVariable;

float degreeToRadian(float degreeAngle) {

	return degreeAngle * (3.141592654 / 180.0);

}

float roundOffZerosInFloatDecimal(float toRound) {

	while (toRound < 0.09) {

		toRound *= 10.0;

	}

	return toRound;

}

vec4 interpolateColour(vec4 min, vec4 max, float interpolate) {

	return vec4(

		mix( min.r, max.r, interpolate ),
		mix( min.g, max.g, interpolate ),
		mix( min.b, max.b, interpolate ),
		1.0

	);

}

bool wave(inout vec4 toMix, vec4 colour) {

	//float wavePositionY = (WINDOW_HEIGHT / 2.0) + (sin((gl_FragCoord.x + (incrementingVariable * 3.0)) / WAVE_FREQUENCY_SMOOTHER) * sin(incrementingVariable * 1.0) * CIRCLULAR_WAVE_INTENSITY); //original

	float waveFrequency = sin((gl_FragCoord.x + (incrementingVariable * WAVE_MOVE_SPEED_MUL)) / WAVE_FREQUENCY_SMOOTHER); //default
	//float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * WAVE_MOVE_SPEED_MUL)) / averageAudioBeat); //combine (causes very high frequencies)
	//float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * WAVE_MOVE_SPEED_MUL)) / (averageMidBeat * 100.0) ); //mid beat

	// ** Add abs() to distort the circle more ** //
	//float waveLength = sin(incrementingVariable * 1.0) * WAVE_INTENSITY; //default
	//float waveLength = sin(gl_FragCoord.x + (incrementingVariable * 0.5)) * WAVE_INTENSITY; //helix
	float waveLength = sin( gl_FragCoord.x / averageBaseBeat ) * ( averageHighBeat * 3000.0 ); //combine 1
	//float waveLength = sin( gl_FragCoord.x / (averageBaseBeat * 3000.0) ) * ( averageHighBeat * 5000.0 ); //combine 2

	float wavePositionY = WAVE_START_POS_Y + (waveFrequency * waveLength);

	float distanceWave = sqrt(( (gl_FragCoord.x - gl_FragCoord.x) * (gl_FragCoord.x - gl_FragCoord.x) ) + ( (wavePositionY - gl_FragCoord.y) * (wavePositionY - gl_FragCoord.y) )); //default

	if(distanceWave < WAVE_HEIGHT) {

		toMix = colour;

		return true;
		
	}
		
	return false;

}

bool circlularWave(inout vec4 toMix) { //vec4 colour

	vec4 colour = vec4(

		mix( 0.3, 1.0, 1.0 - ( (1.0 / WINDOW_HEIGHT) * gl_FragCoord.y) ),
		mix( 0.5, 1.0, 1.0 - ( (1.0 / WINDOW_HEIGHT) * gl_FragCoord.y) ),
		mix( 0.7, 1.0, 1.0 - ( (1.0 / WINDOW_HEIGHT) * gl_FragCoord.y) ),
		1.0

	);

	// !! CIRCLE !! //

	float distanceCircle = sqrt( pow(CIRCLE_CENTER_X - gl_FragCoord.x, 2.0) + pow(CIRCLE_CENTER_Y - gl_FragCoord.y, 2.0) );

	float circleSizeIncrease = averageBaseBeat * 500.0; //1500.0

	// !! WAVE !! //

	//float wavePositionY = (WINDOW_HEIGHT / 2.0) + (sin((gl_FragCoord.x + (incrementingVariable * 3.0)) / CIRCLULAR_WAVE_FREQUENCY_SMOOTHER) * sin(incrementingVariable * 1.0) * CIRCLULAR_WAVE_INTENSITY); //original

	float waveFrequency = sin((gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / CIRCLULAR_WAVE_FREQUENCY_SMOOTHER); //default
	//float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / averageAudioBeat); //combine (causes very high frequencies)
	//float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / (averageMidBeat * 100.0) ); //mid beat

	// ** Add abs() to distort the circle more ** //
	//float waveLength = sin(incrementingVariable * 1.0) * CIRCLULAR_WAVE_INTENSITY; //default
	//float waveLength = sin(gl_FragCoord.x + (incrementingVariable * 0.5)) * CIRCLULAR_WAVE_INTENSITY; //helix
	//float waveLength = sin( gl_FragCoord.x / averageBaseBeat ) * ( averageHighBeat * 3000.0 ); //combine 1
	float waveLength = sin( gl_FragCoord.x / (averageBaseBeat * 3000.0) ) * ( averageHighBeat * 5000.0 ); //combine 2

	float wavePositionY = CIRCLULAR_WAVE_START_POS_Y + (waveFrequency * waveLength);
	//float wavePositionX = CIRCLULAR_WAVE_START_POS_X + (waveFrequency * waveLength);

	//float distanceWave = sqrt(( (gl_FragCoord.x - gl_FragCoord.x) * (gl_FragCoord.x - gl_FragCoord.x) ) + ( (wavePositionY - gl_FragCoord.y) * (wavePositionY - gl_FragCoord.y) )); //default
	//float distanceWave = sqrt( pow(wavePositionX - gl_FragCoord.x, 2.0) + pow(wavePositionY - gl_FragCoord.y, 2.0)); //XY
	float distanceWave = sqrt( pow(CIRCLULAR_WAVE_START_POS_X - gl_FragCoord.x, 2.0) + pow(wavePositionY - gl_FragCoord.y, 2.0)); //combine

	if(distanceWave < (CIRCLULAR_WAVE_RADIUS + circleSizeIncrease) && distanceWave > (CIRCLULAR_WAVE_HOLLOW_RADIUS + circleSizeIncrease)) {

		toMix = colour;

		return true;
		
	}
		
	return false;

}

void invertedCustomEffect(inout vec4 toEffect) {

	//float waveFrequency = sin((gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / CIRCLULAR_WAVE_FREQUENCY_SMOOTHER); //default
	float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / averageAudioBeat); //combine (causes very high frequencies)
	//float waveFrequency = sin( (gl_FragCoord.x + (incrementingVariable * CIRCLULAR_WAVE_ROTATE_SPEED_MUL)) / (averageMidBeat * 100.0) ); //mid beat

	// ** Add abs() to distort the circle more ** //
	//float waveLength = sin(incrementingVariable * 1.0) * CIRCLULAR_WAVE_INTENSITY; //default
	//float waveLength = sin(gl_FragCoord.x + (incrementingVariable * 0.5)) * CIRCLULAR_WAVE_INTENSITY; //helix
	float waveLength = sin( averageBaseBeat * 1000.0 ) * ( averageHighBeat * 1000.0 ); //combine 1
	//float waveLength = sin( gl_FragCoord.x / (averageBaseBeat * 3000.0) ) * ( averageHighBeat * 5000.0 ); //combine 2

	float wavePositionY = (WINDOW_HEIGHT  * 0.45) + (waveFrequency * waveLength);
	
	float distanceWave = sqrt( pow((CIRCLULAR_WAVE_START_POS_X) - gl_FragCoord.x, 2.0) + pow(wavePositionY - gl_FragCoord.y, 2.0)); //combine

	if(distanceWave < WAVE_HEIGHT * (2.0)) {

		toEffect = vec4(1.0 - toEffect.r, 1.0 - toEffect.g, 1.0 - toEffect.b, 1.0);
		
	}

}

void invertedCircleEffect(inout vec4 toEffect) {

	float distanceCircle = sqrt( pow(INVERT_START_X - gl_FragCoord.x, 2.0) + pow(INVERT_START_Y - gl_FragCoord.y, 2.0) );

	if(distanceCircle <= CIRCLE_RADIUS * (1.0)) {

		//toEffect = vec4(1.0 - toEffect.r, 1.0 - toEffect.g, 1.0 - toEffect.b, 1.0);
		
		float grayScale = (toEffect.r + toEffect.g + toEffect.b) / 3.0;

		toEffect.r = grayScale;
		toEffect.g = grayScale;
		toEffect.b = grayScale;

	}

}

void grayscaleMovingCircleEffect(int numOfCircles, inout vec4 toEffect) {

	for(int i = 1; i <= numOfCircles; ++i) {

		float xPos = ( (CIRCLE_BASE_DISTANCE * (averageAudioBeat * 5.0) ) * sin( degreeToRadian( circleRotation + float( (360 / numOfCircles) * i ) ) ) ) + CIRCLE_CENTER_X;
		float yPos = ( (CIRCLE_BASE_DISTANCE * (averageAudioBeat * 5.0) ) * cos( degreeToRadian( circleRotation + float( (360 / numOfCircles) * i ) ) ) ) + CIRCLE_CENTER_Y;

		float distanceCircle = sqrt( pow(xPos - gl_FragCoord.x, 2.0) + pow(yPos - gl_FragCoord.y, 2.0) );

		if(distanceCircle <= CIRCLE_RADIUS * (1.0)) {

			toEffect = vec4(1.0 - toEffect.r, 1.0 - toEffect.g, 1.0 - toEffect.b, 1.0);
			/*
			float grayScale = (toEffect.r + toEffect.g + toEffect.b) / 3.0;

			toEffect.r = grayScale;
			toEffect.g = grayScale;
			toEffect.b = grayScale;
			*/
			return;

		}

	}

}

void backgroundMovingCheckerEffect(inout vec4 toBlend, float speecMultiplier) {

	vec4 effect;

	effect.r += abs( sin( ( gl_FragCoord.x + (incrementingVariable * speecMultiplier) ) * BACKGROUND_EFFECT_SHARPNESS) );
	effect.g += abs( sin( ( gl_FragCoord.y + (incrementingVariable * speecMultiplier) ) * BACKGROUND_EFFECT_SHARPNESS) );
	effect.b += abs( sin( (  (gl_FragCoord.x + gl_FragCoord.y) + (incrementingVariable * speecMultiplier) ) * BACKGROUND_EFFECT_SHARPNESS) );
	
	if(roundOffZerosInFloatDecimal(averageAudioBeat) >= 0.5) {
		effect.r += abs( sin(gl_FragCoord.y) );
		effect.g += abs( sin(gl_FragCoord.x) );
		effect.b += abs( sin(gl_FragCoord.x + gl_FragCoord.y) );
	}

	toBlend += effect;

}

void main() {

	vec4 toMix;
	vec4 resultColour;
	vec4 texColor = texture2D(sampler2d, fTexCoord);
	//vec4 backgroundColour = vec4(0.059, 1.0, 0.867, 1.0);

	resultColour = texColor;

	//backgroundMovingCheckerEffect(resultColour, 2.0);

	//innterpolateColour(vec3(0.3, 0.5, 0.7), vec3(1.0, 1.0, 1.0), (1.0 / WINDOW_HEIGHT) * gl_FragCoord.y);
	
	if(circlularWave(toMix)) { resultColour = toMix; }
	else if(wave(toMix, vec4(0.3, 0.5, 0.7, 1.0))) { resultColour += toMix; }
	
	//invertedCircleEffect(resultColour);
	invertedCustomEffect(resultColour);

	grayscaleMovingCircleEffect(CIRCLE_AMOUNT, resultColour); //int(averageAudioBeat * 10.0)

	gl_FragColor = resultColour;

}