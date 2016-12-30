
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

/* function declaration */
void SaveLogStatusIDConfig(struct DGDataSaveLogStatusIDConfigType*, BOOL);
void FactorySettings();
void DefaultRecipe();
void InitAxisPara();

void _INIT ProgramInit(void)
{
	
	/******************************** FILL THE POINTERS *******************************/
	/* fill pointers */
	pIntf = &gTableControl;
	pIntfSetup = &gTableControlSetup;
	pIntfRecipe = &gTableControlRecipe;
	pInst = &TableControl;
	pInstInt = &TableControl.Internal;
	
	/* control pointers */
	if ((pIntf != 0) && (pIntfSetup != 0) && (pIntfRecipe != 0) && (pInst != 0) && (pInstInt != 0)) {	
		/* control memory is lost of throw out control setup */
		if (pIntfSetup->MemIndct != tblCTRL_MEM_INDCT_VALUE) {
			pIntfSetup->MemIndct = tblCTRL_MEM_INDCT_VALUE;
			/* memory is lost - call factory settings */
			FactorySettings();
		}
		/* control memory is lost of throw out control setup */
		if (pIntfRecipe->MemIndct != tblCTRL_MEM_INDCT_VALUE) {
			pIntfRecipe->MemIndct = tblCTRL_MEM_INDCT_VALUE;
			/* memory is lost - call default recipe */
			DefaultRecipe();
		}
	
		/* fill the setup category */
		strcpy(pInst->SetupCategory, "MachineSetup");
		/* fill the setup category */
		strcpy(pInst->RecipeCategory, "MachineRecipe");
	
		/* call function to init axis parameters */
		InitAxisPara();
	
		/* load configuration */
		pIntf->Cmd.LoadConfig = 1;
	
		/* call function to config Save Log StatusID */
		SaveLogStatusIDConfig(&pInst->StatusIDConfig, 1);
	
		/* set init command */
		pIntf->Cmd.Init = 1;
	}
	
	/* dummy use of variable */
	if (gTableControlIO.DI.HomeSwitch) DummyVar = 0;
	
}
