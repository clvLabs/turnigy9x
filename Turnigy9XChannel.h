// *****************************************************************
// ***
// ***	 Turnigy9XChannel
// ***	------------------
// ***
// ***	Turnigy9X RC radio receiver channel manager class
// ***

#ifndef Turnigy9XChannel_h
#define Turnigy9XChannel_h

// *****************************************************************
// ***
// *** Includes
// ***

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

// *****************************************************************
// ***
// *** Constants
// ***

#define PIN_NOT_SET						( -1 )
#define TIME_NOT_SET					( 0 )

// Default min/max pulse lengths
#define DEFAULT_MIN_PULSE_LENGTH		( 1000 )
#define DEFAULT_MAX_PULSE_LENGTH		( 2000 )

// Default min/max mapping values
#define DEFAULT_MIN_MAPPING_VALUE		( 0 )
#define DEFAULT_MAX_MAPPING_VALUE		( 1023 )

// Default tolerance to detect UP / MIDDLE / DOWN
#define DEFAULT_MIDPOINT_TOLERANCE		( 50 )

// Default debounce treshold value
#define DEFAULT_DEBOUNCE_TRESHOLD		( 10 )

// Default pulse read timeout
#define DEFAULT_PULSE_READ_TIMEOUT		( 21000 )

// Default emitter startup time
#define DEFAULT_EMITTTER_STARTUP_TIME	( 2000 )

// *****************************************************************
// ***
// *** CTurnigy9XChannel
// ***
// *** Channel class
// ***
class CTurnigy9XChannel
{

private:
	// *****************************************************************
	// ***
	// *** Variables
	// ***

	// Channel specific settings
	int miPin;

	bool mbActive;
	unsigned long miActivationTime;
	bool mbCalibrating;
	unsigned long miCalibrationStartTime;
	unsigned long miCalibrationProgrammedTime;
	bool mbDataChanged;

	float miMinPulseLength;
	float miMaxPulseLength;
	float miCenterPulseLength;
	float miMinMappingValue;
	float miMaxMappingValue;
	float miCenterMappingValue;

	unsigned int miDebounceTreshold;
	unsigned int miMidPointTolerance;
	unsigned long miPulseReadTimeout;
	unsigned long miEmitterStartupTime;

	float miCurrentPulseLength;
	float miLastUsedPulseLength;
	float miPreviousUsedPulseLength;
	float miCurrentMappedValue;
	float miLastMappedValue;

public:
	// ////////////////////////////////////////////
	//
	// Constructor
	//
	CTurnigy9XChannel();

	// ////////////////////////////////////////////
	//
	// Channel attach / detach
	//
	void attach( int piPin );
	void detach();

	// ////////////////////////////////////////////
	//
	// Channel data access
	//
	bool isActive();
	bool isCalibrating();
	bool dataChanged();

	bool isUp();
	bool isDown();
	bool isTop();
	bool isBottom();
	bool isCentered();

	float getLastValue();
	float getValue();
	float getPulseLength();
	float getLastPulseLength();
	float getPreviousPulseLength();


	// ////////////////////////////////////////////
	//
	// Calibration
	//
	void calibrate( unsigned long piTime );
	void startCalibration();
	void stopCalibration();

	// ////////////////////////////////////////////
	//
	// Treshold settings
	//
	void setDebounceTreshold( unsigned int piTreshold );
	void setMidpointTolerance( unsigned int piTolerance );
	void setPulseReadTimeout( unsigned long piTimeout );
	void setEmitterStartupTime( unsigned long piTime );

	unsigned int getDebounceTreshold();
	unsigned int getMidpointTolerance();
	unsigned long getPulseReadTimeout();
	unsigned long getEmitterStartupTime();

	// ////////////////////////////////////////////
	//
	// Range settings
	//
	void setPulseLengthRange( float piMinPulseLength, float piMaxPulseLength );
	void setMappingRange( float piMinMappingValue, float piMaxMappingValue );
	float getMinPulseLength();
	float getMaxPulseLength();
	float getCenterPulseLength();

	float getMinMappingValue();
	float getMaxMappingValue();
	float getCenterMappingValue();

	// ////////////////////////////////////////////
	//
	// Data read
	//
	bool read();


private:
	// ////////////////////////////////////////////
	//
	// reset()
	//
	void reset();

	// ////////////////////////////////////////////
	//
	// floatMap() : like map(), but using floats
	//
	float floatMap( float x, float in_min, float in_max, float out_min, float out_max );
};


#endif		// #ifndef Turnigy9XChannel_h