# JankyStateMachineExample (Sample Derived Class of JankyStateMachine)

## Purpose:
JankyStateMachine is a base class that allows derived classes to tell the robot to complete a series of task by creating a list of states for the robot to be in, coding what the robot needs to complete while being in each state, and how/when to go between states.  
Here, JankyStateMachineExample is a sample class used to show the application of JankyStateMachine in autonomous (not based on any specific game)

## Benefits:
-establishes the order of states to run and how/when to go between those states
-being forced to stay in one state and having control over the transition between states ensure that there won't be conflict in instances where the use one mechanism could interfere with, or worse break, another mechanism
-as a derivative of jankyTask, the programmer doesn't have to worry about calling run() or doing anything else to run the code beyond calling the constructor to start it (May still need to initialze any outside sensors/mechanisms and call any other added functions based on the derived class's use). Running jankyTask separately means that you can avoid errors that may result from either needing the caller to do things at the right time or having a task that takes a while to complete in Run() (which may lead to other code not running or sensors/joysticks not being listened to often enough)

## How it works
### Member Variables within JankyStateMachine:
-`currentState`: integer that holds which state we're currently on, class members can check this to know what state we're currently processing
-`priorState`: intenger that holds the state from the previous call, used in Run()
-`machineName`: char array (string) that holds the name of the state machine
-`name`: 2D char array (string array) that holds the name for each state

### Functions within JankyStateMachine:
-`JankyStateMachine()`: constructor, which fills all member variables with default data.  Notably, priorState is initialized to -1 because it must be different from currentState on the first run (view Run()).
-`virtual ~JankyStateMachine(void)`: virtual destructor
-`void SetMachineName(const char* machine)`: changes Machine Name if passed in name fits character length limit
-`int GetCurrentState()`: returns current state
-`virtual void Run()`: runs StateEngine by passing in currentState and true/false based on whether or not the state has changed since the last time the function was called (view StateEngine under JankyStateMachineExample)
-`void NewState()`: changes which state we're on/accessing by changing currentState
-`void setName()`: changes state name held in name array at given position (each name is linked to a state number)
-`StateEngine(int curState, bool onStateEntered)`: pure virtual function that must be implemented in derived class, which will contain code for what actually needs to be done in each state.  curState is the currently running state, and onStateEntered tells StateEngine if it's just calling a new state for the first time so StateEngine can take action at the moment a state is entered
. Called by Run().

