/*****************************************************************/
/*****************************************************************/
/*
	Motocycle / Car Tracker firmware for Particle Electron
	(c) Copyright 2017, Jason R. Haddix

*/
/*****************************************************************/
/*****************************************************************/

#include "_libs/AssetTracker/AssetTracker.h"
#include "math.h"




// DEFINITIONS
#define TIME millis()                                // Global system run time - milliseconds




// PIN VARS
int POWER_PIN = D0;									 // Pin used to determine power states from external source
int ALARM_PIN = D1;                                  // Pin used to trigger piezo alarm
int WKP_PIN = A1;                                    // Pin used to used to wake tracker from SLEEP state

// GLOBAL VARS
long GLOBAL_LAST_CHECK = 0;                          // TIME since last APP_MODE check
long GLOBAL_LAST_CHECK_DELAY = 1;                    // Check/set app mode every [x] minutes
int GLOBAL_BOOT_CHECK = 1;                           // Initial app mode check (runs loop until it gets out of 0:BOOT mode)
int DEEP_SLEEP_TIME = 120;                           // [x] minutes until it exits DEEP SLEEP MODE to check app state
bool GPS = 0;                                         // GPS - 1:TRUE (within geo-fence) | 2: FALSE (outside ge-fence)
bool POWER = 0;                                       // POWER - 1:TRUE (vehicle running) | 2: FALSE (vehicle off)
bool ACCEL = 0;										 // ACCEL - 1: TRUE (threshold hit) | 2: FALSE (threshold not hit)
bool ALARM = 0;
int APP_MODE = 0;                                    // 0:BOOT / 1:SLEEP / 2: REST / 3: GUARD / 4:ALERT 
int PREV_APP_MODE;                                   // Previous APP_MODE state

// WATCHDOG TIMER 
long timer_WatchDog_ResetLast = 0;                   // TIME since last last system reset
int timer_WatchDog_ResetDelay = 24;                  // [x] hours until full system reset




// PARTICLE HARDWARE VARS
AssetTracker tracker = AssetTracker();                // Particle Tracker shield
FuelGauge fuel;                                       // LiPo Battery
CellularSignal cell;                                  // Electron cell module




void setup()
{
	Serial.begin(9600);

	pinMode(POWER_PIN, INPUT);
	pinMode(ALARM_PIN, OUTPUT);
	pinMode(WKP_PIN, INPUT);

	Time.zone(-8); 
  	Time.hourFormat12();

  	tracker.begin();
	tracker.gpsOn();

	define_ExternalFunctions();
	timer_WatchDog();
}




void define_ExternalFunctions()
{
	Particle.function( "ALERT", set_Mode_ALERT );
	Particle.function( "CELL", get_Cell_Strength );	
}




void loop()
{


}




//****************************************************************/
// WATCHDOG TIMER / SYSTEM RESET / Avoids stack overflow
//****************************************************************/
void timer_WatchDog()
{
	if ( TIME - timer_WatchDog_ResetLast > timer_WatchDog_ResetDelay*60000UL*60000UL ) {

		System.reset();

	}
}
//****************************************************************/
//****************************************************************/	
