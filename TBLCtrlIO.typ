
TYPE
	TBLCtrlIOType : 	STRUCT  (*Interface control IO type*)
		DI : TBLCtrlIODigitalInputsType; (*Digital inputs*)
	END_STRUCT;
	TBLCtrlIODigitalInputsType : 	STRUCT  (*Interface control IO digital inputs type*)
		HomeSwitch : BOOL; (*Home switch*)
	END_STRUCT;
END_TYPE
