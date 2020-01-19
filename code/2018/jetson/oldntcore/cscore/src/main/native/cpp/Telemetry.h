/*----------------------------------------------------------------------------*/
/* Copyright (c) 2015-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_TELEMETRY_H_
#define CSCORE_TELEMETRY_H_

#include <support/SafeThread.h>

#include "cscore_cpp.h"

namespace cs {

class SourceImpl;

class Telemetry {
  friend class TelemetryTest;

 public:
  static Telemetry& GetInstance() {
    static Telemetry instance;
    return instance;
  }
  ~Telemetry();

  void Start();
  void Stop();

  // User interface
  void SetPeriod(double seconds);
  double GetElapsedTime();
  int64_t GetValue(CS_Handle handle, CS_TelemetryKind kind, CS_Status* status);
  double GetAverageValue(CS_Handle handle, CS_TelemetryKind kind,
                         CS_Status* status);

  // Telemetry events
  void RecordSourceBytes(const SourceImpl& source, int quantity);
  void RecordSourceFrames(const SourceImpl& source, int quantity);

 private:
  Telemetry();

  class Thread;
  wpi::SafeThreadOwner<Thread> m_owner;
};

}  // namespace cs

#endif  // CSCORE_TELEMETRY_H_
