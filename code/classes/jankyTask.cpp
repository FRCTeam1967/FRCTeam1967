#include "jankyTask.h"
#include <string>

JankyTask::JankyTask(const char* taskName) {
  std::string name = taskName;
  if (!taskName)
  {
    name = "jankyTask-" + getTime();  // TODO:Holly - not sure what time function to call for a 'unique' number here.
  }

  enabled_ = false;
  running_ = true;

  task_ = new Task(name.c_str(), (FUNCPTR)JankyTask::JankyStarterTask, 200);
  task_->Start((UINT32)this);
}

JankyTask::~JankyTask(){
  task_->Terminate();
}

void JankyTask::JankyPrivateStarterTask(JankyTask* task) {
  while (running_) {
    if (task->enabled_) {
      task->Run();
      Wait(0.002);  // Only wait 2ms when task is active.
    }
    else
      Wait(0.05);   // 50 ms wait period while task is 'paused'
  }
}

void JankyTask::Start() {
  enabled_ = true;
}

void JankyTask::Pause() {
  enabled_ = false;
}

void JankyTask::Terminate() {
  running_ = false;
}