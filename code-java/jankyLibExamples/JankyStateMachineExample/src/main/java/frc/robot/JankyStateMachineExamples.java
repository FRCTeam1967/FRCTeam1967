package frc.robot;
import edu.wpi.first.wpilibj.Timer;
import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

enum ExampleState {
    Idle,
    Pause5S,
    PrintIterations,
    PrintYPressed,
    Pause2S,
    DoNothing;

    // By default, Java will provide a toString() method that will return a string value
    // identical to the constants used above. We can override that if we want to customize
    // the strings that would be used. 
    public String toString() {
        switch (this) {
            case Pause2S:
                return "Pause for 2s";
            case Pause5S:
                return "Pause for 5s";
            default:
                // Use the default for everything else
                return super.toString();
        }
    }
};

public class JankyStateMachineExamples extends JankyStateMachine<ExampleState> {
    private Timer timer;
    private int i;
    private jankyXboxJoystick XboxController;

    public JankyStateMachineExamples(){
        super(ExampleState.Idle);
        XboxController = new jankyXboxJoystick(2);
        timer=new Timer();
        i=0;
        SetMachineName("JankyStateMachineExamples");

        start();
    }

    public void StateEngine(ExampleState curState, boolean onStateEntered){
        switch (curState){
            case Idle:
                break;
            case Pause5S:
                if (onStateEntered){
                    timer.reset(); 
                    timer.start();
                    //SmartDashboard.putNumber("onStateEntered", Pause5S);
                    //SmartDashboard.putBoolean("new timer", true);
                }
                SmartDashboard.putNumber("timer", timer.get());
                //System.out.print(timer.get() + " ");
                System.out.println("\n");
                if (timer.get()>=5){
                    NewState(ExampleState.PrintIterations,"timer reached 5");
                }
                break;
            case PrintIterations:
                SmartDashboard.putNumber("iteration", i++);
                SmartDashboard.putBoolean("ButtonX", XboxController.GetButtonX());
                if (XboxController.GetButtonX()) {
                    NewState(ExampleState.Pause2S,"x button pressed");
                }
                SmartDashboard.putBoolean("ButtonY",XboxController.GetButtonY() );

                if (XboxController.GetButtonY()) {
                    NewState(ExampleState.PrintYPressed,"y button pressed");
                } 
                
                break;
            case Pause2S:
                if (onStateEntered){
                    timer.reset();
                    timer.start();
                    //SmartDashboard.putNumber("onStateEntered", Pause2S);
                }
                SmartDashboard.putBoolean("onStateEntered value", onStateEntered);
                SmartDashboard.putNumber("timer", timer.get());
                if (timer.get()>=2){
                    NewState(ExampleState.DoNothing,"timer reached 2");
                }
                break;
            case PrintYPressed:
                System.out.println("\nY was pressed");
                NewState(ExampleState.DoNothing,"straight from Y pressed");
                break;
            case DoNothing:
                break;

        }
    }

    public void startStateMachine(){
        if (GetCurrentState() == ExampleState.Idle) {
            NewState(ExampleState.Pause5S, "starting state machine");
        }
    }

    public double getTimer(){
        return timer.get();
    }

    public void startTimer(){
        timer.start();
    }
    
    public void DisplayCurrentState(){
        SmartDashboard.putString("current state ", GetCurrentState().toString());
    }

    public void DisplayButtonX(){
        SmartDashboard.putBoolean("button X", XboxController.GetButtonX());
        //return XboxController.GetButtonX();
    }

}
