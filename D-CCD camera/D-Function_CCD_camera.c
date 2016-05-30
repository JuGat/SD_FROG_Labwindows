#include "A-SDFROG_LW.h"

 // Include PANEL
#include "D-CCD_camera.h" 
 // Include Prototype  
#include "D-Function_CCD_camera.h"  
#include "C-Function_Frog_acq.h"



# define MAXRATE  10000 // max sampling rate 
# define KHzRATE  1000 // max sampling rate 

#include "toolbox.h"
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
//#include <stdint.h> 
#include "D-CCD_camera.h"
#include "DCamUSB.h"

#include <gpib.h>    
// DAQ
#include <NIDAQmx.h>
#include <DAQmxIOctrl.h>


//files for share variable
#include <Z-variable.h> 

 

// linear CCD 
int bits, bytes,Draw_CCD_value;
unsigned long status;
unsigned int lines=2;
unsigned short  *array=NULL;


// GPIB 
int device; 

// DAQ 
uInt32 sampsPerChan;      
float64 min=0;
float64 max=10.0;
float64 rate=MAXRATE;  // hardware limited, may change for different DAQ
uInt32 numChannels; 
TaskHandle taskHandle = NULL;

// Program global variables
int saveall, global_file_counter;
int firsttime=1;
float64     *data=NULL;
char pathname[512];
int colors [16];
int StopMonitor;

static char pathName_capture[260];


void define_colors(void)
{
colors[0]=0x00FF00L;
colors[1]=0x0000FFL;
colors[2]=0x00FFFFL;
colors[3]=0xFF00FFL;
colors[4]=0xFFFF00L;
colors[5]=0x800000L;
colors[6]=0x000080L;
colors[7]=0x008000L;
colors[8]=0x008080L;
colors[9]=0x800080L;
colors[10]=0x808000L;
colors[11]=0xC0C0C0L;
colors[12]=0x808080L;
colors[13]=0x000000L;
colors[14]=0xFFFFFFL;
colors[15]=0xC0C0C0L;
	
	


}


////////////////////////////////////////////////////////////////////
////////////////////HARDWARE///////////////////////////////////////
///////////////////////////////////////////////////////////////////

void init_hardware(void)
{
	
GetCtrlVal(D_panelHandle, DPANEL_shots,&lines);  	
USB_init();
//GPIB_init();
//GPIB_ready();
init_DAQ(lines);  
}


TaskHandle init_DAQ(uInt32 sampsPerChan)
{
		/*********************************************/
		// DAQmx Configure Code
		/*********************************************/
	
		if(taskHandle)
			DAQmxClearTask(taskHandle);
		
		DAQmxCreateTask("",&taskHandle);

		DAQmxCreateAIVoltageChan (taskHandle, "Dev1/ai1:3", "", DAQmx_Val_Cfg_Default, min, max, DAQmx_Val_Volts, NULL);
		
		DAQmxCfgSampClkTiming (taskHandle, "PFI0", 1000, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, sampsPerChan); // KHz trig in PFI1
		
		DAQmxGetTaskAttribute(taskHandle,DAQmx_Task_NumChans,&numChannels); 
		
		return taskHandle;
}




void GPIB_init(void)
{
	int address=15; 

	device = ibdev (0, address, NO_SAD, T10s, 1, 0);    

}

void GPIB_ready(void)
{
//void *write_buffer;
static char write_buffer[100];
char * GPIB_command;

strcpy (write_buffer, "*RST\n"); // clears memory
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "DLAY 3,2,1e-5\n"); // A--B = 10 micro seconds
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "DLAY 5,4,1e-5\n"); // C--D = 10 micro seconds
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "DLAY 7,6,1e-5\n"); // C--D = 10 micro seconds
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "DLAY 9,8,1e-5\n"); // E--F = 10 micro seconds
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "SHDP 1\n"); 
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "BURT 1\n"); // outputs T0
ibwrt (device, write_buffer, strlen(write_buffer));
GPIB_error_check();

/*
strcpy (write_buffer, "DISP 2\n"); 
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();
*/

strcpy (write_buffer, "TSRC 3\n"); // sets trigger to single shot (no output)
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

strcpy (write_buffer, "BURM 1\n"); 
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();


strcpy (write_buffer, "BURP 1.000263e-3\n"); 
ibwrt (device, write_buffer, strlen(write_buffer)); 
GPIB_error_check();

}


