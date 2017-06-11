#include "timer.h"
#include <windows.h>

void							STimer::Reset											()								{
	QueryPerformanceFrequency( ( LARGE_INTEGER* )&CountsPerSecond );
	SecondsPerCount					= (1.0 / (CountsPerSecond));
	QueryPerformanceCounter( ( LARGE_INTEGER* )&PrevTimeStamp );		
	LastTime 						= 0;
}

void							STimer::Frame											()								{
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &CurrentTimeStamp );
	LastTime						=  (float)(( CurrentTimeStamp - PrevTimeStamp )* SecondsPerCount);
	PrevTimeStamp					= CurrentTimeStamp;
}
