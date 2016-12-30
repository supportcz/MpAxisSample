
#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <string.h>

/********************************************************************
* Factory settings function
********************************************************************/
void FactorySettings(void) {	
		
	/********************************* TURN TABLE SETTINGS *****************************/
	gTableControlSetup.TurnTable.Acceleration = 1600;
	gTableControlSetup.TurnTable.Decelaration = 1600;
	gTableControlSetup.TurnTable.ManualVelocity = tblCTRL_MANUAL_VELOCITY;
	gTableControlSetup.TurnTable.SensorOffset = tblCTRL_HOME_SENSOR_OFFSET;
	gTableControlSetup.TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
		
	
} /* end of factory settings function */

/********************************************************************
* Default Recipe function
********************************************************************/
void DefaultRecipe(void) {
	
	/*********************************** TURN TABLE RECIPE *******************************/
	gTableControlRecipe.TurnTable.AutoVelocity = tblCTRL_AUTO_VELOCITY;
	gTableControlRecipe.TurnTable.AutoVelocitySlow = 10;
	gTableControlRecipe.TurnTable.DistanceNotInPosition = tblCTRL_DIST_BEFORE_IN_POS;
	gTableControlRecipe.TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
	
} /* end of default recipe function */
