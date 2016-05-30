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

#define  DPANEL                           1
#define  DPANEL_Device_ON                 2       /* control type: LED, callback function: (none) */
#define  DPANEL_DLL_ON                    3       /* control type: LED, callback function: (none) */
#define  DPANEL_COMMANDBUTTON_3           4       /* control type: command, callback function: capture */
#define  DPANEL_ccd1                      5       /* control type: graph, callback function: (none) */
#define  DPANEL_daq                       6       /* control type: graph, callback function: (none) */
#define  DPANEL_saveall                   7       /* control type: binary, callback function: saveall_cllbck */
#define  DPANEL_selectdir                 8       /* control type: command, callback function: selectdir */
#define  DPANEL_actual_dir                9       /* control type: string, callback function: (none) */
#define  DPANEL_shots                     10      /* control type: numeric, callback function: shots_number */
#define  DPANEL_COMMANDBUTTON             11      /* control type: command, callback function: monitor */
#define  DPANEL_COMMANDBUTTON_4           12      /* control type: command, callback function: stop_monitor */
#define  DPANEL_trigger                   13      /* control type: numeric, callback function: trigs */
#define  DPANEL_high_pixel_y              14      /* control type: numeric, callback function: (none) */
#define  DPANEL_low_pixel_y               15      /* control type: numeric, callback function: (none) */
#define  DPANEL_resize_2                  16      /* control type: command, callback function: resize_y */
#define  DPANEL_high_pixel                17      /* control type: numeric, callback function: (none) */
#define  DPANEL_low_pixel                 18      /* control type: numeric, callback function: (none) */
#define  DPANEL_resize                    19      /* control type: command, callback function: resize */
#define  DPANEL_CHECKBOX_DrawCCD          20      /* control type: radioButton, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK capture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resize(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resize_y(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveall_cllbck(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK selectdir(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK shots_number(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK stop_monitor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK trigs(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
