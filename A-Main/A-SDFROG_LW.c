//==============================================================================
//
// Title:		User Interface Application
// Purpose:		A short description of the application.
//
// Created on:	01.10.2015 at 15:40:25 by Admin.
// Copyright:	Microsoft. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>

#include "toolbox.h"



// graphic interfaces
#include "A-SDFROG_LW.h"  
#include "B-MotorPI.h"
#include "D-CCD_camera.h" 

//files for share variable
#include "Z-variable.h" 

// files PI
#include "PI_GCS2_DLL.h"
#include "B-Function_MotorPIC863.h"

// files CCD

#include "DCamUSB.h"
#include "D-Function_CCD_camera.h"

//#include "C-Function_Frog_acq.h" 

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions





/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{

SetStdioWindowOptions(10000, 0, 0);
	
	/* initialize and load resources */
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	
	
if ((A_panelHandle = LoadPanel (0, "A-SDFROG_LW.uir", APANEL)) < 0)
		return -1;
if ((B_panelHandle = LoadPanel (0, "B-MotorPI.uir", BPANEL)) < 0)
		return -1;
if ((D_panelHandle = LoadPanel (0, "D-CCD_camera.uir", DPANEL)) < 0)
		return -1;


	
	
	
	/* display the panel and run the user interface */
DisplayPanel (A_panelHandle);
DisplayPanel (B_panelHandle);  	
DisplayPanel (D_panelHandle); 	

init_motor(); 
USB_init();   // for CCD initialisation
init_hardware();

RunUserInterface ();

	// initialisation du moteur


	// close user interfaces on exit
	if (A_panelHandle)
		DiscardPanel (A_panelHandle);	
	if (B_panelHandle)
		DiscardPanel (B_panelHandle);
	if (D_panelHandle)
		DiscardPanel (D_panelHandle);
	
	
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			
			close_motor();
			close_CCD();
			
		   QuitUserInterface (0);
			break;
	}
	return 0;
}


