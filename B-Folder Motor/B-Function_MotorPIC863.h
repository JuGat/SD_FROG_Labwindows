//==============================================================================
// Global functions
int init_motor(void);
int close_motor(void);


// Motor function
int PI_Control(void);
int PI_Position(void);
int PI_Check_Moving(void);
int PI_stepsize(void) ;

int PI_move_motor(double actual_pos_to_go); 

// Scan function
int Get_Parameters_motor(void) ;
int Acquire_Signal(void);
