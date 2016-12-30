
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _EXIT ProgramExit(void)
{
		
	/******************************** FILL THE POINTERS *******************************/
	/* fill pointers */
	pInstInt = &TableControl.Internal;
	
	/* control pointers */
	if (pInstInt != 0) {	
		/* reset enable MpAxisBasic component */
		pInstInt->MpAxisBasic_0.Enable = 0;
		/* calling mapp component */
		MpAxisBasic(&pInstInt->MpAxisBasic_0);
		/* reset enable MpAxisBasicConfig component */
		pInstInt->MpAxisBasicConfig_0.Enable = 0;
		/* calling mapp component */
		MpAxisBasicConfig(&pInstInt->MpAxisBasicConfig_0);
		/* reset enable MpRecipeRegPar component */
		pInstInt->MpRecipeRegPar_0.Enable = 0;
		/* calling mapp component */
		MpRecipeRegPar(&pInstInt->MpRecipeRegPar_0);
		/* reset enable MpRecipeRegPar component */
		pInstInt->MpRecipeRegPar_1.Enable = 0;
		/* calling mapp component */
		MpRecipeRegPar(&pInstInt->MpRecipeRegPar_1);
	}
	
}
