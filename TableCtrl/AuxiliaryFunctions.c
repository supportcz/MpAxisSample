
#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <string.h>

/* function declaration */
void SetAlarmConfig();

/********************************************************************
* Switch Mode Reset
********************************************************************/
void SwitchModeReset(void) {

	/* variable declaration */
	BOOL StopCmd = 0;
	BOOL LoadCmd = 0;
	
	/* control pointers */
	if (pIntf != 0) {
		/* save command */
		StopCmd = pIntf->Cmd.Stop;
		LoadCmd = pIntf->Cmd.LoadConfig;
		/* reset commands */
		memset(&pIntf->Cmd, 0, sizeof(pIntf->Cmd));
		/* set stop command */
		pIntf->Cmd.Stop = StopCmd;
		pIntf->Cmd.LoadConfig = LoadCmd;
	}
	
} /* end of switch mode reset function */

/********************************************************************
* Reset Axis Commands
********************************************************************/
void ResetAxisCmd(void) {
	
	/* control pointers */
	if (pInstInt != 0) {
		/* reset axis commands */
		pInstInt->MpAxisBasic_0.Update = 0;
		pInstInt->MpAxisBasic_0.Home = 0;
		pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
		pInstInt->MpAxisBasic_0.MoveAdditive = 0;
		pInstInt->MpAxisBasic_0.MoveVelocity = 0;
		pInstInt->MpAxisBasic_0.JogPositive = 0;
		pInstInt->MpAxisBasic_0.JogNegative = 0;
		pInstInt->MpAxisBasic_0.Autotune = 0;
	}
	
} /* end of reset axis commands function */

/********************************************************************
* Stop Machine
********************************************************************/
void Stop(void) {

	/* control pointers */
	if (pInstInt != 0) {
		/* call function switch mode reset */
		SwitchModeReset();
		/* call function to reset axis commands */
		ResetAxisCmd();
		/* control move is active - stop axis */
		if (pInstInt->MpAxisBasic_0.MoveActive) pInstInt->MpAxisBasic_0.Stop = 1;
	}
	
} /* end of stop machine function */


/********************************************************************
* Init Axis Parameters
********************************************************************/
void InitAxisPara(void) {
	
	/* control pointers */
	if (pInstInt != 0) {
		/* set home parameters */
		pInstInt->MpAxisParameters.Home.Position = 0;
		pInstInt->MpAxisParameters.Home.StartVelocity = 5;
		pInstInt->MpAxisParameters.Home.HomingVelocity = 0.1;
		pInstInt->MpAxisParameters.Home.Acceleration = 50;
		/* control simulation - set homing mode */
		if (!gMachSetup.mon.Info.ARsimUsed) pInstInt->MpAxisParameters.Home.Mode = mpAXIS_HOME_MODE_ABS_SWITCH;
		else pInstInt->MpAxisParameters.Home.Mode = mpAXIS_HOME_MODE_DIRECT;
		pInstInt->MpAxisParameters.Home.StartDirection = mpAXIS_HOME_DIR_NEGATIVE;
		pInstInt->MpAxisParameters.Home.HomingDirection = mpAXIS_HOME_DIR_NEGATIVE;
		pInstInt->MpAxisParameters.Home.SwitchEdge = mpAXIS_HOME_DIR_NEGATIVE;
		/* fill configuration of hardware end switch */
		strcpy(pInstInt->MCExtHwEndSwConfig.AxisName, "AxisTable");
		strcpy(pInstInt->MCExtHwEndSwConfig.DeviceName, "IF1.ST10.IF1.ST2");
		pInstInt->MCExtHwEndSwConfig.PosLimitSwChannel = mcEXT_HW_END_CHANNEL_NONE;
		pInstInt->MCExtHwEndSwConfig.NegLimitSwChannel = mcEXT_HW_END_CHANNEL_NONE;
		pInstInt->MCExtHwEndSwConfig.ReferenceSwChannel = mcEXT_HW_END_DIGITAL_INPUT_01;
	}
	
	
} /* end of reset axis commands function */

/********************************************************************
* Reset Active Commands
********************************************************************/
void ResetActiveCmd(void) {

	/* control pointers */
	if (pInstInt != 0) {
		/* reset active commands */
		memset(&pInstInt->ActiveCmd, 0, sizeof(pInstInt->ActiveCmd));
	}
	
} /* end of switch mode reset function */