### Member Variables and defined constants within JankyStateMachineExample:
-`#define STARTUPDELAY 3`: for this example, the delay at the start of autonomous is set 3.  Realistically, there should be a selector (or some other way to get driver's input) to decide how long the initial delay is
-`#define WHEEL_CIRCUMFERENCE_INCHES 6 * wpi::math::pi` and `#define PULSES_PER_REVOLUTION 4096`: these are both used to translate the returned encoder value (pulses) to distance in inches (refer to TalonSRX encoder documentation for more info)
-`enum AutoStateValues`: enum list to link integer values with the different states
-pointers for drive and different sensors (refer to their relative documentation for more info)

### Functions within JankyStateMachineExample:
-`JankyStateMachineExample(DifferentialDrive * _pDrive, WPI_TalonSRX * _pFLMotor, WPI_TalonSRX * _pFRMotor, frc::Timer * _pTimer, frc::ADIS16470_IMU * _pGyro, DigitalInput * _pSwitch, VisionSubsystem * _pVision)`: Constructor, which takes in pointers from robot.cpp for drive and sensors.  This way, instantiation and other uses for these sensors can be done outside of the JankyStateMachineExample class.  The constructor also sets the machine name and the different state names (linked to each enum value).  Additionally, the constructor will call Start() (a function in jankyTask), which starts running Run() without a need for Run() to be called anywhere else (including robot.cpp)
-`~JankyStateMachineExample()`: destructor is left empty since the handling of the different pointers and their deletion will be done outside of the class
-`StateEngine()`: Function which takes actions for each state in the state machine.  This includes changing currentState once a condition is met at the end of each state and, at the beginning, completing on-entry actions like resetting necessary sensors if onStateEntered is true, which, from how Run() works (view Run under JankyStateMachine), means that the state just changed.  This way, the code will only reset the sensors once instead of repeatedly resetting every time StateEngine is called. 
-`GoAutonomous()`: When the StateMachine first starts, the initial state is Idle.  Since Run() starts being called the moment the constructor is called (because that is where Start() is called), having a separate Idle state prevents autonomous code from running before it's ready (in this case, in the span of time between AutonomousInit and AutonomousPeriodic).  The if statement makes it so that the condition will only change the first time GoAutonomous() is called since it will be repeatedly called in AutonomousPeriodic()
-the remaining functions are used to reset and get data from the sensors (refer to their relative documentation for more info.)  Note that VisionSubsystem here is not a real class, though in a real game needing vision, you will need to retrieve data from vision.

### Implementating JankyStateMachineExample in robot.cpp
-`JankyStateMachineExample * jankyStateMachine = NULL;`: create a JankyStateMachineExample object called jankyStateMachine and initialize it to null
-`if (jankyStateMachine == NULL){`: on the chance that AutonomousInit is called multiple times (it shouldn't, but in case), this prevents the code from making multiple JankyStateMachineExample objects as that can lead to problems if they tell a mechanism to run conflicting code.  Since jankyStateMachine is originally initalized to NULL, this ensures that it will only be "new"ed the first time it's called
-`jankyStateMachine = new JankyStateMachineExample(pDrive, pFLMotor, pFRMotor, pTimer, pGyro, pSwitch, pVision)`: dynamically creates a JankyStateMachineExample object by passing in the sensors, which were previously initialized in robot.cpp (view documentation on the sensors to get more info on how they work).  Since Start() is in the constructor, this means that jankyTask will start calling Run().
-`jankyStateMachine -> GoAutonomous()` Once in AutonomousPeriodic, this takes currentState out of Idle so it can run the autonomous states (view GoAutonomous under JankyStateMachineExample for more info)

## How to use
### Prep
All that being said, the basics of using jankyStateMachine starts off with figuring out what states (action tasks) you need and what the transition between those states will be (view diagram for example).  
### Sample Project Template (as of 2021)
1. click the "W" at the top right of VSCode and Select "WPILib: Create New Project"
2. Select template, cpp, and Timed Robot 
3. Select the project's folder location
4. Check the checkbox next to 'Create a New Folder'
5. Enter '1967' as our Team Number
### Code
Following the example code, you will need to at the very least set the machine name (`SetMachineName()`), set the name for the different states (`SetName`), write the code for what the states do in `StateEngine`, including the transition between states (can be done based on sensors from robot, timer, joystick feedback, etc.), and call `Start` at the end of the constructor (needs to be at the end so all the set-up has been done before Run() is called).  Depending on the use, you may also need to reset sensors between states (here in JankyStateMachineExample, it is done upon entering a new state to ensure that the change has happened), a function like GoAutonomous() to transition between the inital state at the time of initialization and when the "main" StateMachine is being used, and a constructor to pass in pointers from outside mechanisms so you don't have to handle them in your derived class.
### Finding Files
-Adding JankyLib: drag the classes and include files from jankyLib into the left bar of VSCode.  When prompted, select "Add Folder to Workspace", not copy, so you can access/use the already existing files.
-For vsCode to find those files, you will also need to open build.gradle and add `srcDir '../../classes'` under `model { components { sources.cpp {source` and `srcDir '../../include'` under `model { components { sources.cpp {exportedHeaders`.  The way this works is by giving VSCode the path to those files starting from your project folder (src) and going back up with each `../` until it finds jankyLib (in this case, going up through the jankyStateMachineExample and SampleProjects folders), where it would need to look inside the classes and include folders. 
-Depending on the situation, you can do let robot.cpp access the source code and header file for the derived class (like JankyStateMachineExample.cpp and JankyStateMachineExample.h) by copying and pasting it into src/main/cpp (with robot.cpp) in Finder/WindowsExplorer.  In the main year's code, you can also give robot.cpp access to these files by putting the files in a classes/include folder, dragging those files into the left bar of VS code, and putting its path in build.grade like with JankyLib here.
-Finally, based on the mechanisms you're using, you will need to add support files (ex: Phoenix.json) in vendordeps.  If these files already exist in other code, you can just copy and paste them over in Finder/WindowsExplorer.  If not, you will need to install them.

## How JankyStateMachine Might Be Used:
Autonomous-going through one state at a time can be helpful when running an autonomous path that includes completing several tasks.  For more complex autonomous where there are multiple paths/options based on a selector, you can have multiple branches/options for what state to go to after a given state is completed (use a diagram to help visualize; jankyStateMachineExample does have examples of branches and looping) or, since they are independent of each other, you can code multiple state machines and have a selector decide which one to run.  Be careful to make sure you aren't running multiple state machines that user overlapping mechanisms/sensors at once since that may lead to conflicts of what the robot needs to do
Mechanisms-As mentioned earlier, using JankyStateMachine is helpful in mechanisms that need to complete multiple actions (or states) to fulfill their role.  It's especially benefical since being forced to stay in one state will prevent instances where the use of a mechanism could destory another mechanism if it gets in the way
