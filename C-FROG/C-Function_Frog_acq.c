
 //==============================================================================
//
// Title:		FROG 
// Purpose:		A short description of the application.

//==============================================================================

#include <cviauto.h>
#include "toolbox.h"
#include <utility.h>
#include <formatio.h>
#include <analysis.h>
#include <ansi_c.h>
#include <userint.h>



//PI
#include "B-MotorPI.h" 			 // PANEL POUR RECUP LES BOUTONS
#include "B-Function_MotorPIC863.h"   // Prototype
#include "A-SDFROG_LW.h"  // Panel for get value 

//files for share variable
#include "Z-variable.h"
#include "D-CCD_camera.h"
#include "C-Function_Frog_acq.h"




/////* VARIABLES
int motor_stepsN,pause,scale_switch=0; 
int average; 
double max_value, min_value; 
int offset_min,offset_max;  

double array_frog[2068][5000];  	  // this is the main array which is plotted in FROGPLOT
double array_frog_LEFT[2068];   // this array control the graph on the left 
double array_frog_DOWN[5000];   // this array control the graph down 

double array_shots[2068][5000];
static char pathName[260],pathName1[260],pathName2[260],pathName3[260],pathName_prev[260]; 
static char pathName_load[260],pathName1_load[260]; 


int color0_1,color0_2,color0_3,color0_4,color0_5,color0_6   ;

int color_1 ,color_2,color_3,color_4,color_5,color_6;

int n=0,q=0,NCOLS,step_fs  ;
double Frog_par[10]; 

// cursors variables
double x1,y1,x2,y2,z2,z1,deltax,deltay;
int max_x_DOWN, min_x_DOWN;
double max_DOWN, min_DOWN ;

int max_x_LEFT, min_x_LEFT;
double max_LEFT, min_LEFT;



FILE *File_MotorPosition; 
FILE *File_Frog; 
FILE *Size; 


ColorMapEntry colormap_FROG[6];




/////////////////////////////////////////////////////////////////////////
//* FUNCTIONS
/////////////////////////////////////////////////////////////////////////
set_cursors_variables(void)
{
units_pixel_nm();
units_step_fs();
	
SetCtrlVal (A_panelHandle,APANEL_cursorX1, x1);
SetCtrlVal (A_panelHandle,APANEL_cursorY1, y1);
SetCtrlVal (A_panelHandle,APANEL_cursorX2, x2);
SetCtrlVal (A_panelHandle,APANEL_cursorY2, y2);
			
deltax = fabs(x1-x2);
deltay = fabs(y1-y2);
			
SetCtrlVal (A_panelHandle,APANEL_deltaX, deltax);
SetCtrlVal (A_panelHandle,APANEL_deltaY, deltay);


int A,B,C,D;

B = RoundRealToNearestInteger((y1-offset_nm_value)/conversion_pix_nm);
D = RoundRealToNearestInteger((y2-offset_nm_value)/conversion_pix_nm);

A = RoundRealToNearestInteger(x1/conversion_step_fs) ;
C = RoundRealToNearestInteger(x2/conversion_step_fs) ;
	
z1= array_frog[B][A];
z2= array_frog[D][C];

SetCtrlVal (A_panelHandle,APANEL_cursorZ1, z1);
SetCtrlVal (A_panelHandle,APANEL_cursorZ2, z2);	  
	
	return 0;
}


rainbow(void)
{
color0_1= 0x211AB7L   ;
color0_2= 0x1B9BC4L   ;
color0_3= 0x1BD16BL   ;
color0_4= 0x64DF1CL   ;
color0_5= 0xECC81DL   ;
color0_6= 0xF9221DL   ;	
SetCtrlVal (A_panelHandle,APANEL_MININTENSECOLOR,color0_1);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR1,color0_2);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR2,color0_3);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR3,color0_4);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR4,color0_5);
SetCtrlVal (A_panelHandle,APANEL_MAXINTENSECOLOR,color0_6);

return 0;
}

  

RED_BLACK(void)
{
color0_1= 0x000000L   ;
color0_2= 0x330000L   ;
color0_3= 0x660000L   ;
color0_4= 0x990000L   ;
color0_5= 0xCC0000L   ;
color0_6= 0xFF0000L   ;	
SetCtrlVal (A_panelHandle,APANEL_MININTENSECOLOR,color0_1);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR1,color0_2);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR2,color0_3);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR3,color0_4);
SetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR4,color0_5);
SetCtrlVal (A_panelHandle,APANEL_MAXINTENSECOLOR,color0_6);

