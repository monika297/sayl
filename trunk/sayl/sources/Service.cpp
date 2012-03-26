/** Header **/

#include <sys/time.h>
#include <unistd.h>

#include "Service.h"

#define EXECUTION_RATE_LIMIT 1000. // 1 Kilohertz

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/ 
Service::Service () {
  start = 0.;
  rate = 1;
  ratecontrol = 0;
}

/**********************************************************************************************
 * Run
 *********************************************************************************************/
bool Service::Run () {
  if (rate == 1) return true;
  if (ratecontrol >= rate) ratecontrol = 0;
  if (ratecontrol ++ == 1) return true;
  else return false;
}

/**********************************************************************************************
 * ExecutionRate
 *********************************************************************************************/
double Service::getExecutionRate () const {
  return 1./(EXECUTION_RATE_LIMIT*rate); // execution rate in hertz
}
  
bool Service::setExecutionRate (double r) {
  if (1. <= r && r <= EXECUTION_RATE_LIMIT) rate = (int) (EXECUTION_RATE_LIMIT/r); // r in hertz
  else if (EXECUTION_RATE_LIMIT < r) rate = (int) r; // r in seconds  
  if (rate <= 0) return false;
  return true;
}

/**********************************************************************************************
 * resetTime
 *********************************************************************************************/
void Service::resetTime () {
  start = getCurrentSeconds ();
}
  
/**********************************************************************************************
 * ElapsedTime
 *********************************************************************************************/
double Service::getElapsedTime () const {
  return getCurrentSeconds () - start;
}

/**********************************************************************************************
 * getCurrentSeconds
 *********************************************************************************************/
double Service::getCurrentSeconds () const {
  struct timeval value;
  struct timezone zone;
  gettimeofday(&value, &zone);
  return (value.tv_sec + value.tv_usec * 1e-6);
}

} // namespace SAYL