void GPIB_error_check(void)
{
if (ibsta & 0x8000)
                    {
                    printf("GPIB Comm Error %i\n", iberr); // error is iberr
                    }
}

void USB_init(void)
{
	short error;
	int device_state, width, height, linecount, time_unit, usb, mode;
	unsigned long lasterror;
		 

	error=DcamInitialize();
	if (error==1)
		SetCtrlVal(D_panelHandle, DPANEL_DLL_ON, 1);
		error=DcamOpen();
	if (error==1)
	{	SetCtrlVal(D_panelHandle, DPANEL_Device_ON, 1);  }
	else
	{
		DcamGetDeviceState (&device_state);
		Beep();
	}
	
	DcamStop(); 
	
	DcamSetLEDOperatingMode(DCAM_LEDOPMODE_OFF); 		// Set the LED on the OFF mode
	
	error=DcamSetTriggerMode(DCAM_TRIGMODE_EXT_EDGE2);
//	error=DcamSetTriggerMode(DCAM_TRIGMODE_INT); 

	if (error==0)
	{
		printf("trigger mode error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}


	error=DcamSetExposureTime(600);  // in us!!
	if (error==0)
	{
		printf("setting exp. time error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}

	error=DcamSetMeasureDataCount(1);
	if (error==0)
	{
		printf("setting exp. time error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}

	error=DcamSetOutPulse(DCAM_OUTMODE_PLS_ACCUM, DCAM_OUTPOL_POSITIVE, 0,0);
	if (error==0)
	{
		printf("setting exp. time error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}
	
	error=DcamGetOperatingMode(&mode);
	if (error==0)
	{
		printf("setting exp. time error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}

	error=DcamGetGain(&mode);
	if (error==0)
	{
		printf("setting exp. time error\n");
		lasterror = DcamGetLastError();
		printf("last error code:%i\n", lasterror);
	}



	set_array();
	
}






void set_array(void)
{
	DcamSetMeasureDataCount(lines);
	DcamGetCaptureBytes(&bytes);
	array=malloc(bytes);
}


void Capture_Image(void)
{
	int error;
	unsigned long lasterror, capture_status;
	int shots;
	char write_buffer[100], write_buffer2[100];
//	float64     *data=NULL;
	float64 dummy[100];
	int32 numRead, numRead1, status, status1;
	bool32 DAQcompleted;
	
	sampsPerChan=lines; 
	
	
	strcpy (write_buffer, "BURC"); 
	sprintf(write_buffer2, " %i\n", lines);
	strcat(write_buffer, write_buffer2);
//	printf("%s", write_buffer);

	
	
 //ibwrt (device, write_buffer, strlen(write_buffer)); 
//		GPIB_error_check();

	
	

//	printf("sampsPerChan numChannels: %i %i\n", sampsPerChan,numChannels);
	
	
	
	
	data=malloc(sampsPerChan*numChannels*sizeof(float64));  
	

	DAQmxStartTask(taskHandle);
	//status = DAQmxReadAnalogF64 (taskHandle, sampsPerChan, 5.0, DAQmx_Val_GroupByChannel, data, sampsPerChan*numChannels, &numRead, NULL); // DAQ armed and ready for input
	//status1= DAQmxReadAnalogF64 (taskHandle, 1, 0.1, DAQmx_Val_GroupByChannel, dummy, 1*numChannels, &numRead1, NULL); // DAQ armed and ready for input

	
	error=DcamCapture(&array[0], bytes);  // CCD armed waiting for trigger
	

	//printf("before sending trigger\n");
	
	strcpy (write_buffer, "*TRG\n"); 
	//printf("%s", write_buffer);
//	ibwrt (device, write_buffer, strlen(write_buffer)); 
//	GPIB_error_check();

// ENABLE for camera reading

	
	if (error==0)
	{
		printf("capture error\n");
		lasterror = DcamGetLastError();
		printf("no1: last error code:%i\n", lasterror);
	}

	DcamWait(&capture_status, 200);
		
	if(DcamWait(&capture_status, 200)!=DCAM_WAITSTATUS_COMPLETED)
	 
	{
		lasterror = DcamGetLastError();
	}
	
	error=DcamStop();
	if (error==0)
	{
		printf("stop error\n");
		lasterror = DcamGetLastError();
		printf("no3: last error code:%i\n", lasterror);
	}

//*/
	
//	printf("before reading DAQ\n");

//	status = DAQmxReadAnalogF64 (taskHandle, sampsPerChan, 1, DAQmx_Val_GroupByChannel, data, sampsPerChan*numChannels, &numRead, NULL); // DAQ armed and ready for input

//* Diagnostics
//	printf("DAQ Status: %i\t NumRead: %i \tNumRead dummy: %i\n", status,  numRead, numRead);
//	printf("[%g][%g][%g]\n", data[0], data[1], data[2]);
	
	DAQmxStopTask(taskHandle);
	
	if (saveall)
	save_spectra();

//*	 ENABLE
	GetCtrlVal(D_panelHandle, DPANEL_CHECKBOX_DrawCCD, &Draw_CCD_value);
	if(Draw_CCD_value==1)
	{
	plot_spectra();
	}
	
	free(data);
   DcamStop();
	
return;

}


void disable_capture_buttons()
{				  
	SetCtrlAttribute (D_panelHandle, DPANEL_COMMANDBUTTON, ATTR_DIMMED, 1);
	SetCtrlAttribute (D_panelHandle, DPANEL_COMMANDBUTTON_3, ATTR_DIMMED, 1);
	ProcessSystemEvents();
}

void enable_capture_buttons()
{				  
	SetCtrlAttribute (D_panelHandle, DPANEL_COMMANDBUTTON, ATTR_DIMMED, 0);
	SetCtrlAttribute (D_panelHandle, DPANEL_COMMANDBUTTON_3, ATTR_DIMMED, 0);
}
	


int select_path()
{
   int status;
   
   
strcpy(pathName_capture,"NULL"); // permet de controler les noms de dossier
	
	if ((CompareStrings ("NULL", 0, pathName_capture, 0, 1) == 0))   
	{
		status = DirSelectPopup("d:\\Labuser\\Desktop\\SDFROG_Labwindows_20160523", "Select Folder", 1, 1, pathName_capture);
		
		if(status==0)
		{	
			strcpy(pathName_capture,"NULL"); // permet de controler les noms de dossier
			printf("You didn t select a directory\n");
			return(status);
		}
	
	}
	
  

	SetCtrlVal(D_panelHandle, DPANEL_actual_dir, pathName_capture);
	global_file_counter=0;
	
	
	return 0;

}

int save_spectra()
{
FILE *singleshot_data;
char filename[512];
char dummy[100];
int i, j;
static char string1[260]; 
  

strcpy(filename, pathName_capture);
strcpy(string1, "\\Capture_CCD_");
strcat(filename,string1);

sprintf(dummy, "ss%i.out", global_file_counter);

strcat(filename, dummy);
 
   




//printf("%s\n", filename);

singleshot_data = fopen (filename, "w");

	if (singleshot_data==NULL)
	{

		SetCtrlVal(D_panelHandle, DPANEL_actual_dir, "BAD FILENAME!!! ");

		Beep();

	return;

	}

	for (i=0; i<lines; i++)
	{
	
		// write DAQ channels first
		for (j=0; j<numChannels; j++)
		fprintf(singleshot_data, "%g\t", data[j*lines+i]);
	
		for (j=0; j<2068; j++)
		fprintf(singleshot_data, "%i\t", array[i*2068+j]);
	
		fprintf(singleshot_data, "\n");

	}


fclose(singleshot_data);

global_file_counter++;


return 0;
}


void plot_spectra(void)
{
	int i, k;
	
	units_pixel_nm();
	


	DeleteGraphPlot (D_panelHandle, DPANEL_ccd1, -1, VAL_IMMEDIATE_DRAW);         
	DeleteGraphPlot (D_panelHandle, DPANEL_daq, -1, VAL_IMMEDIATE_DRAW);   
	
	for (i=0; i<lines; i++)
	{
	
		if (i<15)
			k=i;
		else
			k=i%16;
	
	PlotXY (D_panelHandle, DPANEL_ccd1, pixel_nm_axis, &array[i*2068], 2068,VAL_DOUBLE, VAL_UNSIGNED_SHORT_INTEGER, VAL_THIN_LINE,VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
	
//	PlotY (D_panelHandle, DPANEL_ccd1, &array[i*2068], 2068, VAL_UNSIGNED_SHORT_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
	SetAxisRange (D_panelHandle, DPANEL_ccd1, VAL_MANUAL, offset_nm_value, Max_pixel_nm, VAL_AUTOSCALE, 0.0, 1.0);

//	PlotY (D_panelHandle, DPANEL_ccd1, &array[i*2068], 2068, VAL_UNSIGNED_SHORT_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1,  colors[k]);

	}

/*	for (i=0; i<numChannels; i++)
	{
	PlotY (D_panelHandle, DPANEL_daq, &data[i*lines], lines, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_EMPTY_DIAMOND, VAL_SOLID, 1, colors[i]);
	}
*/
	return;
}


void close_CCD(void)
{
	char write_buffer[256];
    int error;
	
	error=DcamClose();
	if (error==0)
	SetCtrlVal(D_panelHandle, DPANEL_Device_ON, 0);
	
	error=DcamUninitialize();
	if (error==0)
	SetCtrlVal(D_panelHandle, DPANEL_DLL_ON, 0);
	
/*	strcpy (write_buffer, "*RST\n"); 
	ibwrt (device, write_buffer, strlen(write_buffer)); 
	GPIB_error_check();
	
	
	strcpy (write_buffer, "DLAY 3,2,1e-5\n"); // A--B = 10 micro seconds
//	ibwrt (device, write_buffer, strlen(write_buffer)); 
	GPIB_error_check(); */
	
	
}


void real_time (void)
{
 
  StopMonitor=1;
  saveall=0;
  
  SetCtrlVal(D_panelHandle, DPANEL_saveall,0);
  
  while(StopMonitor)
  {
   	  	Capture_Image();
		ProcessSystemEvents();

  }


}

 void change_trigger (void)           

 {
 	 double new_trig;
	 static char write_buffer[100], dummy[100];
	 char * GPIB_command;
	 
	 GetCtrlVal(D_panelHandle, DPANEL_trigger, &new_trig);
	 
	 sprintf(dummy, "DLAY 2,0,%ge-9\n",new_trig);
	 //printf("%s", dummy);
	 strcpy (write_buffer, dummy); 
	 ibwrt (device, write_buffer, strlen(write_buffer)); 
	 GPIB_error_check();
 
 }

			
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




int CVICALLBACK capture (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			disable_capture_buttons();
	  		Capture_Image();
			enable_capture_buttons();
		break;
	}
	return 0;
}

int CVICALLBACK status_check (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

		DcamWait(&status, 0);
 		//printf("status=%i\n", status);
 		DcamStop();
		break;
	}
	return 0;
}





int CVICALLBACK saveall_cllbck (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		if (saveall)
			saveall=0;
		else
		{saveall=1;
		if (firsttime)
			select_path();
		}
		
		
			break;
	}
	return 0;
}

int CVICALLBACK selectdir (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		select_path();
			break;
	}
	return 0;
}

int CVICALLBACK shots_number (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			init_hardware();
			 break;
	}
	return 0;
}

int CVICALLBACK monitor (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			disable_capture_buttons();
			real_time();
			enable_capture_buttons();
			break;
	}
	return 0;
}

int CVICALLBACK stop_monitor (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	   
			StopMonitor=0;
			
			break;
	}
	return 0;
}

int CVICALLBACK trigs (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		change_trigger();
			break;
	}
	return 0;
}

int CVICALLBACK resize (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int lowPix, highPix;
	
	switch (event)
	{
		case EVENT_COMMIT:
		GetCtrlVal(D_panelHandle, DPANEL_low_pixel, &lowPix);
		GetCtrlVal(D_panelHandle, DPANEL_high_pixel, &highPix);
		SetAxisScalingMode (D_panelHandle, DPANEL_ccd1, VAL_BOTTOM_XAXIS, VAL_MANUAL, lowPix, highPix);
		
		break;
	}
	return 0;
}

int CVICALLBACK resize_y (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int lowPix, highPix;
	
	switch (event)
	{
		case EVENT_COMMIT:
		GetCtrlVal(D_panelHandle, DPANEL_low_pixel_y, &lowPix);
		GetCtrlVal(D_panelHandle, DPANEL_high_pixel_y, &highPix);
		SetAxisScalingMode (D_panelHandle, DPANEL_ccd1, VAL_LEFT_YAXIS, VAL_MANUAL, lowPix, highPix);
		
		break;
	}
	return 0;
}