return 0;
}   

left_down_plots(void)
{

for(int a=0; a<2068;a++)
{
	for(int b=0; b<NCOLS;b++)
	{
		array_frog_LEFT[a] += array_frog[a][b]; 
	}
  array_frog_LEFT[a] /= NCOLS;
}

for(int b=0; b<NCOLS;b++)
{
	for(int a=0; a<2068;a++)
	{
		array_frog_DOWN[b] += array_frog[a][b]; 
	}
	array_frog_DOWN[b] /= 2068; 
}
	
return 0;	
}




Define_colormap(void)
{
	int min_value_row, min_value_col, max_value_row, max_value_col;   
	
MaxMin2D (array_frog, 2068, 5000, &max_value, &max_value_row, &max_value_col, &min_value, &min_value_row, &min_value_col);

    GetCtrlVal (A_panelHandle,APANEL_offset_min,&offset_min); 
	GetCtrlVal (A_panelHandle,APANEL_offset_max,&offset_max); 


	double dvalue = ((max_value+offset_max) - (min_value+offset_min))/5;

	
colormap_FROG[0].color = color0_1;		//LOWEST VALUE

colormap_FROG[0].dataValue.valDouble = min_value +offset_min;

colormap_FROG[1].color = color0_2;    

colormap_FROG[1].dataValue.valDouble = min_value + dvalue +offset_min;

colormap_FROG[2].color = color0_3;    

colormap_FROG[2].dataValue.valDouble = min_value + 2*dvalue +offset_min;

colormap_FROG[3].color = color0_4;    

colormap_FROG[3].dataValue.valDouble = min_value + 3*dvalue +offset_min;

colormap_FROG[4].color = color0_5;    

colormap_FROG[4].dataValue.valDouble = min_value + 4*dvalue +offset_min;

colormap_FROG[5].color = color0_6;    //HIGHEST VALUE

colormap_FROG[5].dataValue.valDouble = max_value+offset_max;   


return 0;
}


/////////////////////////////////////////////////////////////////////////

set_scale(void)
{
	int	 scale_bar_risol=50  ; 
	double dscale_risol,scale_bar[2][50];
	char    scaleLabel[100]; 
	
	
				dscale_risol =  ((max_value+offset_max)-(min_value+offset_min))/(scale_bar_risol-1);  
	
					for(int q =0; q<scale_bar_risol; q++){
		
				scale_bar[1][q]=scale_bar[0][q] = (min_value+offset_min) + (q)*dscale_risol;
						}
						
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger (scale_bar[0][0]));
					SetCtrlVal (A_panelHandle,APANEL_label_1, scaleLabel);
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger (scale_bar[0][10]));
					SetCtrlVal (A_panelHandle,APANEL_label_2, scaleLabel);
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger (scale_bar[0][20]));
					SetCtrlVal (A_panelHandle,APANEL_label_3, scaleLabel);
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger (scale_bar[0][30]));
					SetCtrlVal (A_panelHandle,APANEL_label_4, scaleLabel);
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger (scale_bar[0][40]));
					SetCtrlVal (A_panelHandle,APANEL_label_5, scaleLabel);
			
					Fmt (scaleLabel,"%d",RoundRealToNearestInteger( scale_bar[0][49]));
					SetCtrlVal (A_panelHandle,APANEL_label_6, scaleLabel);
					
///----------------- Plot the scale

				DeleteGraphPlot (A_panelHandle,APANEL_INTENSESCALE, -1, VAL_IMMEDIATE_DRAW);		
				PlotScaledIntensity (A_panelHandle, APANEL_INTENSESCALE, scale_bar, 2,
									 scale_bar_risol, VAL_DOUBLE, 1.0, 0.0, 1.0, 0.0,
									 colormap_FROG, VAL_GREEN, 6, 1, 0);
		 	
return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
units_step_fs(void)
{	  
	 GetCtrlVal(A_panelHandle,APANEL_step_fs, &step_fs);

			if(step_fs==0)
	{
			  // from fs to staps 

			  conversion_step_fs= Step_nbr_value;
			 	
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT, ATTR_XNAME, "Steps");
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_DOWN, ATTR_XNAME, "Steps"); 
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_LEFT, ATTR_XNAME, "Step-average");
	}
				if(step_fs==1)

	{
			  // from steps to fs
			  
			 conversion_step_fs= Step_nbr_value*6.6;  //fix it 1 step = 1 um means 6.6 fs
			 
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT, ATTR_XNAME, "Time (fs)");
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_DOWN, ATTR_XNAME, "Time (fs)");
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_LEFT, ATTR_XNAME, "Time-average");

	}	
		 Max_step_fs= ((NCOLS-0.5)*conversion_step_fs) ;
		 
		for(int a=0;a<NCOLS;a++)
	{															
    	step_fs_axis[a]=(a*conversion_step_fs) ;
	}
	
	return 0;
}




