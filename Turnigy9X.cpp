// ***********************************************************************************************
// ***
// ***	 Turnigy9X
// ***	-----------
// ***
// ***	Turnigy9X RC radio receiver manager class
// ***

// ***********************************************************************************************
// ***
// *** Includes
// ***

#include "Turnigy9X.h"


// ***********************************************************************************************
// ***
// *** CTurnigy9XReceiver
// ***
// *** Manager class
// ***

// ////////////////////////////////////////////
//
// Constructor
//
CTurnigy9XReceiver::CTurnigy9XReceiver()
{
}

// ////////////////////////////////////////////
//
// Channel access
//
CTurnigy9XChannel* CTurnigy9XReceiver::getChannel( int piChannel )
{
	return( &(mChannels[ piChannel-1 ] ) );
}

bool CTurnigy9XReceiver::isAnyChannelActive()
{
	bool lbAnyActive = false;
	
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
	{
		if( mChannels[ liLoop ].isActive() )
		{
			lbAnyActive = true;
			break;
		}
	}
	
	return( lbAnyActive );
}	

bool CTurnigy9XReceiver::isAnyChannelCalibrating()
{
	bool lbAnyCalibrating = false;
	
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
	{
		if( mChannels[ liLoop ].isCalibrating() )
		{
			lbAnyCalibrating = true;
			break;
		}
	}
	
	return( lbAnyCalibrating );
}	

// ////////////////////////////////////////////
//
// Data read
//
bool CTurnigy9XReceiver::read()
{
	bool lbAnyChanged = false;
	
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
	{
		if( mChannels[ liLoop ].read() )
			lbAnyChanged = true;
	}
	
	return( lbAnyChanged );
}	

// ////////////////////////////////////////////
//
// Calibration
//
void CTurnigy9XReceiver::calibrate( unsigned long piTime )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].calibrate( piTime );
}

void CTurnigy9XReceiver::startCalibration()
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].startCalibration();
}

void CTurnigy9XReceiver::stopCalibration()
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].stopCalibration();
}

// ////////////////////////////////////////////
//
// Treshold settings
//
void CTurnigy9XReceiver::setPulseLengthRange( float piMinPulseLength, float piMaxPulseLength )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].setPulseLengthRange( piMinPulseLength, piMaxPulseLength );
}

void CTurnigy9XReceiver::setMappingRange( float piMinMappingValue, float piMaxMappingValue )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].setMappingRange( piMinMappingValue, piMaxMappingValue );
}

void CTurnigy9XReceiver::setDebounceTreshold( int piTreshold )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].setDebounceTreshold( piTreshold );
}

void CTurnigy9XReceiver::setMidpointTolerance( int piTolerance )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].setMidpointTolerance( piTolerance );
}

void CTurnigy9XReceiver::setPulseReadTimeout( int piTimeout )
{
	for( int liLoop = 0; liLoop < USED_CHANNELS; liLoop++ )
		mChannels[ liLoop ].setPulseReadTimeout( piTimeout );
}
