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

#define  APANEL                           1
#define  APANEL_COMMANDBUTTON             2       /* control type: command, callback function: quit */
#define  APANEL_INTENSESCALE              3       /* control type: graph, callback function: (none) */
#define  APANEL_INTINTENSECOLOR1          4       /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_FROGPLOT_LEFT             5       /* control type: graph, callback function: MoveCursor_2 */
#define  APANEL_FROGPLOT_DOWN             6       /* control type: graph, callback function: MoveCursor_2 */
#define  APANEL_FROGPLOT                  7       /* control type: graph, callback function: MoveCursor */
#define  APANEL_INTINTENSECOLOR2          8       /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_INTINTENSECOLOR3          9       /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_INTINTENSECOLOR4          10      /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_MININTENSECOLOR           11      /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_MAXINTENSECOLOR           12      /* control type: color, callback function: AdjustIntenseColor */
#define  APANEL_label_1                   13      /* control type: textMsg, callback function: (none) */
#define  APANEL_label_2                   14      /* control type: textMsg, callback function: (none) */
#define  APANEL_label_3                   15      /* control type: textMsg, callback function: (none) */
#define  APANEL_label_4                   16      /* control type: textMsg, callback function: (none) */
#define  APANEL_average                   17      /* control type: numeric, callback function: (none) */
#define  APANEL_COMMANDBUTTON_2           18      /* control type: command, callback function: Callback_Start_Scan */
#define  APANEL_COMMANDBUTTON_3           19      /* control type: command, callback function: Callback_stop_frog */
#define  APANEL_step_fs                   20      /* control type: binary, callback function: callback_steps_nm */
#define  APANEL_conversion_pix_nm         21      /* control type: numeric, callback function: callback_pixel_nm */
#define  APANEL_pixel_nm                  22      /* control type: binary, callback function: callback_pixel_nm */
#define  APANEL_CHECKBOX_pause            23      /* control type: radioButton, callback function: Callback_pause */
#define  APANEL_offset_max                24      /* control type: numeric, callback function: AdjustIntenseColor */
#define  APANEL_offset_min                25      /* control type: numeric, callback function: AdjustIntenseColor */
#define  APANEL_scale_switch              26      /* control type: binary, callback function: callback_scale_switch */
#define  APANEL_offset_nm_value           27      /* control type: numeric, callback function: callback_pixel_nm */
#define  APANEL_TEXTMSG_PAUSE             28      /* control type: textMsg, callback function: (none) */
#define  APANEL_deltaX                    29      /* control type: numeric, callback function: (none) */
#define  APANEL_deltaY                    30      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorX2                  31      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorZ2                  32      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorY2                  33      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorX1                  34      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorZ1                  35      /* control type: numeric, callback function: (none) */
#define  APANEL_cursorY1                  36      /* control type: numeric, callback function: (none) */
#define  APANEL_DECORATION_2              37      /* control type: deco, callback function: (none) */
#define  APANEL_DECORATION_4              38      /* control type: deco, callback function: (none) */
#define  APANEL_DECORATION_5              39      /* control type: deco, callback function: (none) */
#define  APANEL_LOAD                      40      /* control type: command, callback function: callback_load */
#define  APANEL_DEL_GRAPH                 41      /* control type: command, callback function: callback_del_graph */
#define  APANEL_DECORATION_6              42      /* control type: deco, callback function: (none) */
#define  APANEL_label_6                   43      /* control type: textMsg, callback function: (none) */
#define  APANEL_num_load                  44      /* control type: numeric, callback function: (none) */
#define  APANEL_DECORATION                45      /* control type: deco, callback function: (none) */
#define  APANEL_label_5                   46      /* control type: textMsg, callback function: (none) */
#define  APANEL_DECORATION_3              47      /* control type: deco, callback function: (none) */
#define  APANEL_TEXTMSG_2                 48      /* control type: textMsg, callback function: (none) */
#define  APANEL_DECORATION_8              49      /* control type: deco, callback function: (none) */
#define  APANEL_TEXTMSG_7                 50      /* control type: textMsg, callback function: (none) */
#define  APANEL_DECORATION_7              51      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AdjustIntenseColor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_del_graph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_load(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_pause(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_pixel_nm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_scale_switch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_Start_Scan(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK callback_steps_nm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Callback_stop_frog(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MoveCursor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MoveCursor_2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
