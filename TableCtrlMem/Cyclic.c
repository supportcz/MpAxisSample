
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC ProgramCyclic(void)
{

	/* use remanent variable at different task to prevent initialization variable during tranfering the task */
	if (gTableControlSetup.MemIndct != tblCTRL_MEM_INDCT_VALUE) {
		gTableControlSetup.MemIndct = tblCTRL_MEM_INDCT_VALUE;
	}
	/* use remanent variable at different task to prevent initialization variable during tranfering the task */
	if (gTableControlRecipe.MemIndct != tblCTRL_MEM_INDCT_VALUE) {
		gTableControlRecipe.MemIndct = tblCTRL_MEM_INDCT_VALUE;
	}
	
}
