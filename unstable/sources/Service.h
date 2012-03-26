#ifndef __SAYL_SERVICE_H__
#define __SAYL_SERVICE_H__

namespace SAYL {

/**********************************************************************************************
 * Service
 *********************************************************************************************/
class Service {
public:
  /** Constructor **/
  Service ();
  
  /** Destructor **/
  virtual ~Service () {}

  /** Run **/
  virtual bool Run ();

  /** ExecutionRate **/
  double getExecutionRate () const;
  
  virtual bool setExecutionRate (double);
  
  /** ElapsedTime **/
  double getElapsedTime () const;
  
  /** resetTime **/
  void resetTime ();  
  
private:
  double start;
  int rate;
  int ratecontrol;

  /** getCurrentSeconds **/
  double getCurrentSeconds () const;
  
}; // class Service

}  // namespace SAYL

#endif

