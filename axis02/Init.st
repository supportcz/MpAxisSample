PROGRAM _INIT
	
	(* change following 3 lines for a new axis *)
	adrAxis02 := ADR(interface); 
	interface.MpAxisBasic_0.MpLink := ADR(gAxisBasic02); 
	interface.MpAxisBasic_0.Axis := ADR(gAxis02); 
	
END_PROGRAM