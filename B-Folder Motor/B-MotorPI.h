/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  BPANEL                           1
#define  BPANEL_Button_Get_error          2       /* control type: command, callback function: Callback_Get_error */
#define  BPANEL_Indic_Error               3       /* control type: numeric, callback function: (none) */
#define  BPANEL_Home                      4       /* control type: command, callback function: Callback_Home */
#define  BPANEL_GO_TO_value               5       /* control type: numeric, callback function: (none) */
#define  BPANEL_GO_TO_button              6       /* control type: command, callback function: Callback_Go_To */
#define  BPANEL_Home_value                7       /* control type: numeric, callback function: (none) */
#define  BPANEL_Current_pos_value         8       /* control type: numeric, callback function: (none) */
#define  BPANEL_Define_Home               9       /* control type: command, callback function: Callback_DH */
#define  BPANEL_Stop_pos                  10      /* control type: numeric, callback function: (none) */
#define  BPANEL_Step_nbr                  11      /* control type: numeric, callback function: (none) */
#define  BPANEL_Start_pos                 12      /* control type: numeric, callback function: (none) */
#define  BPANEL_Time_scan                 13      /* control type: command, callback function: Callback_TS */
#define  BPANEL_Stop_scan                 14      /* control type: command, callback function: Stopscan_callback */
#define  BPANEL_Get_pos_button            15      /* control type: command, callback function: Get_pos_callback */
#define  BPANEL_step_size_indicator       16      /* control type: numeric, callback function: (none) */
#define  BPANEL_GET_STEP_SIZE             17      /* control type: command, callback function: callback_get_step_size */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Callback_DH(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_Get_error(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_get_step_size(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_Go_To(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_Home(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_TS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Get_pos_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stopscan_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
