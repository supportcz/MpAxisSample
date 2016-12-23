
PROGRAM _INIT
	
	interface.MpAxisBasic_0.MpLink := ADR(gAxisBasic);
	interface.MpAxisBasic_0.Enable := TRUE;
	interface.MpAxisBasic_0.Parameters := ADR(interface.parameters);
	interface.MpAxisBasic_0.Axis := ADR(gAxis01);
	
	adrAxis01 := ADR(interface);
	 
END_PROGRAM

PROGRAM _CYCLIC
	
	interface.MpAxisBasic_0.Power := interface.powerOn;
		 
	interface.MpAxisBasic_0();
	
	
END_PROGRAM

PROGRAM _EXIT
	interface.MpAxisBasic_0(Enable := FALSE);
	 
END_PROGRAM

