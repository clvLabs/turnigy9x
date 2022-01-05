// *****************************************************************
// ***
// ***	 Turnigy9X
// ***	-----------
// ***
// ***	Turnigy9X RC radio receiver manager
// ***

#ifndef Turnigy9X_h
#define Turnigy9X_h

// *****************************************************************
// ***
// *** Includes
// ***

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Turnigy9XChannel.h"

// *****************************************************************
// ***
// *** Constants
// ***

// Total number of channels
#define MAX_CHANNELS					( 15 )			// Should fit most receivers
#define USED_CHANNELS					( 8 )


// *****************************************************************
// ***
// *** CTurnigy9XReceiver
// ***
// *** Manager class
// ***
class CTurnigy9XReceiver
{
private:
	
	// ////////////////////////////////////////////
	//
	// Variables
	//
	
	// Channel list
	CTurnigy9XChannel mChannels[ MAX_CHANNELS ];
	
public:
	
	// ////////////////////////////////////////////
	//
	// Constructor
	//
	CTurnigy9XReceiver();
	
	// ////////////////////////////////////////////
	//
	// Channel access
	//
	CTurnigy9XChannel* getChannel( int piChannel );
	
	bool isAnyChannelActive();
	bool isAnyChannelCalibrating();

	// ////////////////////////////////////////////
	//
	// Data read
	//
	bool read();
	
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
	void setPulseLengthRange( float piMinPulseLength, float piMaxPulseLength );
	void setMappingRange( float piMinMappingValue, float piMaxMappingValue );

	void setDebounceTreshold( int piTreshold );
	void setMidpointTolerance( int piTolerance );
	void setPulseReadTimeout( int piTimeout );

private:
	
};

#endif		// #ifndef Turnigy9X_h