/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HLUsageReporting.h"

class HardwareHLReporting : public HLUsageReportingInterface {
 public:
  virtual void ReportScheduler();
  virtual void ReportSmartDashboard();
};