units_pixel_nm(void)
{


 GetCtrlVal(A_panelHandle,APANEL_pixel_nm, &pixel_nm);

			if(pixel_nm==0)
	{
			  // from nm to pixel 

		
			  Max_pixel_nm=2068;
			  offset_nm_value=0;
			  conversion_pix_nm=1;
			  
			  			 	
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT, ATTR_YNAME, "Pixels");
			SetCtrlAttribute (D_panelHandle,DPANEL_ccd1, ATTR_XNAME, "Pixels"); 
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_LEFT, ATTR_YNAME, "Pixels"); 
		   	SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_DOWN, ATTR_YNAME, "Pixel-average");

	}
	else
	{
			  // from pixel to nm

			 GetCtrlVal(A_panelHandle,APANEL_conversion_pix_nm, &conversion_pix_nm);
 		 	 GetCtrlVal (A_panelHandle,APANEL_offset_nm_value,&offset_nm_value);
			 
			 Max_pixel_nm= (2068*conversion_pix_nm)+offset_nm_value;
			 	
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT, ATTR_YNAME, "Wavelenght (nm)");
			SetCtrlAttribute (D_panelHandle,DPANEL_ccd1, ATTR_XNAME, "Wavelenght (nm)"); 
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_LEFT, ATTR_YNAME, "Wavelenght (nm)");
			SetCtrlAttribute (A_panelHandle,APANEL_FROGPLOT_DOWN, ATTR_YNAME, "Wavelength-average");

	}	
	
	for(int a=0;a<2068;a++)
	{															
		pixel_nm_axis[a]=(a*conversion_pix_nm)+offset_nm_value ;
	}

return 0;	
}

