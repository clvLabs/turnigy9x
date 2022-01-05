// ***********************************************************************************************
// ***
// ***	 Turnigy9XChannel
// ***	------------------
// ***
// ***	Turnigy9X RC radio receiver manager class
// ***

// ***********************************************************************************************
// ***
// *** Includes
// ***

#include "Turnigy9XChannel.h"


// ***********************************************************************************************
// ***
// *** CTurnigy9XChannel
// ***
// *** Channel class
// ***


// ////////////////////////////////////////////
//
// Constructor
//
CTurnigy9XChannel::CTurnigy9XChannel()
{
	reset();
}

// ////////////////////////////////////////////
//
// Channel attach / detach
//
void CTurnigy9XChannel::attach( int piPin )
{
	miPin = piPin;
	pinMode( miPin, INPUT );
}

void CTurnigy9XChannel::detach()
{
	reset();
}

// ////////////////////////////////////////////
//
// Channel data access
//
bool CTurnigy9XChannel::isActive()					{	return( mbActive );						}
bool CTurnigy9XChannel::isCalibrating()				{	return( mbCalibrating );				}
bool CTurnigy9XChannel::dataChanged()				{	return( mbDataChanged );				}

bool CTurnigy9XChannel::isUp()						{	return( miCurrentMappedValue > miCenterMappingValue );	}
bool CTurnigy9XChannel::isDown()					{	return( miCurrentMappedValue < miCenterMappingValue );	}
bool CTurnigy9XChannel::isTop()						{	return( miCurrentMappedValue == miMaxMappingValue );	}
bool CTurnigy9XChannel::isBottom()					{	return( miCurrentMappedValue == miMinMappingValue );	}
bool CTurnigy9XChannel::isCentered()				{	return( miCurrentMappedValue == miCenterMappingValue );	}

float CTurnigy9XChannel::getLastValue()				{	return( miLastMappedValue );			}
float CTurnigy9XChannel::getValue()					{	return( miCurrentMappedValue );			}
float CTurnigy9XChannel::getPulseLength()			{	return( miCurrentPulseLength );			}
float CTurnigy9XChannel::getLastPulseLength()		{	return( miLastUsedPulseLength );		}
float CTurnigy9XChannel::getPreviousPulseLength()	{	return( miPreviousUsedPulseLength );	}


// ////////////////////////////////////////////
//
// Calibration
//
void CTurnigy9XChannel::calibrate( unsigned long piTime )
{
	if( !isActive() )
		return;

	miCalibrationProgrammedTime = piTime;
	miCalibrationStartTime = millis();

	setPulseLengthRange( DEFAULT_MAX_PULSE_LENGTH, DEFAULT_MIN_PULSE_LENGTH );

	mbCalibrating = true;
}

void CTurnigy9XChannel::startCalibration()
{
	// Start calibrating with no start time
	calibrate( TIME_NOT_SET );
}

void CTurnigy9XChannel::stopCalibration()
{
	// Reset control variables
	mbCalibrating = false;
	miCalibrationProgrammedTime = TIME_NOT_SET;
	miCalibrationStartTime = TIME_NOT_SET;

	// If control didn't move during the calibration,
	//	its current range (delta) shoud be in the range 0-( miDebounceTreshold * 5 ).
	// In that case, we'll reset to the default pulse length range
	//	( better than an uncalibrated channel!! )
	if( abs( miMaxPulseLength - miMinPulseLength ) <= 100 ) // ( miDebounceTreshold * 5 ) )
		setPulseLengthRange( DEFAULT_MIN_PULSE_LENGTH, DEFAULT_MAX_PULSE_LENGTH );
}


// ////////////////////////////////////////////
//
// Treshold settings
//
void CTurnigy9XChannel::setDebounceTreshold( unsigned int piTreshold )		{	miDebounceTreshold = piTreshold;	}
void CTurnigy9XChannel::setMidpointTolerance( unsigned int piTolerance )	{	miMidPointTolerance = piTolerance;	}
void CTurnigy9XChannel::setPulseReadTimeout( unsigned long piTimeout )		{	miPulseReadTimeout = piTimeout;		}
void CTurnigy9XChannel::setEmitterStartupTime( unsigned long piTime )		{	miEmitterStartupTime = piTime;		}

unsigned int CTurnigy9XChannel::getDebounceTreshold()						{	return( miDebounceTreshold );		}
unsigned int CTurnigy9XChannel::getMidpointTolerance()						{	return( miMidPointTolerance );		}
unsigned long CTurnigy9XChannel::getPulseReadTimeout()						{	return( miPulseReadTimeout );		}
unsigned long CTurnigy9XChannel::getEmitterStartupTime()					{	return( miEmitterStartupTime );		}

// ////////////////////////////////////////////
//
// Range settings
//
void CTurnigy9XChannel::setPulseLengthRange( float piMinPulseLength, float piMaxPulseLength )
{
	miMinPulseLength = piMinPulseLength;
	miMaxPulseLength = piMaxPulseLength;

	miCenterPulseLength = miMinPulseLength + ( ( miMaxPulseLength - miMinPulseLength ) / 2 );
}

void CTurnigy9XChannel::setMappingRange( float piMinMappingValue, float piMaxMappingValue )
{
	miMinMappingValue = piMinMappingValue;
	miMaxMappingValue = piMaxMappingValue;

	miCenterMappingValue = round( miMinMappingValue + ( ( miMaxMappingValue - miMinMappingValue ) / 2 ) );
}

