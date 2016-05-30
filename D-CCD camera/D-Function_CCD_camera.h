
#include <NIDAQmx.h>
#include <DAQmxIOctrl.h>



///////////////////////////////////////////////
//*CCD function
///////////////////////////////////

void USB_init(void);  
void Capture_Image(void);
void set_array(void);
void plot_spectra(void);
int save_spectra();
void close_CCD(void);    

///////////////////////////////////////////////
//* hardware : GPIB AND DAQ function
///////////////////////////////////
void init_hardware(void);

void GPIB_init(void);
void GPIB_ready(void);
void GPIB_error_check(void);


TaskHandle init_DAQ(uInt32);
int Read_DAQ(uInt32);
void define_colors(void); 



