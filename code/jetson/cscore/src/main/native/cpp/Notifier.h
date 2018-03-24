/*----------------------------------------------------------------------------*/
/* Copyright (c) 2015-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_NOTIFIER_H_
#define CSCORE_NOTIFIER_H_

#include <functional>

#include <support/SafeThread.h>

#include "cscore_cpp.h"

namespace cs {

class SinkImpl;
class SourceImpl;

class Notifier {
  friend class NotifierTest;

 public:
  static Notifier& GetInstance() {
    static Notifier instance;
    return instance;
  }
  ~Notifier();

  void Start();
  void Stop();

  static bool destroyed() { return s_destroyed; }

  void SetOnStart(std::function<void()> on_start) { m_on_start = on_start; }
  void SetOnExit(std::function<void()> on_exit) { m_on_exit = on_exit; }

  int AddListener(std::function<void(const RawEvent& event)> callback,
                  int eventMask);
  void RemoveListener(int uid);

  // Notification events
  void NotifySource(llvm::StringRef name, CS_Source source, CS_EventKind kind);
  void NotifySource(const SourceImpl& source, CS_EventKind kind);
  void NotifySourceVideoMode(const SourceImpl& source, const VideoMode& mode);
  void NotifySourceProperty(const SourceImpl& source, CS_EventKind kind,
                            llvm::StringRef propertyName, int property,
                            CS_PropertyKind propertyKind, int value,
                            llvm::StringRef valueStr);
  void NotifySink(llvm::StringRef name, CS_Sink sink, CS_EventKind kind);
  void NotifySink(const SinkImpl& sink, CS_EventKind kind);
  void NotifySinkSourceChanged(llvm::StringRef name, CS_Sink sink,
                               CS_Source source);
  void NotifyNetworkInterfacesChanged();
  void NotifyTelemetryUpdated();

 private:
  Notifier();

  class Thread;
  wpi::SafeThreadOwner<Thread> m_owner;

  std::function<void()> m_on_start;
  std::function<void()> m_on_exit;
  static bool s_destroyed;
};

}  // namespace cs

#endif  // CSCORE_NOTIFIER_H_