////////////////////////////////////////////////////////////////////////////////////////
FROG_graph(void)
{
	 
  	 Define_colormap();
				
	 set_scale() ;
	 
	 units_pixel_nm();	
	 units_step_fs();
	 
///------- MAIN PLOT	 
	 
	
	 PlotScaledIntensity (A_panelHandle, APANEL_FROGPLOT, array_frog, 5000,
						  2068, VAL_DOUBLE, conversion_pix_nm,
						  offset_nm_value, conversion_step_fs, 0, colormap_FROG, VAL_GREEN,
						  6, 1, 0);
	 SetAxisRange (A_panelHandle, APANEL_FROGPLOT, VAL_MANUAL, 0, Max_step_fs, VAL_AUTOSCALE, 0.0, 1.0);

///------- RIGHT AND DOWN PLOTS
	 
 	 left_down_plots();

	 PlotXY (A_panelHandle, APANEL_FROGPLOT_LEFT,array_frog_LEFT,pixel_nm_axis,2068, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
	 
	 PlotXY (A_panelHandle, APANEL_FROGPLOT_DOWN,step_fs_axis,array_frog_DOWN,5000, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE,VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

	 MaxMin1D (array_frog_DOWN, NCOLS, &max_DOWN, &max_x_DOWN, &min_DOWN, &min_x_DOWN);
	 MaxMin1D (array_frog_LEFT, 2068, &max_LEFT, &max_x_LEFT, &min_LEFT, &min_x_LEFT);
	
	 SetAxisRange (A_panelHandle, APANEL_FROGPLOT_LEFT, VAL_MANUAL, min_LEFT-10, max_LEFT+10, VAL_MANUAL, offset_nm_value, Max_pixel_nm );
	 SetAxisRange (A_panelHandle, APANEL_FROGPLOT_DOWN, VAL_MANUAL, 0, Max_step_fs, VAL_MANUAL, min_DOWN-10, max_DOWN+10);
 

	
return 0;
	
}
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////			  
int Load_FROG()
{
	int a,b;
			
			if(q==0)
			{
				for(int a=0;a<2068;a++)
		  	 	{
				 	for(int b=0;b<5000;b++)
					{ 
					array_frog[a][b]=0;
					}
			
				}
			}
			
			int status, num_file_selected;
			char **file_list=NULL;
			static char path_load[260], path_size[260];
			
			ProcessSystemEvents();	// to allow stopping the scan

			FILE *load;
			FILE *Size_load;
			
			
///---------------------- SELECT FILES
			
     strcpy(path_load,"NULL"); // permet de controler les noms de dossier
	
	if ((CompareStrings ("NULL", 0, path_load, 0, 1) == 0))   
	{
		printf("Select the FROG and the  Parameters .out files\n");
		
	
		status= MultiFileSelectPopup("d:\\Labuser\\Desktop\\SDFROG_Labwindows_20160523","*.out*", "", "", 0, 0, 1, &num_file_selected, &file_list);	
		
		if(num_file_selected>2)
		{
		printf("You select too many files\n");
		return 0;
		}
		
		if(num_file_selected==0)
		{
		printf("You didn't select a file\n");
		return 0;
		}
		
		   strcpy(path_load,file_list[0]);			
		   strcpy(path_size,file_list[1]);	
		
			if(status==0)
		{	
			strcpy(path_load,"NULL"); // permet de controler les noms de dossier
			printf("You didn t select a file\n");
			return(status);
		}
	
	}
			load = fopen(path_load,"r");
			Size_load = fopen(path_size,"r");
			
			FileToArray (path_size,&Frog_par, VAL_DOUBLE,10, 10, VAL_GROUPS_TOGETHER,VAL_GROUPS_AS_ROWS, VAL_ASCII);
			
			motor_stepsN = RoundRealToNearestInteger(Frog_par[0]);
			Step_nbr_value =Frog_par[4]; 
			Start_pos_value = Frog_par[2] ;
			average=Frog_par[1];
			Current_pos_value = Frog_par[3];
			
			NCOLS = motor_stepsN;
			
			// Here we save the Frog Matrice			
			for	(a=0;  a<2068; a++)     		
			{			
				for (b=0;  b<NCOLS; b++)
				{
					fscanf(load, "%lf", &array_frog[a][b]);
				
				}

					fscanf(load, "\n"); 
			}
			
			if(scale_switch==0)
			{
				RED_BLACK();
			}
			else
			{
				 rainbow();
			}
		
FROG_graph();			

fclose(Size_load);
fclose(load);
			
			
			
				for (int i=0; i<2; ++i) 
	{
       printf ("Selected File : %s\n", file_list [i]);
       free (file_list [i]);
       file_list [i] = NULL;
	}		
			q++;
SetCtrlVal (A_panelHandle, APANEL_num_load, q)  ;
	
	return 0;  
}


int Acquire_FROG()
{
   		static char string1[260];
		static char string2[260];
   		static char string3[260];
		static char number[260];
		

		int status;

		int a,b,k=0;
		double j,positions[5000],elapsed1,elapsed2,TIME;
		
			Get_Parameters_motor();

			
			j=Start_pos_value;
			Stop_scan_value = 0;
			
		
		
		//	motor_stepsN = RoundRealToNearestInteger ((Stop_pos_value-Start_pos_value)/Step_nbr_value);

	
	  		GetCtrlVal (A_panelHandle,APANEL_average,&average); 

///---------------------- SELECT A FOlDER 
			
strcpy(pathName,"NULL"); // permet de controler les noms de dossier
	
	if ((CompareStrings ("NULL", 0, pathName, 0, 1) == 0))   
	{
		status = DirSelectPopup("d:\\Labuser\\Desktop\\SDFROG_Labwindows_20160523", "Select Folder", 1, 1, pathName);
		
		if(status==0)
		{	
			strcpy(pathName,"NULL"); // permet de controler les noms de dossier
			printf("You didn t select a directory\n");
			return(status);
		}
	
	}
	
if ((CompareStrings (pathName_prev, 0, pathName, 0, 1) != 0))
	{
	 n=0;
	}

	strcpy(pathName_prev, pathName); 	
	
	
	
			strcpy(pathName1, pathName); 
			strcpy(pathName2, pathName);
			strcpy(pathName3, pathName);


			sprintf(number, "%d", n);
		
			strcpy(string1, "\\C_File_MotorPosition_"); 
			strcat(string1,number);
			strcat(string1,".out");
		
			strcat(pathName1,string1);       
		

			strcpy(string2, "\\A_File_Frog_"); 
			strcat(string2,number);
			strcat(string2,".out");		
		
			strcat(pathName2,string2);
			
			
			strcpy(string3, "\\B_Frog_parameters"); 
			strcat(string3,number);
			strcat(string3,".out");		
		
			strcat(pathName3,string3);
			
	
			File_Frog = fopen(pathName2,"w"); 
			File_MotorPosition = fopen(pathName1,"w");
			Size = fopen(pathName3,"w"); 

			
 
			for(int a=0;a<2068;a++)
			{
				for(int b=0;b<5000;b++)
				{ 
				array_frog[a][b]=0;
				array_frog_DOWN[b]=0;
				}
				array_frog_LEFT[a]=0;
				}
			
			
			
//------------------- WHILE LOOP
			
			while(Stop_scan_value==0)
			{
				ProcessSystemEvents();	// to allow stopping the scan
				
	
				while(pause==1)
				{
					ProcessSystemEvents();
					Delay(1);
				 SetCtrlAttribute (A_panelHandle, APANEL_TEXTMSG_PAUSE, ATTR_VISIBLE,1);
				}
				SetCtrlAttribute (A_panelHandle, APANEL_TEXTMSG_PAUSE, ATTR_VISIBLE,0);
				pause=0;
				
				motor_stepsN =  RoundRealToNearestInteger ((Current_pos_value-Start_pos_value)/Step_nbr_value) ;

				PI_move_motor(j);

				positions[k] = Current_pos_value;   
				NCOLS = k+1;  
				j=j+Step_nbr_value; 

					
															   
				if (j >Stop_pos_value)
				{Stop_scan_value = 1;
				}
				else
				{
					
//--------------STARTS THE AVERAGE LOOP
				for(int aver=0; aver<average;aver++)
				{   
					
					for( a=0;a<2068;a++)
					{
						for( b=0;b<5000;b++)
						{ 
							array_shots[a][b]=0;
						}
					}  
		
					elapsed1=Timer();
					Capture_Image();         //it ends with an array ---> array[lines*2068]
					elapsed2=Timer();
					TIME=elapsed2-elapsed1;
					printf("%f",TIME);
		
					for(a=0;a<2068;a++)			   
					{
						 for(b=0;b<lines-1;b++)
						{
					   		array_shots[a][k] += array[b*2068+a]/((lines-1)*average);
						}
					}

		
					Add2D (array_frog, array_shots, 2068, 5000, &array_frog);
				
				}   
//--------------ENDS THE AVERAGE LOOP
			
					if(scale_switch==0)
						{
								RED_BLACK();
						}else{
					   			 rainbow();
						}	   
				DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT, -1, VAL_IMMEDIATE_DRAW);		
				DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_LEFT, -1, VAL_IMMEDIATE_DRAW);
	 			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_DOWN, -1, VAL_IMMEDIATE_DRAW);
	 
				FROG_graph();
				
				k++;
				}
//--------------------- END OF WHILE LOOP

				NCOLS = motor_stepsN;
			}
			