/********************************************************************
* Save Log StatusID Config
********************************************************************/
void SaveLogStatusIDConfig(struct DGDataSaveLogStatusIDConfigType* SaveLogConfig, BOOL Init) {
	
	/* control pointers */
	if ((pIntf != 0) && (pInst != 0) && (SaveLogConfig != 0)) {
		/* control init */
		if (Init) {
			/* set object ID */
			strcpy(SaveLogConfig->LogObjectID, "TableCtrl");
			/* set text preface */
			strcpy(SaveLogConfig->LogTextPreface, "Turn table control: ");
			/* set EventID */
			SaveLogConfig->LogEventID = tblCTRL_ERR_GENERAL;
			/* set log on */
			SaveLogConfig->LogOn = 1;
			/* set log name */
			strcpy(SaveLogConfig->LogName, "MachLog");
		}
		/* save address to StatusID in monitor structure */
		SaveLogConfig->pMonStatusID = (DINT*)&pIntf->Mon.StatusID;
		/* save address to Error in monitor structure */
		SaveLogConfig->pMonError = (BOOL*)&pIntf->Mon.Error;
		/* save address to ErrorCnt in internal structure */
		SaveLogConfig->pInternalErrorCnt = (UDINT*)&pInst->DiagCounters.ErrorCnt;
		/* save address to StatusID struct in diagnostic structure */
		SaveLogConfig->pDiagStatusIDStruct = (DGDataStatusIDType*)&pIntf->Mon.Info.Diag.Parts.StatusID;
		/* save address to FirstLevelID struct in diagnostic structure */
		SaveLogConfig->pDiagFirstLevelIDStruct = (DGDataInternalIDType*)&pIntf->Mon.Info.Diag.Parts.Internal.FirstLevel;
		/* save address to NextLevelsID struct in diagnostic structure */
		SaveLogConfig->pDiagNextLevelIDStruct = (DGDataInternalIDType*)&pIntf->Mon.Info.Diag.Parts.Internal.NextLevels[0];
		/* save size of NextLevelsID struct */
		SaveLogConfig->DiagNextLevelIDSize = sizeof(pIntf->Mon.Info.Diag.Parts.Internal.NextLevels);
		/* save address to State in monitor structure */
		SaveLogConfig->pMonState = (DINT*)&pIntf->Mon.State;
		/* save address to Last Error States in diagnostic structure */
		SaveLogConfig->pDiagLastErrorStates = (DINT*)&pIntf->Mon.Info.Diag.LastErrorStates;
		/* save size of Last Error States array */
		SaveLogConfig->DiagLastErrorStatesSize = sizeof(pIntf->Mon.Info.Diag.LastErrorStates);
		/* save address of diagnostic buffer */
		SaveLogConfig->pLogBuffer = gMachDgData.pLogBuffer;
		/* set mode */
		SaveLogConfig->LogMode = dgDATA_MODE_CFG_ON_WRITE_BOTH;
		/* save set alarm function address */
		SaveLogConfig->AlarmFceAdr = (UDINT)SetAlarmConfig;
	}
	
} /* end save log StatusID config */

/********************************************************************
* Set Alarm Config
********************************************************************/
void SetAlarmConfig(void) {
	
	/* control pointers */
	if ((pIntf != 0) && (pInst != 0) && (pIntfSetup != 0) && (gAlarm.Mon.Info.AlarmSystemActive)) {
		/* control StatusID */
		if ((pIntf->Mon.StatusID != tblCTRL_NO_ERROR) && (pIntf->Mon.StatusID != pInst->Internal.StatusIDOld)) {
			pInst->Internal.StatusIDOld = pIntf->Mon.StatusID;
			/* control status ID */
			if (pIntf->Mon.StatusID == tblCTRL_ERR_STATION_NOT_IN_POS) {
				/* set alarm */
				pInst->AlarmInsID = MpAlarmXSet(&gAlarmXCoreLink, (STRING*)&tblCTRL_ALM_STATION_NOT_IN_POS);
			} else if ((pIntf->Mon.StatusID == tblCTRL_ERR_POWER_ON_AXIS) || (pIntf->Mon.StatusID == tblCTRL_ERR_AXIS_NOT_READY_PWR)
				|| (pIntf->Mon.StatusID == tblCTRL_ERR_HOME_AXIS)  || (pIntf->Mon.StatusID == tblCTRL_ERR_HOME_AXIS_TIME_OUT)
			|| (pIntf->Mon.StatusID == tblCTRL_ERR_POWER_OFF_AXIS) || (pIntf->Mon.StatusID == tblCTRL_ERR_INIT_HW_END_SW)) {
				/* set alarm */
				pInst->AlarmInsID = MpAlarmXSet(&gAlarmXCoreLink, (STRING*)&tblCTRL_ALM_AXIS_PREPARE);
			} else if ((pIntf->Mon.StatusID == tblCTRL_ERR_TURN_TABLE) || (pIntf->Mon.StatusID == tblCTRL_ERR_JOG_POS)
			|| (pIntf->Mon.StatusID == tblCTRL_ERR_JOG_NEG)) {
				/* set alarm */
				pInst->AlarmInsID = MpAlarmXSet(&gAlarmXCoreLink, (STRING*)&tblCTRL_ALM_AXIS_MOVE);
			}
		} else if (pIntf->Mon.StatusID == tblCTRL_NO_ERROR) {
			pInst->Internal.StatusIDOld = pIntf->Mon.StatusID;
		}
	}
	
}

