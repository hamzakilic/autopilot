//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#include "../common/atp_common.h"

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile em_float32 beta;				// algorithm gain
extern volatile em_float32 q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame

//---------------------------------------------------------------------------------------------------
// Function declarations

void MadgwickAHRSupdate(em_float32 gx, em_float32 gy, em_float32 gz, em_float32 ax, em_float32 ay, em_float32 az, em_float32 mx, em_float32 my, em_float32 mz);
void MadgwickAHRSupdateIMU(em_float32 gx, em_float32 gy, em_float32 gz, em_float32 ax, em_float32 ay, em_float32 az);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