// Here we save the Frog Matrice			
for	(a=0;  a<2068; a++)     		
{			
	for (b=0;  b<NCOLS; b++)
		{
		fprintf(File_Frog, "%lf\t", array_frog[a][b]);
	
		}

		fprintf(File_Frog, "\n"); 
			
}			
		

fclose(File_Frog);
		
ArrayToFile (pathName1,positions, VAL_DOUBLE, motor_stepsN, 1, VAL_GROUPS_TOGETHER,VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_ASCII,VAL_TRUNCATE);
			
fclose(File_MotorPosition); 

//----- FROG PARAMETER
Frog_par[0]= motor_stepsN;
Frog_par[1]= average;
Frog_par[2]= Start_pos_value;
Frog_par[3]= Current_pos_value;
Frog_par[4]= Step_nbr_value;


ArrayToFile (pathName3,Frog_par, VAL_DOUBLE, 10, 1, VAL_GROUPS_TOGETHER,VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_ASCII,VAL_TRUNCATE);
fclose(Size);

n++;		
	
return 0;
}

//////////////////////////////////////////////////////////////////////
//*  CALLBACKS
//////////////////////////////////////////////////////////////////////

int CVICALLBACK Callback_Start_Scan (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{

		case EVENT_COMMIT:
			
			
		Acquire_FROG(); 
		break;	
		
	}
	return 0;
}

