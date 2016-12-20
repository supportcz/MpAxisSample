
PROGRAM _INIT
	 
END_PROGRAM

PROGRAM _CYCLIC
	
	CASE stateAxis OF
		
		stateAxisInit:
			
			IF powerOn THEN
				stav := 10;
				MpAxisBasic_0.Enable := TRUE;
			END_IF;	
		
		stateAxisWaitForPowerOn:	
			
			IF MpAxisBasic_0.Info.ReadyToPowerOn THEN
				MpAxisBasic_0.Power := TRUE;
			END_IF

			IF MpAxisBasic_0.PowerOn THEN
				stateAxis := stateAxisWait;
			END_IF	
		
		stateAxisWait:
			
			Parametry1.Home.Mode := mpAXIS_HOME_MODE_ABSOLUTE;
			Parametry1.Home.Position := -315;
				MpAxisBasic_0.Home := TRUE;
				stav := 30;
			ELSIF MpAxisBasic_0.Error THEN
				stav := 100;
			END_IF;
		
		30: IF MpAxisBasic_0.IsHomed THEN
				MpAxisBasic_0.Home := FALSE;
				stav := 50;
			ELSIF MpAxisBasic_0.Error THEN
				stav := 100;
			END_IF;
		
		50: //stav cekani na prikaz
			IF cmdJogPos OR cmdJogNeg THEN
				stav := 60;
			ELSIF cmdAbsMove THEN
				stav := 70;
			ELSIF MpAxisBasic_0.Error THEN
				stav := 100;	
			END_IF
			
		60: //jogy			
			IF cmdJogPos THEN
				Parametry1.Jog.Velocity := 360;
				MpAxisBasic_0.JogPositive := TRUE;
			ELSIF cmdJogNeg THEN
				Parametry1.Jog.Velocity := 360;
				MpAxisBasic_0.JogNegative := TRUE;
			ELSIF MpAxisBasic_0.Error THEN
				stav := 100;	
			ELSE
				MpAxisBasic_0.JogPositive := 
				MpAxisBasic_0.JogNegative := FALSE;
				stav := 50;
			END_IF;
		
		70: //najezd na 0
			Parametry1.Position := 0;
			Parametry1.Direction := mpAXIS_DIR_SHORTEST_WAY;
			MpAxisBasic_0.MoveAbsolute := TRUE;
			stav := 75;
		
		75:	//		
			IF MpAxisBasic_0.InPosition THEN
				MpAxisBasic_0.MoveAbsolute := FALSE;
				stav := 50;
			ELSIF MpAxisBasic_0.Error THEN
				stav := 100;	
			ELSE
				

			END_IF;
		
		100: // error stav
			IF cmdResetError THEN
				MpAxisBasic_0.ErrorReset := TRUE;
			ELSE
				MpAxisBasic_0.ErrorReset := FALSE;
			END_IF;
		

			IF NOT(MpAxisBasic_0.Error) THEN
				cmdResetError := FALSE;
				MpAxisBasic_0.ErrorReset := FALSE;
				stav := 0;
			END_IF;
		
	END_CASE;
	 
	MpAxisBasic_0();
	
	
END_PROGRAM

PROGRAM _EXIT
	MpAxisBasic_0(Enable := FALSE);
	 
END_PROGRAM

