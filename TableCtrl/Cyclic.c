
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

/* function declaration */
void SaveLogStatusIDConfig(struct DGDataSaveLogStatusIDConfigType*, BOOL);
void SetAlarmConfig();
void FactorySettings();
void DefaultRecipe();
void SwitchModeReset();
void Stop();
void ResetAxisCmd();
void InitAxisPara();
void ResetActiveCmd();
LREAL CalNextPosition();
LREAL CalPrevPosition();
BOOL CalInPosition();
BOOL CalOutPosition();

void _CYCLIC ProgramCyclic(void)
{
		
	/******************************** FILL THE POINTERS *******************************/
	/* fill pointers */
	pIntf = &gTableControl;
	pIntfSetup = &gTableControlSetup;
	pIntfRecipe = &gTableControlRecipe;
	pInst = &TableControl;
	pInstInt = &TableControl.Internal;
	
	/* control pointers */
	if ((pIntf != 0) && (pIntfSetup != 0) && (pIntfRecipe != 0) && (pInst != 0) && (pInstInt != 0)) {
		
		/* call function to config Save Log StatusID */
		SaveLogStatusIDConfig(&pInst->StatusIDConfig, 0);
		
		/*********************************** SIMULATION ***********************************/
		/* control simulation is used */
		if ((gMachSetup.mon.Info.ARsimUsed) && (!pInstInt->Edges.Pos_4)) {
			pInstInt->Edges.Pos_4 = 1;
			/* call function to init axis parameters */
			InitAxisPara();
		} else {
			if (!gMachSetup.mon.Info.ARsimUsed) pInstInt->Edges.Pos_4 = 0;
		}
	
		/* control simulation is not used */
		if ((!gMachSetup.mon.Info.ARsimUsed) && (pInstInt->Edges.Neg_0)) {
			pInstInt->Edges.Neg_0 = 0;
			/* call function to init axis parameters */
			InitAxisPara();
		} else {
			if (gMachSetup.mon.Info.ARsimUsed) pInstInt->Edges.Neg_0 = 1;
		}
	
		/********************************** ALARM MAPPING ********************************/
		/* control simulation is not used */
		if (!gMachSetup.mon.Info.ARsimUsed) {
		
		}
		/********************************* ALARM REACTION ********************************/
	
	
		/********************************** INPUT MAPPING ********************************/
	
		/**************************** INPUT SCALING AND MAPPING **************************/
	
		/****************************************************************
		* MACHINE CONTROL MODE SWITCH
		****************************************************************/
		switch(gMachControl.Mon.Mode) {
		
			/************************************************************************/
			/* Service mode */
			case machCTRL_SERVICE_MODE:
		
				/* init mode */
				if (pInstInt->MachineModeOld != gMachControl.Mon.Mode) {
					pInstInt->MachineModeOld = gMachControl.Mon.Mode;
					/* call switch mode reset function */
					SwitchModeReset();
				}
			
				break;
		
			/************************************************************************/
			/* Manual mode */
			case machCTRL_MANUAL_MODE:
		
				/* init mode */
				if (pInstInt->MachineModeOld != gMachControl.Mon.Mode) {
					pInstInt->MachineModeOld = gMachControl.Mon.Mode;
					/* call switch mode reset function */
					SwitchModeReset();
				}
			
				/* control commands */
				if (pIntf->Cmd.JogPosTable) {
					/* stations are in position */
					if (!pIntf->Mon.Info.StationInPos) {
						/* set block jog table */
						pIntf->Mon.BlockJogTable = 1;
						/* start timer */
						pInstInt->Timers.TON_1.IN = 1;
						/* set status */
						pIntf->Mon.Info.Status = tblCTRL_STATUS_ST_NOT_IN_POS;
					}
				} else if (pIntf->Cmd.JogNegTable) {
					/* stations are in position */
					if (!pIntf->Mon.Info.StationInPos) {
						/* set block jog table */
						pIntf->Mon.BlockJogTable = 1;
						/* start timer */
						pInstInt->Timers.TON_1.IN = 1;
						/* set status */
						pIntf->Mon.Info.Status = tblCTRL_STATUS_ST_NOT_IN_POS;
					}
				}
			
				break;
		
			/************************************************************************/
			/* Automat mode */
			case machCTRL_AUTOMAT_MODE:
		
				/* init mode */
				if (pInstInt->MachineModeOld != gMachControl.Mon.Mode) {
					pInstInt->MachineModeOld = gMachControl.Mon.Mode;
					/* call switch mode reset function */
					SwitchModeReset();
				}
			
				/* control commands */
				if (pIntf->Cmd.JogPosTable) {
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* start timer */
					pInstInt->Timers.TON_1.IN = 1;
					/* set status */
					pIntf->Mon.Info.Status = tblCTRL_STATUS_AUTOMAT_MODE;
				} else if (pIntf->Cmd.JogNegTable) {
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* start timer */
					pInstInt->Timers.TON_1.IN = 1;
					/* set status */
					pIntf->Mon.Info.Status = tblCTRL_STATUS_AUTOMAT_MODE;
				}
		
				break;
		
		} /* end of Machine Manager Mode switch */

		/********************************* GENERAL COMMANDS ******************************/
		/* control error reset command */
		if (pIntf->Cmd.ErrorReset) {
			pIntf->Cmd.ErrorReset = 0;
			/* reset output */
			pIntf->Mon.Error = 0;
			pIntf->Mon.StatusID = 0;
			memset(&pIntf->Mon.Info.Diag.Parts, 0, sizeof(pIntf->Mon.Info.Diag.Parts));
			memset(&pIntf->Mon.Info.Diag.LastErrorStates, 0, sizeof(pIntf->Mon.Info.Diag.LastErrorStates));
			pInst->DiagCounters.ErrorResetCnt++;
			/* reset mapp component */
			pInstInt->MpAxisBasic_0.ErrorReset = 1;
			pInstInt->MpAxisBasicConfig_0.ErrorReset = 1;
			/* reset function block */
			pInstInt->MCExtHwEndSw_0.ErrorReset = 1;
			/* set status */
			pIntf->Mon.Info.Status = tblCTRL_STATUS_IDLE;
		}

		/* control stop command */
		if (pIntf->Cmd.Stop) {
			pIntf->Cmd.Stop = 0;
			/* reset stop done */
			pIntf->Mon.StopDone = 0;
			/* control state of automat - go to the next state */
			if (pIntf->Mon.State != tblCTRL_ST_WAIT_CMD) pIntf->Mon.State = tblCTRL_ST_STOP;
			else pIntf->Mon.State = tblCTRL_ST_STOP;
		}
	
		/* control commands - reset block jog table */
		if ((!pIntf->Cmd.JogPosTable) && (!pIntf->Cmd.JogNegTable)) pIntf->Mon.BlockJogTable = 0;

		/****************************************************************
		* TURN TABLE CONTROL STATE SWITCH
		****************************************************************/
		switch(pIntf->Mon.State) {
		
			/************************************************************************/
			/* Waiting for commands */
			case tblCTRL_ST_WAIT_CMD:
			
				/* control commnads */
				if (pIntf->Cmd.TurnTable) {
					/* reset command */
					pIntf->Cmd.TurnTable = 0;
					/* set active command */
					pInstInt->ActiveCmd.TurnTable = 1;
					/* control power status - set active command */
					if (!pIntf->Mon.Info.PowerOn) pInstInt->ActiveCmd.Power = 1;
					/* control homed status - set active command */
					if (!pIntf->Mon.Info.IsHomed) pInstInt->ActiveCmd.Home = 1;
					/* control power on status - go to the next state */
					if (!pIntf->Mon.InitDone) pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
					else if (!pIntf->Mon.Info.PowerOn) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else if (!pIntf->Mon.Info.IsHomed) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else pIntf->Mon.State = tblCTRL_ST_READY_TURN_TABLE;
				} else if (pIntf->Cmd.Power) {
					/* reset command */
					pIntf->Cmd.Power = 0;
					/* set active command */
					pInstInt->ActiveCmd.Power = 1;
					/* control power on status - go to the next state */
					if (!pIntf->Mon.InitDone) pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
					else if (!pIntf->Mon.Info.PowerOn) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else pIntf->Mon.State = tblCTRL_ST_POWER_OFF_AXIS;
				} else if (pIntf->Cmd.Home) {
					/* reset command */
					pIntf->Cmd.Home = 0;
					/* set active command */
					pInstInt->ActiveCmd.Home = 1;
					/* control power status - set active command */
					if (!pIntf->Mon.Info.PowerOn) pInstInt->ActiveCmd.Power = 1;
					/* control power on status - go to the next state */
					if (!pIntf->Mon.InitDone) pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
					else if (!pIntf->Mon.Info.PowerOn) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
				} else if ((pIntf->Cmd.JogPosTable) && (!pIntf->Mon.BlockJogTable)) {
					/* control power status - set active command */
					if (!pIntf->Mon.Info.PowerOn) pInstInt->ActiveCmd.Power = 1;
					/* control homed status - set active command */
					if (!pIntf->Mon.Info.IsHomed) pInstInt->ActiveCmd.Home = 1;
					/* control power on status - go to the next state */
					if (!pIntf->Mon.InitDone) pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
					else if (!pIntf->Mon.Info.PowerOn) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else if (!pIntf->Mon.Info.IsHomed) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else pIntf->Mon.State = tblCTRL_ST_JOG_POS;
				} else if ((pIntf->Cmd.JogNegTable) && (!pIntf->Mon.BlockJogTable)) {
					/* control power status - set active command */
					if (!pIntf->Mon.Info.PowerOn) pInstInt->ActiveCmd.Power = 1;
					/* control homed status - set active command */
					if (!pIntf->Mon.Info.IsHomed) pInstInt->ActiveCmd.Home = 1;
					/* control power on status - go to the next state */
					if (!pIntf->Mon.InitDone) pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
					else if (!pIntf->Mon.Info.PowerOn) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else if (!pIntf->Mon.Info.IsHomed) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else pIntf->Mon.State = tblCTRL_ST_JOG_NEG;
				} else if (pIntf->Cmd.Init) {
					pIntf->Cmd.Init = 0;
					/* reset init done */
					pIntf->Mon.InitDone = 0;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_ACTIVE_ALM_SYS_W;
				}
				
				break;
		
			/************************************************************************/
			/* Wait for active alarm system */
			case tblCTRL_ST_ACTIVE_ALM_SYS_W:
			
				/* control alarm system is active */
				if (gAlarm.Mon.Info.AlarmSystemActive) {
					/* set init done */
					pIntf->Mon.InitDone = 1;
					/* control active command - go to the next state */
					if (pInstInt->ActiveCmd.Power) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else if (pInstInt->ActiveCmd.Home) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else if (pInstInt->ActiveCmd.TurnTable) pIntf->Mon.State = tblCTRL_ST_READY_TURN_TABLE;
					else if (pIntf->Cmd.JogPosTable) pIntf->Mon.State = tblCTRL_ST_JOG_POS;
					else if (pIntf->Cmd.JogNegTable) pIntf->Mon.State = tblCTRL_ST_JOG_NEG;
					else pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (gAlarm.Mon.Info.AlarmSystemError) {
					/* reset active command */
					ResetActiveCmd();
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Init hardware end switch */
			case tblCTRL_ST_INIT_HW_END_SW:
			
				/* control status FB */
				if (pInstInt->MCExtHwEndSw_0.Done) {
					/* set init done */
					pIntf->Mon.InitDone = 1;
					/* control active command - go to the next state */
					if (pInstInt->ActiveCmd.Power) pIntf->Mon.State = tblCTRL_ST_POWER_ON_AXIS;
					else if (pInstInt->ActiveCmd.Home) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else if (pInstInt->ActiveCmd.TurnTable) pIntf->Mon.State = tblCTRL_ST_READY_TURN_TABLE;
					else if (pIntf->Cmd.JogPosTable) pIntf->Mon.State = tblCTRL_ST_JOG_POS;
					else if (pIntf->Cmd.JogNegTable) pIntf->Mon.State = tblCTRL_ST_JOG_NEG;
					else pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MCExtHwEndSw_0.Error) {
					/* reset active command */
					ResetActiveCmd();
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Power on axis */
			case tblCTRL_ST_POWER_ON_AXIS:
			
				/* call FB */
				TON(&pInstInt->Timers.TON_0);
			
				/* start timer */
				pInstInt->Timers.TON_0.IN = 1;
				pInstInt->Timers.TON_0.PT = tblCTRL_POWER_ON_AXIS_TIME_OUT*1000;
			
				/* control axis is ready to power on */
				if (pInstInt->MpAxisBasic_0.Info.ReadyToPowerOn) {
					/* power on axis */
					pInstInt->MpAxisBasic_0.Power = 1;
				}
				/* set status */
				pIntf->Mon.Info.Status = tblCTRL_STATUS_POWER;
			
				/* control axis is power on */
				if (pInstInt->MpAxisBasic_0.PowerOn) {
					pInstInt->Timers.TON_0.IN = 0;
					/* reset acitve command */
					pInstInt->ActiveCmd.Power = 0;
					/* set command done */
					pIntf->Mon.PowerDone = 1;
					/* control active command - go to the next state */
					if (pInstInt->ActiveCmd.Home) pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					else if (pInstInt->ActiveCmd.TurnTable) pIntf->Mon.State = tblCTRL_ST_READY_TURN_TABLE;
					else if (pIntf->Cmd.JogPosTable) pIntf->Mon.State = tblCTRL_ST_JOG_POS;
					else if (pIntf->Cmd.JogNegTable) pIntf->Mon.State = tblCTRL_ST_JOG_NEG;
					else pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MpAxisBasic_0.Error) {
					pInstInt->Timers.TON_0.IN = 0;
					/* reset command */
					pInstInt->MpAxisBasic_0.Power = 0;
					/* reset active command */
					ResetActiveCmd();
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* call function to save and log StatusID */
					DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_POWER_ON_AXIS, 
						(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
						(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->Timers.TON_0.Q) {
					pInstInt->Timers.TON_0.IN = 0;
					/* reset command */
					pInstInt->MpAxisBasic_0.Power = 0;
					/* reset active command */
					ResetActiveCmd();
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* call function to save and log StatusID */
					DGDataSaveLogStatusID(&pInst->StatusIDConfig, tblCTRL_ERR_AXIS_NOT_READY_PWR, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Power off axis */
			case tblCTRL_ST_POWER_OFF_AXIS:
			
				/* power off axis */
				pInstInt->MpAxisBasic_0.Power = 0;
			
				/* control axis is power on */
				if (!pInstInt->MpAxisBasic_0.PowerOn) {
					/* reset acitve command */
					pInstInt->ActiveCmd.Power = 0;
					/* set command done */
					pIntf->Mon.PowerDone = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MpAxisBasic_0.Error) {
					/* reset active command */
					ResetActiveCmd();
					/* call function to save and log StatusID */
					DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_POWER_OFF_AXIS, 
						(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
						(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Home axis */
			case tblCTRL_ST_HOME_AXIS:
			
				/* control stations are in position */
				if (pIntf->Mon.Info.StationInPos) {
					/* call FB */
					TON(&pInstInt->Timers.TON_0);
			
					/* start timer */
					pInstInt->Timers.TON_0.IN = 1;
					pInstInt->Timers.TON_0.PT = tblCTRL_HOME_AXIS_TIME_OUT*1000;
			
					/* control homing mode - call function to init axis parameters */
					if ((!pInstInt->DirectHomeMode) && (pInstInt->MpAxisParameters.Home.Mode == mpAXIS_HOME_MODE_DIRECT)) InitAxisPara();
					/* control sensor offset */
					if (pIntfSetup->TurnTable.SensorOffset == 0) pIntfSetup->TurnTable.SensorOffset = tblCTRL_HOME_SENSOR_OFFSET;
					/* fill parameters */
					pInstInt->MpAxisParameters.Home.Position = 0;
					/* control direct home mode is active */
					if (pInstInt->DirectHomeMode) {
						/* control period */
						if (pInstInt->MpAxisConfig.Axis.PeriodSettings.Period == 0) pInstInt->MpAxisConfig.Axis.PeriodSettings.Period = tblCTRL_AXIS_PERIOD;
						/* set direct homing mode */
						pInstInt->MpAxisParameters.Home.Mode = mpAXIS_HOME_MODE_DIRECT;
						pInstInt->MpAxisParameters.Home.SensorOffset = 0;
					} else {
						/* set sensor offset */
						pInstInt->MpAxisParameters.Home.SensorOffset = pIntfSetup->TurnTable.SensorOffset;
					}
					/* home axis */
					pInstInt->MpAxisBasic_0.Home = 1;
					/* set status */
					pIntf->Mon.Info.Status = tblCTRL_STATUS_HOME;
			
					/* call mapp component */
					MpAxisBasic(&pInstInt->MpAxisBasic_0);
			
					/* control axis is homed */
					if ((pInstInt->MpAxisBasic_0.IsHomed) && (!pInstInt->DirectHomeMode)) {
						pInstInt->Timers.TON_0.IN = 0;
						/* reset command */
						pInstInt->MpAxisBasic_0.Home = 0;
						/* set variable */
						pInstInt->DirectHomeMode = 1;
						/* go to the next state */
						pIntf->Mon.State = tblCTRL_ST_HOME_AXIS;
					} else if ((pInstInt->MpAxisBasic_0.IsHomed) && (pInstInt->DirectHomeMode)) {
						pInstInt->Timers.TON_0.IN = 0;
						/* reset command */
						pInstInt->MpAxisBasic_0.Home = 0;
						/* reset acitve command */
						pInstInt->ActiveCmd.Home = 0;
						/* reset variable */
						pInstInt->DirectHomeMode = 0;
						/* set command done */
						pIntf->Mon.HomeDone = 1;
						/* call function to init axis parameters */
						InitAxisPara();
						/* control active command - go to the next state */
						if (pInstInt->ActiveCmd.TurnTable) pIntf->Mon.State = tblCTRL_ST_READY_TURN_TABLE;
						else if (pIntf->Cmd.JogPosTable) pIntf->Mon.State = tblCTRL_ST_JOG_POS;
						else if (pIntf->Cmd.JogNegTable) pIntf->Mon.State = tblCTRL_ST_JOG_NEG;
						else pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
					} else if (pInstInt->MpAxisBasic_0.Error) {
						pInstInt->Timers.TON_0.IN = 0;
						/* reset command */
						pInstInt->MpAxisBasic_0.Home = 0;
						/* reset active command */
						ResetActiveCmd();
						/* set block jog table */
						pIntf->Mon.BlockJogTable = 1;
						/* reset variable */
						pInstInt->DirectHomeMode = 0;
						/* call function to init axis parameters */
						InitAxisPara();
						/* call function to save and log StatusID */
						DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_HOME_AXIS, 
							(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
							(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
						/* go to the next state */
						pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
					} else if (pInstInt->Timers.TON_0.Q) {
						pInstInt->Timers.TON_0.IN = 0;
						/* reset command */
						pInstInt->MpAxisBasic_0.Home = 0;
						/* reset active command */
						ResetActiveCmd();
						/* set block jog table */
						pIntf->Mon.BlockJogTable = 1;
						/* reset variable */
						pInstInt->DirectHomeMode = 0;
						/* call function to init axis parameters */
						InitAxisPara();
						/* call function to save and log StatusID */
						DGDataSaveLogStatusID(&pInst->StatusIDConfig, tblCTRL_ERR_HOME_AXIS_TIME_OUT, 0, 0);
						/* go to the next state */
						pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
					}
				} else {
					/* reset active command */
					ResetActiveCmd();
					/* reset variable */
					pInstInt->DirectHomeMode = 0;
					/* call function to init axis parameters */
					InitAxisPara();
					/* call function to save and log StatusID */
					DGDataSaveLogStatusID(&pInst->StatusIDConfig, tblCTRL_ERR_STATION_NOT_IN_POS, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Control ready to turn table */
			case tblCTRL_ST_READY_TURN_TABLE:
			
				/* control stations are in position */
				if (pIntf->Mon.Info.StationInPos) {
					/* save out position status */
					pInstInt->OutPositionOld = pIntf->Mon.Info.OutPosition;
					/* reset turn table before in station */
					pInstInt->SlowBeforeInPosOld = 0;
					/* control active command go to the next state */
					if (pInstInt->ActiveCmd.TurnTable) pIntf->Mon.State = tblCTRL_ST_TURN_TABLE;
				} else {
					/* reset active command */
					ResetActiveCmd();
					/* call function to save and log StatusID */
					DGDataSaveLogStatusID(&pInst->StatusIDConfig, tblCTRL_ERR_STATION_NOT_IN_POS, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				}
			
				break;
		
			/************************************************************************/
			/* Turn table */
			case tblCTRL_ST_TURN_TABLE:
			
				/* fill parameters */
				pInstInt->MpAxisParameters.Acceleration = pIntfSetup->TurnTable.Acceleration;
				pInstInt->MpAxisParameters.Deceleration = pIntfSetup->TurnTable.Decelaration;
				pInstInt->MpAxisParameters.Direction = mpAXIS_DIR_POSITIVE;
				pInstInt->MpAxisParameters.Position = CalNextPosition();
				/* control turn table is before station */
				if (pInstInt->SlowBeforeInPos) pInstInt->MpAxisParameters.Velocity = pIntfRecipe->TurnTable.AutoVelocitySlow;
				else pInstInt->MpAxisParameters.Velocity = pIntfRecipe->TurnTable.AutoVelocity;
				/* control velocity */
				if (pInstInt->MpAxisParameters.Velocity == 0) pInstInt->MpAxisParameters.Velocity = tblCTRL_AUTO_VELOCITY;
				/* start movement */
				pInstInt->MpAxisBasic_0.MoveAbsolute = 1;
			
				/* control turn table is before station */
				if ((pInstInt->SlowBeforeInPos != pInstInt->SlowBeforeInPosOld) && (pInstInt->SlowBeforeInPosOld == 0)) {
					pInstInt->SlowBeforeInPosOld = pInstInt->SlowBeforeInPos;
					/* update axis parameters */
					pInstInt->MpAxisBasic_0.Update = 1;
				}
			
				/* call mapp component */
				MpAxisBasic(&pInstInt->MpAxisBasic_0);
			
				/* control status */
				if (pInstInt->MpAxisBasic_0.InPosition) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					pInstInt->MpAxisBasic_0.Update = 0;
					/* reset acitve command */
					pInstInt->ActiveCmd.TurnTable = 0;
					/* set command done */
					pIntf->Mon.TurnTableDone = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MpAxisBasic_0.Error) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					pInstInt->MpAxisBasic_0.Update = 0;
					/* reset active command */
					ResetActiveCmd();
					/* call function to save and log StatusID */
					DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_TURN_TABLE, 
						(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
						(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_STOP;
				} else if (pInstInt->MpAxisBasic_0.UpdateDone) {
					/* reset commands */
					pInstInt->MpAxisBasic_0.Update = 0;
				}
			
				break;
		
			/************************************************************************/
			/* Jog positive */
			case tblCTRL_ST_JOG_POS:
			
				/* fill parameters */
				pInstInt->MpAxisParameters.Acceleration = pIntfSetup->TurnTable.Acceleration;
				pInstInt->MpAxisParameters.Deceleration = pIntfSetup->TurnTable.Decelaration;
				pInstInt->MpAxisParameters.Velocity = pIntfSetup->TurnTable.ManualVelocity;
				pInstInt->MpAxisParameters.Direction = mpAXIS_DIR_POSITIVE;
				pInstInt->MpAxisParameters.Position = CalNextPosition();
				/* control velocity */
				if (pInstInt->MpAxisParameters.Velocity == 0) pInstInt->MpAxisParameters.Velocity = tblCTRL_MANUAL_VELOCITY;
				/* start movement */
				pInstInt->MpAxisBasic_0.MoveAbsolute = 1;
				/* set status */
				pIntf->Mon.Info.Status = tblCTRL_STATUS_JOG_POS;
			
				/* call mapp component */
				MpAxisBasic(&pInstInt->MpAxisBasic_0);
			
				/* control status */
				if (pInstInt->MpAxisBasic_0.InPosition) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MpAxisBasic_0.Error) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* call function to save and log StatusID */
					DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_JOG_POS, 
						(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
						(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_STOP;
				} else if ((!pIntf->Cmd.JogPosTable) || (pIntf->Mon.BlockJogTable)) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_STOP;
				}
			
				break;
		
			/************************************************************************/
			/* Jog negative */
			case tblCTRL_ST_JOG_NEG:
			
				/* fill parameters */
				pInstInt->MpAxisParameters.Acceleration = pIntfSetup->TurnTable.Acceleration;
				pInstInt->MpAxisParameters.Deceleration = pIntfSetup->TurnTable.Decelaration;
				pInstInt->MpAxisParameters.Velocity = pIntfSetup->TurnTable.ManualVelocity;
				pInstInt->MpAxisParameters.Direction = mpAXIS_DIR_NEGATIVE;
				pInstInt->MpAxisParameters.Position = CalPrevPosition();
				/* control velocity */
				if (pInstInt->MpAxisParameters.Velocity == 0) pInstInt->MpAxisParameters.Velocity = tblCTRL_MANUAL_VELOCITY;
				/* start movement */
				pInstInt->MpAxisBasic_0.MoveAbsolute = 1;
				/* set status */
				pIntf->Mon.Info.Status = tblCTRL_STATUS_JOG_NEG;
			
				/* call mapp component */
				MpAxisBasic(&pInstInt->MpAxisBasic_0);
			
				/* control status */
				if (pInstInt->MpAxisBasic_0.InPosition) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
				} else if (pInstInt->MpAxisBasic_0.Error) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* set block jog table */
					pIntf->Mon.BlockJogTable = 1;
					/* call function to save and log StatusID */
					DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_JOG_NEG, 
						(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
						(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_STOP;
				} else if ((!pIntf->Cmd.JogNegTable) || (pIntf->Mon.BlockJogTable)) {
					/* reset command */
					pInstInt->MpAxisBasic_0.MoveAbsolute = 0;
					/* go to the next state */
					pIntf->Mon.State = tblCTRL_ST_STOP;
				}
			
				break;
		
			/************************************************************************/
			/* Stop machine */
			case tblCTRL_ST_STOP:
		
				/* call function to stop */
				Stop();
				/* set stop done */
				pIntf->Mon.StopDone = 1;
		
				/* go to the next state */
				pIntf->Mon.State = tblCTRL_ST_WAIT_CMD;
			
				break;
			
		} /* end of Turn Table state switch */
		
		/******************************** MONITOR STRUCTURE ******************************/
		/* reset monitor structure when command case is left */
		if ((pIntf->Mon.State != tblCTRL_ST_WAIT_CMD) && (!pInstInt->Edges.Pos_2)) {
			pInstInt->Edges.Pos_2 = 1;
			/* reset monitor structure */
			pIntf->Mon.Error = 0;
			pIntf->Mon.StatusID = 0;
			pIntf->Mon.TurnTableDone = 0;
			pIntf->Mon.PowerDone = 0;
			pIntf->Mon.HomeDone = 0;
			pIntf->Mon.StopDone = 0;
			memset(&pIntf->Mon.Info.Diag.Parts, 0, sizeof(pIntf->Mon.Info.Diag.Parts));
			memset(&pIntf->Mon.Info.Diag.LastStates, 0, sizeof(pIntf->Mon.Info.Diag.LastStates));
			memset(&pIntf->Mon.Info.Diag.LastErrorStates, 0, sizeof(pIntf->Mon.Info.Diag.LastErrorStates));
			/* reset timer */
			pInstInt->Timers.TON_0.IN = 0;
			/* reset variables */
			pInstInt->DirectHomeMode = 0;
			/* set command is busy */
			pIntf->Mon.Busy = 1;
			/* set status */
			pIntf->Mon.Info.Status = tblCTRL_STATUS_ACTIVE;
		} else {
			if (pIntf->Mon.State == tblCTRL_ST_WAIT_CMD) pInstInt->Edges.Pos_2 = 0;
		}

		/* control state */
		if ((pIntf->Mon.State == tblCTRL_ST_WAIT_CMD) && (!pInstInt->Edges.Pos_3)) {
			pInstInt->Edges.Pos_3 = 1;
			/* reset timer */
			pInstInt->Timers.TON_0.IN = 0;
			/* control error - set status */
			if (pIntf->Mon.Error) pIntf->Mon.Info.Status = tblCTRL_STATUS_ERROR;
			else pIntf->Mon.Info.Status = tblCTRL_STATUS_IDLE;
		} else {
			if (pIntf->Mon.State != tblCTRL_ST_WAIT_CMD) pInstInt->Edges.Pos_3 = 0;
		}

		/* reset command is busy */
		if (pIntf->Mon.State == tblCTRL_ST_WAIT_CMD) pIntf->Mon.Busy = 0;

		/* control state change */
		if (pIntf->Mon.State != pIntf->Mon.Info.Diag.LastStates[0]) {
			/* shift the last state */
			memmove(&pIntf->Mon.Info.Diag.LastStates[1], &pIntf->Mon.Info.Diag.LastStates[0], (sizeof(pIntf->Mon.Info.Diag.LastStates)-sizeof(pIntf->Mon.Info.Diag.LastStates[0])));
			/* save last state */
			pIntf->Mon.Info.Diag.LastStates[0] = pIntf->Mon.State;
		}

		/***************************** GLOBAL MONITOR STRUCTURE ***************************/
		/* copy turn table power on */
		pIntf->Mon.Info.PowerOn = pInstInt->MpAxisBasic_0.PowerOn;
		/* copy turn table is homed */
		pIntf->Mon.Info.IsHomed = pInstInt->MpAxisBasic_0.IsHomed;
		/* copy move is active */
		pIntf->Mon.Info.MoveActive = pInstInt->MpAxisBasic_0.MoveActive;
		/* copy turn table position */
		pIntf->Mon.Info.Position = pInstInt->MpAxisBasic_0.Position;
		/* copy turn table velocity */
		pIntf->Mon.Info.Velocity = pInstInt->MpAxisBasic_0.Velocity;
		/* set turn table is in position */
		pIntf->Mon.Info.InPosition = CalInPosition();
		/* set turn table is out position */
		pIntf->Mon.Info.OutPosition = CalOutPosition();
	
		/* set status timer */
		pInstInt->Timers.TON_1.PT = statCTRL_STATUS_TIME*1000;
		/* call FB */
		TON(&pInstInt->Timers.TON_1);
		/* control timer */
		if (pInstInt->Timers.TON_1.Q) {
			pInstInt->Timers.TON_1.IN = 0;
			/* set status */
			pIntf->Mon.Info.Status = tblCTRL_STATUS_IDLE;
		}
	
		/* control error */
		if (pIntf->Mon.Error) {
			/* call FB */
			TON(&pInstInt->Timers.TON_2);
			/* start timer */
			pInstInt->Timers.TON_2.IN = 1;
			pInstInt->Timers.TON_2.PT = statCTRL_ERR_BLINK_TIME*1000;
			/* control timer */
			if (pInstInt->Timers.TON_2.Q) {
				pInstInt->Timers.TON_2.IN = 0;
				/* change value of error blink */
				pIntf->Mon.Info.ErrorBlink = !pIntf->Mon.Info.ErrorBlink;
			}
		} else {
			/* reset timer */
			pInstInt->Timers.TON_2.IN = 0;
			/* reset error blink */
			pIntf->Mon.Info.ErrorBlink = 0;
		}
	
		/**************************** OUTPUT SCALING AND MAPPING **************************/
	
		/************************ MOTION CONTROL EXTENSION SETTINGS ***********************/
		/* fill parameters */
		//pInstInt->MCExtHwEndSw_0.Enable = 1;
		pInstInt->MCExtHwEndSw_0.AutoResetCmd = 1;
		pInstInt->MCExtHwEndSw_0.Axis = (UDINT)&gAxis01;
		pInstInt->MCExtHwEndSw_0.pConfiguration = &pInstInt->MCExtHwEndSwConfig;
	
		/* control error */
		if (pInstInt->MCExtHwEndSw_0.StatusID == ERR_OK) pInstInt->MCExtHwEndSw_0.ErrorReset = 0;
	
		/* call FB */
		//MCExtHwEndSw(&pInstInt->MCExtHwEndSw_0);
	
		/* control error */
		if ((pInstInt->MCExtHwEndSw_0.Error) && (!pInstInt->Edges.Pos_9)) {
			pInstInt->Edges.Pos_9 = 1;
			/* call function to save and log StatusID */
			DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_INIT_HW_END_SW, 
				(DGDataStatusIDType*)&pInstInt->MCExtHwEndSw_0.Info.Diag.StatusID, 
				(DGDataInternalIDType*)&pInstInt->MCExtHwEndSw_0.Info.Diag.Internal, 0, 0);
		} else {
			if (!pInstInt->MCExtHwEndSw_0.Error) pInstInt->Edges.Pos_9 = 0;
		}
	
		/************************** MAPP COMPONENT MOTION SETTINGS *************************/
		/* fill parameters */
		pInstInt->MpAxisBasic_0.MpLink = &gAxisBasicLinkTable;
		pInstInt->MpAxisBasic_0.Enable = 1;
		pInstInt->MpAxisBasic_0.Parameters = &pInstInt->MpAxisParameters;
		pInstInt->MpAxisBasic_0.Axis = (UDINT)&gAxis01;
	
		/* control error */
		if (pInstInt->MpAxisBasic_0.StatusID == ERR_OK) pInstInt->MpAxisBasic_0.ErrorReset = 0;
	
		/* call mapp component */
		MpAxisBasic(&pInstInt->MpAxisBasic_0);
	
		/* control axis is stopped */
		if ((pInstInt->MpAxisBasic_0.Stopped) || (pInstInt->MpAxisBasic_0.Info.PLCopenState == mpAXIS_DISABLED) 
			|| (pInstInt->MpAxisBasic_0.Info.PLCopenState == mpAXIS_STANDSTILL)) pInstInt->MpAxisBasic_0.Stop = 0;
	
		/* control power on status - power off the axis */
		if (((pInstInt->MpAxisBasic_0.Info.PLCopenState == mpAXIS_DISABLED) || (!pInstInt->MpAxisBasic_0.PowerOn))
			&& (pInstInt->MpAxisBasic_0.Power) && (pInstInt->MpAxisPowerOnChange)) pInstInt->MpAxisBasic_0.Power = 0;
		/* control power on change */
		if (pInstInt->MpAxisPowerOnChange != pInstInt->MpAxisBasic_0.PowerOn) pInstInt->MpAxisPowerOnChange = pInstInt->MpAxisBasic_0.PowerOn;
	
		/* control error */
		if ((pInstInt->MpAxisBasic_0.Error) && (!pInstInt->MpAxisBasic_0.ErrorReset)) {
			/* call function to reset axis commands */
			ResetAxisCmd();
		}
	
		/* control error */
		if ((pInstInt->MpAxisBasic_0.Error) && (!pInstInt->Edges.Pos_6)) {
			pInstInt->Edges.Pos_6 = 1;
			/* call function to save and log StatusID */
			DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_MAPP, 
				(DGDataStatusIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.StatusID, 
				(DGDataInternalIDType*)&pInstInt->MpAxisBasic_0.Info.Diag.Internal, 0, 0);
		} else {
			if (!pInstInt->MpAxisBasic_0.Error) pInstInt->Edges.Pos_6 = 0;
		}
	
		/* fill parameters */
		pInstInt->MpAxisBasicConfig_0.MpLink = &gAxisBasicLinkTable;
		pInstInt->MpAxisBasicConfig_0.Enable = 1;
		pInstInt->MpAxisBasicConfig_0.Configuration = &pInstInt->MpAxisConfig;
		pInstInt->MpAxisBasicConfig_0.Save = pIntf->Cmd.SaveConfig;
		pInstInt->MpAxisBasicConfig_0.Load = pIntf->Cmd.LoadConfig;
	
		/* control error */
		if (pInstInt->MpAxisBasicConfig_0.StatusID == ERR_OK) pInstInt->MpAxisBasicConfig_0.ErrorReset = 0;
	
		/* call mapp component */
		MpAxisBasicConfig(&pInstInt->MpAxisBasicConfig_0);
	
		/* control command done or error */
		if ((pInstInt->MpAxisBasicConfig_0.CommandDone) || (pInstInt->MpAxisBasicConfig_0.Error)) {
			/* reset commands */
			pIntf->Cmd.SaveConfig = 0;
			pIntf->Cmd.LoadConfig = 0;
		}
	
		/* control error */
		if ((pInstInt->MpAxisBasicConfig_0.Error) && (!pInstInt->Edges.Pos_5)) {
			pInstInt->Edges.Pos_5 = 1;
			/* call function to save and log StatusID */
			DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_MAPP, 
				(DGDataStatusIDType*)&pInstInt->MpAxisBasicConfig_0.Info.Diag.StatusID, 0, 0, 0);
		} else {
			if (!pInstInt->MpAxisBasicConfig_0.Error) pInstInt->Edges.Pos_5 = 0;
		}
	
		/***************************** MAPP COMPONENT SETTINGS ****************************/
		/* setup MpRecipeRegPar component */
		pInstInt->MpRecipeRegPar_0.MpLink = &gMachSetupLink;
		pInstInt->MpRecipeRegPar_0.Enable = 1;
		pInstInt->MpRecipeRegPar_0.PVName = (STRING *)&"gTableControlSetup";
		pInstInt->MpRecipeRegPar_0.Category = (STRING *)&pInst->SetupCategory;
		pInstInt->MpRecipeRegPar_0.ErrorReset = gMachSetup.mon.Info.ErrorResetPulse;

		/* control factory settings */
		if ((gMachSetup.mon.Info.FactSetPulse) && (!pInstInt->Edges.Pos_0)) {
			pInstInt->Edges.Pos_0 = 1;
			FactorySettings();
			DefaultRecipe();
		} else {
			if (!gMachSetup.mon.Info.FactSetPulse) pInstInt->Edges.Pos_0 = 0;
		}

		/* calling mapp component */
		MpRecipeRegPar(&pInstInt->MpRecipeRegPar_0);
	
		/* control error */
		if ((pInstInt->MpRecipeRegPar_0.Error) && (!pInstInt->Edges.Pos_7)) {
			pInstInt->Edges.Pos_7 = 1;
			/* call function to save and log StatusID */
			DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_MAPP, 
				(DGDataStatusIDType*)&pInstInt->MpRecipeRegPar_0.Info.Diag.StatusID, 0, 0, 0);
		} else {
			if (!pInstInt->MpRecipeRegPar_0.Error) pInstInt->Edges.Pos_7 = 0;
		}
	
		/***************************** MAPP COMPONENT SETTINGS ****************************/
		/* setup MpRecipeRegPar component */
		pInstInt->MpRecipeRegPar_1.MpLink = &gMachRecipeLink;
		pInstInt->MpRecipeRegPar_1.Enable = 1;
		pInstInt->MpRecipeRegPar_1.PVName = (STRING *)&"gTableControlRecipe";
		pInstInt->MpRecipeRegPar_1.Category = (STRING *)&pInst->RecipeCategory;
		pInstInt->MpRecipeRegPar_1.ErrorReset = gMachRecipe.mon.Info.ErrorResetPulse;

		/* calling mapp component */
		MpRecipeRegPar(&pInstInt->MpRecipeRegPar_1);
	
		/* control error */
		if ((pInstInt->MpRecipeRegPar_1.Error) && (!pInstInt->Edges.Pos_8)) {
			pInstInt->Edges.Pos_8 = 1;
			/* call function to save and log StatusID */
			DGDataSaveLogStatusIDStruct(&pInst->StatusIDConfig, tblCTRL_ERR_MAPP, 
				(DGDataStatusIDType*)&pInstInt->MpRecipeRegPar_1.Info.Diag.StatusID, 0, 0, 0);
		} else {
			if (!pInstInt->MpRecipeRegPar_1.Error) pInstInt->Edges.Pos_8 = 0;
		}
	
		/*************************************** ALARMS ***********************************/
		/* call function to set the alarm */
		SetAlarmConfig();
	
		/************************************* TESTING ************************************/
		if (test == 1) {
			test = 0;
		}
		
	} /* end of control pointer if statement */
	
}