int CVICALLBACK Callback_stop_scan (int panel, int control, int event,
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


int CVICALLBACK AdjustIntenseColor (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			 case EVENT_VAL_CHANGED:
				 
			GetCtrlVal (A_panelHandle,APANEL_MININTENSECOLOR,&color_1);
			GetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR1,&color_2);
			GetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR2,&color_3);
			GetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR3,&color_4);
			GetCtrlVal (A_panelHandle,APANEL_INTINTENSECOLOR4,&color_5);
			GetCtrlVal (A_panelHandle,APANEL_MAXINTENSECOLOR,&color_6);
							
			color0_1 = color_1; 
			color0_2 = color_2; 
			color0_3 = color_3;
			color0_4 = color_4; 
    		color0_5 = color_5;
			color0_6 = color_6;
		
			FROG_graph();
			
		
			break;
	}
	return 0;
}		 

int CVICALLBACK Callback_stop_frog (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			Stop_scan_value=1;

			break;
	}
	return 0;
}

int CVICALLBACK Callback_pause (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			  GetCtrlVal (A_panelHandle,APANEL_CHECKBOX_pause,&pause);
			  
			break;
	}
	return 0;
}

int CVICALLBACK callback_scale_switch (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		
			
			if(scale_switch==1)
					{
						RED_BLACK();
						scale_switch=0;
					}		
else					{
					    rainbow();
						scale_switch=1;
					}
			
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT, -1, VAL_IMMEDIATE_DRAW);		
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_LEFT, -1, VAL_IMMEDIATE_DRAW);
	 		DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_DOWN, -1, VAL_IMMEDIATE_DRAW);		
					
			FROG_graph();

			break;
	}
	return 0;
}


int CVICALLBACK callback_pixel_nm (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT, -1, VAL_IMMEDIATE_DRAW);		
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_LEFT, -1, VAL_IMMEDIATE_DRAW);
	 		DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_DOWN, -1, VAL_IMMEDIATE_DRAW);
			
			plot_spectra();  
		    FROG_graph();
		
			break;
	}
	return 0;
}


int CVICALLBACK callback_steps_nm (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT, -1, VAL_IMMEDIATE_DRAW);		
			DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_LEFT, -1, VAL_IMMEDIATE_DRAW);
	 		DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_DOWN, -1, VAL_IMMEDIATE_DRAW);
			
			
			FROG_graph();
			
			break;
	}
	return 0;
}


 int CVICALLBACK MoveCursor (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
 {


    switch (event) {
        case EVENT_VAL_CHANGED:
			
            GetGraphCursor (A_panelHandle,APANEL_FROGPLOT, 1, &x1, &y1);
			GetGraphCursor (A_panelHandle,APANEL_FROGPLOT, 2, &x2, &y2);

			
			set_cursors_variables();
			
            
            SetGraphCursor (A_panelHandle,APANEL_FROGPLOT_LEFT, 1, min_LEFT+1, y1);
			SetGraphCursor (A_panelHandle,APANEL_FROGPLOT_LEFT, 2, min_LEFT+1, y2);

		    SetGraphCursor (A_panelHandle,APANEL_FROGPLOT_DOWN, 1, x1, min_DOWN+1);
   		    SetGraphCursor (A_panelHandle,APANEL_FROGPLOT_DOWN, 2, x2, min_DOWN+1);

			
			 break;
	 }
	 return 0;
 }

												  

int CVICALLBACK MoveCursor_2 (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			
			double w1,w2,w3,w4; 
			
			GetGraphCursor (A_panelHandle,APANEL_FROGPLOT_LEFT, 1, &w1, &y1);
			GetGraphCursor (A_panelHandle,APANEL_FROGPLOT_LEFT, 2, &w2, &y2) ;
			
			GetGraphCursor (A_panelHandle,APANEL_FROGPLOT_DOWN, 1, &x1, &w3);
			GetGraphCursor (A_panelHandle,APANEL_FROGPLOT_DOWN, 2, &x2, &w4) ;
			
			set_cursors_variables();
			
			SetGraphCursor (A_panelHandle,APANEL_FROGPLOT, 1, x1, y1);
			SetGraphCursor (A_panelHandle,APANEL_FROGPLOT, 2, x2, y2);
			
			

			break;
	}
	return 0;
}



int CVICALLBACK callback_load (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			Load_FROG();
			
			break;
	}
	return 0;
}

int CVICALLBACK callback_del_graph (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	
		 DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT, -1, VAL_IMMEDIATE_DRAW);
	 	 DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_LEFT, -1, VAL_IMMEDIATE_DRAW);
   	     DeleteGraphPlot (A_panelHandle,APANEL_FROGPLOT_DOWN, -1, VAL_IMMEDIATE_DRAW);
	 	 q=0;
		 SetCtrlVal (A_panelHandle, APANEL_num_load, q)  ;  
			break;
	}
	return 0;
}
