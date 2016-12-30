
TYPE
	TBLCtrlIntfRecipeType : 	STRUCT  (*Interface control recipe type*)
		MemIndct : UINT; (*Memory indication*)
		TurnTable : TBLCtrlInfRecipeTurnTableType; (*Turn table structure*)
	END_STRUCT;
	TBLCtrlInfRecipeTurnTableType : 	STRUCT  (*Turn table control recipe type*)
		AutoVelocity : REAL; (*Auto velocity of the movement [mm/s]*)
		AutoVelocitySlow : REAL; (*Slow auto velocity of the movement [mm/s]*)
		DistanceNotInPosition : LREAL; (*Distance turn table before in position [mm]*)
		ClipsWidth : LREAL; (*Clips width [mm]*)
	END_STRUCT;
END_TYPE