/********************************************************************
* Calculation Next Position
********************************************************************/
LREAL CalNextPosition(void) {
	
	/* variable declaration */
	LREAL nextPos = 0;
	LREAL nextPosSlowDown = 0;
	
	/* control pointers */
	if ((pIntfSetup != 0) && (pIntfRecipe) && (pInstInt != 0)) {
		/* control station distance */
		if (pIntfSetup->TurnTable.StationDistance <= 0) pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
		/* control clips width */
		if (pIntfRecipe->TurnTable.ClipsWidth <= 0) pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		/* control period */
		if (pInstInt->MpAxisConfig.Axis.PeriodSettings.Period == 0) pInstInt->MpAxisConfig.Axis.PeriodSettings.Period = tblCTRL_AXIS_PERIOD;
		/* control station distance and clips width */
		if (pIntfSetup->TurnTable.StationDistance <= (pIntfRecipe->TurnTable.ClipsWidth/2.0)) {
			/* set default values */
			pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
			pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		}
		/* calculate next position */
		nextPos = ((((UDINT)((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0))/pIntfSetup->TurnTable.StationDistance))+1)*pIntfSetup->TurnTable.StationDistance)-(pIntfRecipe->TurnTable.ClipsWidth/2.0);
		/* control next position */
		if (nextPos >= pInstInt->MpAxisConfig.Axis.PeriodSettings.Period) nextPos -= pInstInt->MpAxisConfig.Axis.PeriodSettings.Period;
		/* control distance slow down before in position */
		if (pIntfRecipe->TurnTable.DistanceNotInPosition != 0) {
			/* control distance slow donw before in position */
			if (pIntfRecipe->TurnTable.DistanceNotInPosition < 0) pIntfRecipe->TurnTable.DistanceNotInPosition = -1*pIntfRecipe->TurnTable.DistanceNotInPosition;
			if (pIntfRecipe->TurnTable.DistanceNotInPosition > (pIntfSetup->TurnTable.StationDistance/1.05)) pIntfRecipe->TurnTable.DistanceNotInPosition = (pIntfSetup->TurnTable.StationDistance/1.05);
			/* control next position */
			if (nextPos == 0) nextPosSlowDown = pInstInt->MpAxisConfig.Axis.PeriodSettings.Period;
			else nextPosSlowDown = nextPos;
			/* control actual position */
			if (gTableControl.Mon.Info.Position > (nextPosSlowDown-pIntfRecipe->TurnTable.DistanceNotInPosition)) pInstInt->SlowBeforeInPos = 1;
			else pInstInt->SlowBeforeInPos = 0;
		} else {
			/* reset variable */
			pInstInt->SlowBeforeInPos = 0;
		}
		/* return next position */
		return nextPos;
	} else {
		/* return next position */
		return nextPos;
	}
	
} /* end of calculation next position function */

/********************************************************************
* Calculation Previous Position
********************************************************************/
LREAL CalPrevPosition(void) {
	
	/* variable declaration */
	LREAL prevPos = 0;
	LREAL epsilon = 0.0001;
	LREAL position = 0;
	
	/* control pointers */
	if ((pIntfSetup != 0) && (pIntfRecipe) && (pInstInt != 0)) {
		/* control length distance */
		if (pIntfSetup->TurnTable.StationDistance <= 0) pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
		/* control clips width */
		if (pIntfRecipe->TurnTable.ClipsWidth <= 0) pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		/* control period */
		if (pInstInt->MpAxisConfig.Axis.PeriodSettings.Period == 0) pInstInt->MpAxisConfig.Axis.PeriodSettings.Period = tblCTRL_AXIS_PERIOD;
		/* control station distance and clips width */
		if (pIntfSetup->TurnTable.StationDistance <= (pIntfRecipe->TurnTable.ClipsWidth/2.0)) {
			/* set default values */
			pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
			pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		}
		/* control zero axis position - calculate previous position */
		if (pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0)-epsilon < 0) prevPos = (((UDINT)((pInstInt->MpAxisConfig.Axis.PeriodSettings.Period-epsilon)/pIntfSetup->TurnTable.StationDistance))*pIntfSetup->TurnTable.StationDistance)-(pIntfRecipe->TurnTable.ClipsWidth/2.0);
		else prevPos = (((UDINT)((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0)-epsilon)/pIntfSetup->TurnTable.StationDistance))*pIntfSetup->TurnTable.StationDistance)-(pIntfRecipe->TurnTable.ClipsWidth/2.0);
		/* control next position */
		if (prevPos < 0) prevPos += pInstInt->MpAxisConfig.Axis.PeriodSettings.Period;
		/* control distance slow down before in position */
		if (pIntfRecipe->TurnTable.DistanceNotInPosition != 0) {
			/* control distance slow donw before in position */
			if (pIntfRecipe->TurnTable.DistanceNotInPosition < 0) pIntfRecipe->TurnTable.DistanceNotInPosition = -1*pIntfRecipe->TurnTable.DistanceNotInPosition;
			if (pIntfRecipe->TurnTable.DistanceNotInPosition > (pIntfSetup->TurnTable.StationDistance/1.05)) pIntfRecipe->TurnTable.DistanceNotInPosition = (pIntfSetup->TurnTable.StationDistance/1.05);
			/* control position */
			if (gTableControl.Mon.Info.Position == 0) position = pInstInt->MpAxisConfig.Axis.PeriodSettings.Period;
			else position = gTableControl.Mon.Info.Position;
			/* control actual position */
			if (position < (prevPos+pIntfRecipe->TurnTable.DistanceNotInPosition)) pInstInt->SlowBeforeInPos = 1;
			else pInstInt->SlowBeforeInPos = 0;
		} else {
			/* reset variable */
			pInstInt->SlowBeforeInPos = 0;
		}
		/* return previous position */
		return prevPos;
	} else {
		/* return previous position */
		return prevPos;
	}
	
} /* end of calculation previous position function */

