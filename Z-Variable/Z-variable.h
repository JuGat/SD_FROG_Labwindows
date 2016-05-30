#include "D-Function_CCD_camera.h"





// Panel Share variables
int A_panelHandle,B_panelHandle,D_panelHandle;

// Motor Share Variables
double Current_pos_value;


// Scan Share Variables

int Stop_scan_value;

double Start_pos_value;
double Step_nbr_value;
double Stop_pos_value;


// CCD variables
unsigned int lines; 
unsigned short  *array; 
double conversion_pix_nm, offset_nm_value,Max_pixel_nm,Max_step_fs;
double conversion_step_fs; 

int pixel_nm;
double pixel_nm_axis[2068];
double step_fs_axis[5000];
