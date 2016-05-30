// INCLUDE PANEL 

#include "B-MotorPI.h"  
// INCLUDE PROTOTYPES
#include "B-Function_MotorPIC863.h" 

//==============================================================================
//
// Title:		Motor PI C863 
// Purpose:		A short description of the application.

//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "A-SDFROG_LW.h"
#include "PI_GCS2_DLL.h" 
#include "toolbox.h"
#include <formatio.h> 

//files for share variable
#include "Z-variable.h"


//==============================================================================
// Constants

//==============================================================================
// Static global variables
int iBufferSize =100;



//==============================================================================
// Types
int ID_new_obj, Error_number, piControllerReady=0;

double Home_position;

const char szDescription[100];
char sz_Axes[100];

const BOOL pbValueArray=1;
double step_size;

double  pdValueArray[3];

double pdStepSize;



FILE *File_MotorPosition;

//==============================================================================
//* Motor functions
//===============================================================================
PI_stepsize(void)
{
	int status;
	
//	PI_STE (ID_new_obj,"1", &pdStepSize);
//	PI_Check_Moving();  
//	PI_qSST (ID_new_obj,"1", &pdStepSize);
	
	

//	SetCtrlVal (B_panelHandle,BPANEL_step_size_indicator,pdStepSize); 
return 0;
}

PI_Position(void)
{
PI_qPOS(ID_new_obj,"1",&Current_pos_value);

printf("Current position = %f\n",Current_pos_value);

SetCtrlVal (B_panelHandle,BPANEL_Current_pos_value,Current_pos_value);
return 0;
}

PI_Check_Moving()
{

  PI_IsMoving(ID_new_obj,"1",&pbValueArray);
  
  while(pbValueArray==1)
	{
     PI_IsMoving(ID_new_obj,"1",&pbValueArray);
  	}
	
return 0;
}

//* Boucle de control pour vérifier si le moteur PI est prêt*///////////////////////////////
/////////////////////////////////////////////////////////////
PI_Control(void)
{
	PI_IsControllerReady (ID_new_obj, &piControllerReady);
			
	while(piControllerReady==0)
			{
				PI_IsControllerReady (ID_new_obj, &piControllerReady);
				printf("piControllerReady %i\n", piControllerReady); 
			}
return 0; 
}

PI_move_motor(double actual_pos_to_go)
{
	
PI_MOV(ID_new_obj,"1", &actual_pos_to_go);  // MVE permet de stopper le mouvement alors que MOV non . This command can be interrupted by PI_STP() and PI_HLT().

// Boucle de control pour vérifier si le moteur PI est prêt
			PI_Check_Moving();
			PI_Position();	
	
return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////


init_motor(void)
{

	//int position_at_start;
	
	int status;
	char szBuffer[iBufferSize];
	const char szFilter[iBufferSize];
	
	strcpy(szBuffer,""); 
	strcpy(szFilter,"C-863"); 
	
	
	status=PI_EnumerateUSB(szBuffer, iBufferSize, szFilter);
	strcpy(szDescription, szBuffer);
//	printf("number of controllers connected to the computer = %i\n",status);
//	printf("szDescription = %s\n",szDescription);
	
	ID_new_obj=PI_ConnectUSB (szDescription);
	printf("ID_new_obj = %i\n",ID_new_obj);
	

	PI_qDFH(ID_new_obj,"1",&Home_position);
	SetCtrlVal (B_panelHandle,BPANEL_Home_value,Home_position);
	
	PI_qSAI(ID_new_obj,sz_Axes,iBufferSize);
//	printf("sz_Axes = %s\n",sz_Axes);
	
	status=PI_SVO (ID_new_obj,"1", &pbValueArray);
    printf("status mode = %i\n",status); 
	
	PI_Check_Moving();
	PI_Position();
	
	// write actual position
//	position_at_start = PI_qPOS(long ID, const char* szAxes, double* pdValueArray);
	//showPos(position_at_start);	// actually just SHOWS the position...
	//printf("%i", position_at_start);

	return 0;
}

close_motor(void)
{

	PI_CloseConnection (ID_new_obj);	
	
return 0;
}

//==============================================================================
//* Scan functions
//===============================================================================
Get_Parameters_motor(void)
{
   GetCtrlVal (B_panelHandle,BPANEL_Start_pos,&Start_pos_value);     
   GetCtrlVal (B_panelHandle,BPANEL_Step_nbr,&Step_nbr_value);     
   GetCtrlVal (B_panelHandle,BPANEL_Stop_pos,&Stop_pos_value);     

return 0;
}




//============================================================================== 
//* FUNCTION CALLBACK
//=============================================================================== 
int CVICALLBACK Callback_Get_error (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			Error_number=PI_GetError(ID_new_obj);
			//printf("%i\n", Error_number); 
			SetCtrlVal (B_panelHandle,BPANEL_Indic_Error,Error_number);
			
			break;
	}
	return 0;
}

int CVICALLBACK Callback_Home (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{

	
		case EVENT_COMMIT:

			
		PI_Control(); 	
			PI_GOH (ID_new_obj, NULL);
			
		// Boucle de control pour vérifier si le moteur PI est prêt
		PI_Check_Moving();
		PI_Position();
	
				
			
			
			break;
	}
	return 0;
}

int CVICALLBACK Callback_Go_To (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		double pos_to_go;	
			
		case EVENT_COMMIT:
			
			
			GetCtrlVal (B_panelHandle,BPANEL_GO_TO_value,&pos_to_go);
			printf("%f\n", pos_to_go); 
			PI_MOV(ID_new_obj,"1", &pos_to_go);  // MVE permet de stopper le mouvement alors que MOV non . This command can be interrupted by PI_STP() and PI_HLT().

			
			// Boucle de control pour vérifier si le moteur PI est prêt
			PI_Check_Moving();
			PI_Position();
			
			break;
	}
	return 0;
}

int CVICALLBACK Callback_DH (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			PI_DFH(ID_new_obj,"1"); 

			break;
	}
	return 0;
}

int CVICALLBACK Callback_TS (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		int k;
		double j,positions[5000];
		
		
		case EVENT_COMMIT:
			
			Get_Parameters_motor();
			k=0;
			j=Start_pos_value;
			Stop_scan_value = 0;
			
 
				File_MotorPosition = fopen("Motor_position.out","w");
				
			
			while(Stop_scan_value==0)
			{
				ProcessSystemEvents();	// to allow stopping the scan

				k++;
				
				PI_move_motor(j);
				PI_Check_Moving();
				PI_Position();	  
				
				
				//fprintf(File_MotorPosition, "%f\n", j);
				
				positions[k - 1] = Current_pos_value;
						 
				SetCtrlVal (B_panelHandle,BPANEL_Current_pos_value,Current_pos_value); 
				
				
				j=j+Step_nbr_value;
	
		
				
				

				if (j >= Stop_pos_value)
				{Stop_scan_value = 1;
				Beep();
				}
			}

			
			Beep();
			
			
			ArrayToFile ("Motor_position.out", positions, VAL_DOUBLE, k, 1,
						 VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS,
						 VAL_CONST_WIDTH, 10, VAL_ASCII, VAL_TRUNCATE);

			
			fclose(File_MotorPosition);
			
break;
	}
	return 0;
}

int CVICALLBACK Stopscan_callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			Stop_scan_value = 1;

			break;
	}
	return 0;
}

int CVICALLBACK Get_pos_callback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				  PI_Position();
			break;
	}
	return 0;
}

int CVICALLBACK callback_get_step_size (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		
				 PI_stepsize();
			
			

			break;
	}
	return 0;
}
