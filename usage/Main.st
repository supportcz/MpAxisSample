
PROGRAM _INIT
	 
END_PROGRAM

PROGRAM _CYCLIC
	
	IF adrAxis01 = 0 THEN
		RETURN;
	END_IF
	
	axis01 ACCESS adrAxis01;
	
	CASE step OF
		0:
			IF go THEN
				step := step + 1;
			END_IF
		1:
			axis01.power := TRUE;
			
			IF axis01.MpAxisBasic_0.PowerOn THEN
				step := step + 1;
			END_IF
	
		2:
			axis01.parameters.Position := 100;
			axis01.MpAxisBasic_0.MoveAbsolute := TRUE;
		
			IF axis01.MpAxisBasic_0.InPosition THEN
				step := step + 1;
			END_IF

		3:
			axis01.parameters.Position := 200;
			axis01.MpAxisBasic_0.MoveAbsolute := TRUE;
		
			IF axis01.MpAxisBasic_0.InPosition THEN
				step := 2;
			END_IF


	END_CASE;	
	 
END_PROGRAM

PROGRAM _EXIT
	 
END_PROGRAM