/********************************************************************
* Calculation In Position
********************************************************************/
BOOL CalInPosition(void) {
	
	/* variable declaration */
	BOOL inPos = 0;
	LREAL diffPos = 0;
	LREAL epsilon = 0.00001;
	
	/* control pointers */
	if ((pIntfSetup != 0) && (pIntfRecipe) && (pInstInt != 0)) {
		/* control length distance */
		if (pIntfSetup->TurnTable.StationDistance <= 0) pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
		/* control clips width */
		if (pIntfRecipe->TurnTable.ClipsWidth <= 0) pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		/* control station distance and clips width */
		if (pIntfSetup->TurnTable.StationDistance <= (pIntfRecipe->TurnTable.ClipsWidth/2.0)) {
			/* set default values */
			pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
			pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
		}
		/* calculation of position difference */
		diffPos = (((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0))/pIntfSetup->TurnTable.StationDistance)-((UDINT)((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0))/pIntfSetup->TurnTable.StationDistance)));
		/* control position difference - set in position */
		if (diffPos > epsilon) inPos = 0;
		else if (diffPos < -epsilon) inPos = 0;
		else inPos = 1;
		/* return in position */
		return inPos;
	} else {
		/* return in position */
		return inPos;
	}
	
} /* end of calculation in position function */

/********************************************************************
* Calculation Out Position
********************************************************************/
BOOL CalOutPosition(void) {
	
	/* variable declaration */
	BOOL outPos = 0;
	LREAL diffPos = 0;
	LREAL distNotPos = 0;
	
	/* control pointers */
	if ((pIntfSetup != 0) && (pIntfRecipe) && (pInstInt != 0)) {
		/* control turn table is not in postion */
		if (!gTableControl.Mon.Info.InPosition) {
			/* control length distance */
			if (pIntfSetup->TurnTable.StationDistance <= 0) pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
			/* control clips width */
			if (pIntfRecipe->TurnTable.ClipsWidth <= 0) pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
			/* control station distance and clips width */
			if (pIntfSetup->TurnTable.StationDistance <= (pIntfRecipe->TurnTable.ClipsWidth/2.0)) {
				/* set default values */
				pIntfSetup->TurnTable.StationDistance = tblCTRL_STATION_DISTANCE;
				pIntfRecipe->TurnTable.ClipsWidth = tblCTRL_CLIPS_WIDTH;
			}
			/* calculation of position difference */
			diffPos = (((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0))/pIntfSetup->TurnTable.StationDistance)-((UDINT)((pInstInt->MpAxisBasic_0.Position+(pIntfRecipe->TurnTable.ClipsWidth/2.0))/pIntfSetup->TurnTable.StationDistance)));
			/* calculation disatance leave in postion */
			distNotPos = tblCTRL_DIST_NOT_IN_POS/(pIntfSetup->TurnTable.StationDistance);
			/* control position difference is negative - make position value */
			if (diffPos < 0) diffPos = -1*diffPos;
			/* control position difference - set out position */
			if ((diffPos > distNotPos) && (diffPos < (1-distNotPos))) outPos = 1;
			else outPos = 0;
			/* return out position */
			return outPos;
		} else {
			/* set out position */
			outPos = 0;
			/* return out position */
			return outPos;
		}
	} else {
		/* return out position */
		return outPos;
	}
	
} /* end of calculation out position function */
