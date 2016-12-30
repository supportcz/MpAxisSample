
PROGRAM _INIT
	
	interface.MpAxisBasic_0.MpLink := ADR(gAxisBasic);
	interface.MpAxisBasic_0.Enable := TRUE;
	interface.MpAxisBasic_0.Parameters := ADR(interface.parameters);
	interface.MpAxisBasic_0.Axis := ADR(gAxis01);
	
	adrAxis01 := ADR(interface);
	 
END_PROGRAM

PROGRAM _CYCLIC
	
	interface.MpAxisBasic_0.Power := interface.power AND interface.MpAxisBasic_0.Info.ReadyToPowerOn;
	
	interface.MpAxisBasic_0.ErrorReset := interface.errorReset;
	interface.errorReset := FALSE;
	
	interface.MpAxisBasic_0.Update := interface.update;
	interface.update := FALSE;
	
	IF interface.MpAxisBasic_0.Info.PLCopenState = mpAXIS_ERRORSTOP OR interface.MpAxisBasic_0.Info.PLCopenState = mpAXIS_STOPPING THEN
		interface.MpAxisBasic_0.MoveVelocity := FALSE;
		interface.MpAxisBasic_0.MoveAbsolute := FALSE;
		interface.MpAxisBasic_0.MoveAdditive := FALSE;
		interface.MpAxisBasic_0.JogPositive := FALSE;
		interface.MpAxisBasic_0.JogNegative := FALSE;
	END_IF
		
	interface.MpAxisBasic_0();
	
	IF interface.MpAxisBasic_0.InPosition THEN
		interface.MpAxisBasic_0.MoveAbsolute := FALSE;
		interface.MpAxisBasic_0.MoveAdditive := FALSE;
	END_IF
	
	interface.MpAxisBasic_0.Stop := FALSE;

END_PROGRAM

PROGRAM _EXIT
	interface.MpAxisBasic_0(Enable := FALSE);
	 
END_PROGRAM

