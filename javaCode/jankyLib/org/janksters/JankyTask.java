/*
Purpose: The purpose of this JankyTask class is to allow the compiler to run 
2 separate chunks of code simultaenously so that they don't interfere with each 
other or cause lag.  This works by using the Thread class in Java, which lets
each thread run independent code within a process.  
To  Use: Classes derived of JankyTask will need to implement the
Run() method, and JankyTask will call Run() over and over while the task is running 
(not paused or terminated). Additionally, the code using that class will need to call
start() to start the task at the beginning or resume it if it's been paused, pause() 
to temporarily pause the task, and terminate() to permenantly exit the task.
*/

public abstract class JankyTask extends Thread{
    private boolean enabled_;
    private boolean running_;
    private boolean isDead_;

    private boolean baseThreadStarted;

    //constructor that doesn't take in a parameter, so it uses the time to generate a name for debugging purposes
    public JankyTask(){
        super("jankyTask-" + System.currentTimeMillis()); //TODO: change random to FPGATime whenn WPILib is available
        System.out.println("Called JankyTask constructor");
        running_ = true;
        enabled_ = false;
        isDead_ = false;

        baseThreadStarted = false;
    }

    //constructor that gets the name of the Task as a parameter for debugging purposes
    public JankyTask(String taskName){
        super(taskName);
        System.out.println("Called JankyTask constructor");
        running_ = true;
        enabled_ = false;
        isDead_ = false;

        baseThreadStarted = false;
    }

    //Run(): To be implemented by the derived class.  This method will be called over and over as long as the task is running (not terminated) and enabled (started and not paused)
    abstract void Run();

    //run(): Will be called once by the thread, and this checks if the task should be running.
    //It calls Run() with a small pause in between if it should be running and just has a pause if the task shouldn't be running
    public void run(){
        while (running_){
            if (enabled_){
                Run();
                try {
                    Thread.sleep(40);
                }
                catch (Exception e) {
                    System.out.println(e);
                }
            }
            else{
                try {
                    Thread.sleep(150);
                }
                catch (Exception e) {
                    System.out.println(e);
                }
            }
        }
        isDead_ = true;
        System.out.println("JankyTask is exiting");
    }

    //start(): allows you to start or resume the task
    public void start(){
        if (!baseThreadStarted){
            super.start(); 
            baseThreadStarted = true;
        }
        
        enabled_ = true;
        //System.out.println("Start-Enabled is " + enabled_);
    }

    //Pause(): temporarily pauses the task, though start() will resume it
    public void Pause(){
        enabled_ = false;
        //System.out.println("Pause-Enabled is " + enabled_);
    }

    //Terminate(): permenantely exits the tasks (exits the while loop in run())
    public void Terminate(){
        if (!running_){  //exits immediately if Terminate is called more than once 
            return;
        }
        running_ = false;
        //System.out.println("Terminante-Enabled is " + enabled_);

        // Above told the task to exit on the next loop around.
        // That could take 2ms or 50ms based on whether it's in pause or run and how long
        // the actual Run() routine takes too. So we have to wait until we're really terminated here.
        //System.out.println("Currently in Terminate()");
        if (!Thread.currentThread().getName().equals(this.getName())){  
            //Problem - If this code was called by Run() [same thread we're trying to kill], while(!isDead_) becomes a deadlock.
            //this solves the problem since it checks to make sure the calling thread is not the same as the derived class's thread.
            //If the derived class's Run() is calling Terminate(), the thread would be the derived class's thread.
            
            //System.out.println("Currently in Terminate if statement");
            while (!isDead_) {
            try {
                    Thread.sleep(1);
                }
                catch (Exception e) {
                    System.out.println(e);
                }
            }
        }
    }
}