float CTurnigy9XChannel::getMinPulseLength()		{	return miMinPulseLength;	}
float CTurnigy9XChannel::getMaxPulseLength()		{	return miMaxPulseLength;	}
float CTurnigy9XChannel::getCenterPulseLength()		{	return miCenterPulseLength;	}

float CTurnigy9XChannel::getMinMappingValue()		{	return miMinMappingValue;	}
float CTurnigy9XChannel::getMaxMappingValue()		{	return miMaxMappingValue;	}
float CTurnigy9XChannel::getCenterMappingValue()	{	return miCenterMappingValue;	}

// ////////////////////////////////////////////
//
// Data read
//
bool CTurnigy9XChannel::read()
{
	unsigned long liCurrTime = millis();

	mbDataChanged = false;

	// Unatacched channel ?
	if( miPin == PIN_NOT_SET )
	{
		mbActive = false;
		return( false );
	}

	miCurrentPulseLength = pulseIn( miPin, HIGH, miPulseReadTimeout );

	// Emitter turned off ?
	if( miCurrentPulseLength == 0 )
	{
		mbActive = false;
		return( false );
	}

	// Emitter JUST turned on ?
	if( !mbActive )
	{
		mbActive = true;

		// Remember activation time to be able to skip initial glitches
		miActivationTime = liCurrTime;
	}

	// Emitter still in startup time ?
	if( miActivationTime != TIME_NOT_SET )
	{
		// If the startup time has lapsed
		if( ( liCurrTime - miActivationTime ) > miEmitterStartupTime )
		{
			// Reset activation time so we know we are already out of startup time
			miActivationTime = TIME_NOT_SET;
		}
		else
		{
			// As the emitter is still initializing, exit to avoid initial glitches
			return( false );
		}

	}

	// Calibrating ?
	if( mbCalibrating )
	{
		if( miCurrentPulseLength < miMinPulseLength )
			setPulseLengthRange( miCurrentPulseLength+1, miMaxPulseLength );	// +1 to smooth 1us ripple from receiver

		if( miCurrentPulseLength > miMaxPulseLength )
			setPulseLengthRange( miMinPulseLength, miCurrentPulseLength-1 );	// -1 to smooth 1us ripple from receiver

		if( miCalibrationProgrammedTime != TIME_NOT_SET )
		{
			if( ( liCurrTime - miCalibrationStartTime ) >= miCalibrationProgrammedTime )
				stopCalibration();
		}
	}

	// Midpoints tolerance
	if( abs( miCurrentPulseLength - miMinPulseLength ) <= ( miMidPointTolerance / 2 ) )		// Partial tolerance on extremes
		miCurrentPulseLength = miMinPulseLength;

	if( abs( miCurrentPulseLength - miCenterPulseLength ) <= miMidPointTolerance )
		miCurrentPulseLength = miCenterPulseLength;

	if( abs( miCurrentPulseLength - miMaxPulseLength ) <= ( miMidPointTolerance / 2 ) )		// Partial tolerance on extremes
		miCurrentPulseLength = miMaxPulseLength;

	// Same value ?
	if( miCurrentPulseLength == miLastUsedPulseLength )
		return( false );

	// Debounce!
	if( abs( miLastUsedPulseLength - miCurrentPulseLength ) < miDebounceTreshold )
		return( false );

	// Normal read
	miPreviousUsedPulseLength = miLastUsedPulseLength;
	miLastUsedPulseLength = miCurrentPulseLength;

	// Value mapping
	miCurrentMappedValue = floatMap(	miCurrentPulseLength,
										miMinPulseLength,	miMaxPulseLength,
										miMinMappingValue,	miMaxMappingValue );
	// Value rounding
	miCurrentMappedValue = round( miCurrentMappedValue );

	// Value constrain
	miCurrentMappedValue = constrain( miCurrentMappedValue, miMinMappingValue, miMaxMappingValue );

	if( miCurrentMappedValue == miLastMappedValue )
		return( false );

	miLastMappedValue = miCurrentMappedValue;
	mbDataChanged = true;

	return( true );
}


// ////////////////////////////////////////////
//
// reset()
//
void CTurnigy9XChannel::reset()
{
	// Pin assignation
	miPin = PIN_NOT_SET;

	// Channel state
	mbActive = false;
	miActivationTime = TIME_NOT_SET;
	mbCalibrating = false;
	miCalibrationStartTime = TIME_NOT_SET;
	miCalibrationProgrammedTime = TIME_NOT_SET;
	mbDataChanged = false;

	// Mapping
	setPulseLengthRange( DEFAULT_MIN_PULSE_LENGTH, DEFAULT_MAX_PULSE_LENGTH );
	setMappingRange( DEFAULT_MIN_MAPPING_VALUE, DEFAULT_MAX_MAPPING_VALUE );

	// Tresholds
	setDebounceTreshold( DEFAULT_DEBOUNCE_TRESHOLD );
	setMidpointTolerance( DEFAULT_MIDPOINT_TOLERANCE );
	setPulseReadTimeout( DEFAULT_PULSE_READ_TIMEOUT );
	setEmitterStartupTime( DEFAULT_EMITTTER_STARTUP_TIME );

	// Data
	miCurrentPulseLength		= miCenterPulseLength;
	miLastUsedPulseLength		= miCenterPulseLength;
	miPreviousUsedPulseLength	= miCenterPulseLength;
	miCurrentMappedValue		= miCenterMappingValue;
	miLastMappedValue			= miCenterMappingValue;
}

// ////////////////////////////////////////////
//
// floatMap() : like map(), but using floats
//
float CTurnigy9XChannel::floatMap( float x, float in_min, float in_max, float out_min, float out_max )
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
