PROGRAM _INIT
	
	(* change following 3 lines for a new axis *)
	adrAxis01 := ADR(interface); //change "adrAxis01"
	interface.MpAxisBasic_0.MpLink := ADR(gAxisBasic01); //change "gAxisBasic01"
	interface.MpAxisBasic_0.Axis := ADR(gAxis01); //change "gAxis01"
	
END_PROGRAM