#include "frc/WPILib.h"
#include "jankyTask.h"
#include <string>


JankyTask::JankyTask(const char* taskName) {
  char oldstyle_name[128];

  if (!taskName)
  {
    sprintf(oldstyle_name, "jankyTask-%llu", frc::RobotController::GetFPGATime()); //changed GetFPGATime() to frc::RobotController::GetFPGATime()
  }

  enabled_ = false;
  running_ = true;
  isDead_ = false;

  pthread_create(&ptask, NULL, &JankyTask::JankyPrivateStarterTask, (void*)this);
  //task_ = new Task(oldstyle_name, (FUNCPTR)JankyTask::JankyPrivateStarterTask, this);
}

JankyTask::~JankyTask(){
	void* res;
	this->Terminate();
  pthread_join(ptask, &res);
  free(res);
}

void* JankyTask::JankyPrivateStarterTask(void* vtask) {
	JankyTask*task= (JankyTask*)vtask;
  while (task->running_) {
    if (task->enabled_ && frc::RobotState::IsEnabled()) {
      task->Run();
      frc::Wait(0.002);  // Only wait 2ms when task is active.
    }
    else
      frc::Wait(0.05);   // 50 ms wait period while task is 'paused'
  }

  task->isDead_ = true; // Falling off the edge of the earth...
  return NULL;
}

void JankyTask::Start() {
  enabled_ = true;
}

void JankyTask::Pause() {
  enabled_ = false;
}

void JankyTask::Terminate() {
  running_ = false;

  // Above told the task to exit on the next loop around.
  // That could take 2ms or 50ms based on whether it's in pause or run and how long
  // the actual Run() routine takes too. So we have to wait until we're really terminated here.
  while (!isDead_) {
    frc::Wait(0.02); // Wait until we're really dead on that task.
  }
}
