
TYPE
	TBLCtrIntfErrorEnum : 
		( (*Interface control error enumeration*)
		tblCTRL_NO_ERROR := 0, (*No error*)
		tblCTRL_ERR_GENERAL := -535854672, (*Interface control general error. [Error: 33200]*)
		tblCTRL_ERR_POWER_ON_AXIS := -535854671, (*Power on axis, MpAxisBasic has error. [Error: 33201]*)
		tblCTRL_ERR_AXIS_NOT_READY_PWR := -535854670, (*Axis is not ready to power on. [Error: 33202]*)
		tblCTRL_ERR_HOME_AXIS := -535854669, (*Home of axis, MpAxisBasic has error. [Error: 33203]*)
		tblCTRL_ERR_HOME_AXIS_TIME_OUT := -535854668, (*Axis homing time out. [Error: 33204]*)
		tblCTRL_ERR_TURN_TABLE := -535854667, (*Turn table to next position error, MpAxisBasic has error. [Error: 33205]*)
		tblCTRL_ERR_POWER_OFF_AXIS := -535854666, (*Power on axis, MpAxisBasic has error. [Error: 33206]*)
		tblCTRL_ERR_JOG_POS := -535854665, (*Jog positive, MpAxisBasic has error. [Error: 33207]*)
		tblCTRL_ERR_JOG_NEG := -535854664, (*Jog negative, MpAxisBasic has error. [Error: 33208]*)
		tblCTRL_ERR_STATION_NOT_IN_POS := -535854663, (*Table is not ready to turn, station are not in position. [Error: 33209]*)
		tblCTRL_ERR_MAPP := -535854662, (*Mapp components have error. [Error: 33210]*)
		tblCTRL_ERR_TURN_TABLE_MIDDLE := -535854661, (*Turn table to middle position error, MpAxisBasic has error. [Error: 33211]*)
		tblCTRL_ERR_INIT_HW_END_SW := -535854660 (*Init hardware end switch error, MCExtHwEndSw has error. [Error: 33212]*)
		);
END_TYPE
