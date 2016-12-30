
TYPE
	TBLCtrlIntfSetupType : 	STRUCT  (*Interface control setup type*)
		MemIndct : UINT; (*Memory indication*)
		TurnTable : TBLCtrlInfSetupTurnTableType; (*Turn table structure*)
	END_STRUCT;
	TBLCtrlInfSetupTurnTableType : 	STRUCT  (*Turn table control setup type*)
		Acceleration : REAL; (*Acceleration of the movement [mm/s2]*)
		Decelaration : REAL; (*Deceleration of the movement [mm/s2]*)
		ManualVelocity : REAL; (*Manual velocity of the movement [mm/s]*)
		SensorOffset : LREAL; (*Sensor offset [mm]*)
		StationDistance : LREAL; (*Station distance [mm]*)
	END_STRUCT;
END_TYPE
