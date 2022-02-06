package frc.robot;
import edu.wpi.first.wpilibj.Timer;
import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class JankyStateMachineExamples extends JankyStateMachine{
    
    private final int Idle = 0;
    private final int Pause5S = 1;
    private final int PrintIterations = 2;
    private final int PrintYPressed = 3;
    private final int Pause2S = 4;
    private final int DoNothing = 5;

    /*
    enum StateValues{
        Idle(0), Pause5S(1), PrintIterations(2), PrintYPressed(3), Pause2S(4), DoNothing(5);
        
        private final int value;
        private StateValues(int _value){
            value = _value;
        }
        public int getValue(){
            return value;
        }   
    }  */
    private Timer timer;
    private int i;
    private jankyXboxJoystick XboxController;

    public JankyStateMachineExamples(){
        XboxController = new jankyXboxJoystick(2);
        timer=new Timer();
        i=0;
        SetMachineName("JankyStateMachineExamples");
        SetName(Idle,"Idle");
        SetName(Pause5S,"Pause5S");
        SetName(PrintIterations,"PrintIterations");
        SetName(PrintYPressed,"PrintYPressed");
        SetName(Pause2S,"Pause2S");
        SetName(DoNothing,"DoNothing");

        SmartDashboard.putBoolean("new timer", true);//delete later
        
        start();
    }
    public void StateEngine(int curState, boolean onStateEntered){
        //System.out.println("on state entered: " + onStateEntered);
        switch (curState){
            case Idle:
                break;
            case Pause5S:
                if (onStateEntered){
                    timer.reset();
                    timer.start();
                    SmartDashboard.putBoolean("new timer", true);
                }
                SmartDashboard.putBoolean("onStateEntered value", onStateEntered);
                SmartDashboard.putNumber("timer", timer.get());
                //System.out.print(timer.get() + " ");
                System.out.println("\n");
                if (timer.get()>=5){
                    NewState(PrintIterations,"timer reached 5");
                    timer.stop();
                }
                break;
            case PrintIterations:
                SmartDashboard.putNumber("iteration", i++);
                //System.out.print(i+" ");
                //i++;
                
                SmartDashboard.putBoolean("ButtonX", XboxController.GetButtonX());
                if (XboxController.GetButtonX()) {
                    NewState(Pause2S,"x button pressed");
                }
                SmartDashboard.putBoolean("ButtonY",XboxController.GetButtonY() );

                if (XboxController.GetButtonY()) {
                    NewState(PrintYPressed,"y button pressed");
                } 
                
                break;
            case Pause2S:
                if (onStateEntered){
                    timer.reset();
                    timer.start();
                }
                SmartDashboard.putBoolean("onStateEntered value", onStateEntered);
                SmartDashboard.putNumber("timer", timer.get());
                //System.out.println(timer.get());
                if (timer.get()>=2){
                    NewState(DoNothing,"timer reached 2");
                    timer.stop();
                }
                break;
            case PrintYPressed:
                System.out.println("\nY was pressed");
                NewState(DoNothing,"straight from Y pressed");
                break;
            case DoNothing:
                break;

        }
    }

    public void startStateMachine(){
        if (GetCurrentState() == Idle) {
            NewState(Pause5S, "starting state machine");
        }
    }

    public double getTimer(){
        return timer.get();
    }

    public void startTimer(){
        timer.start();
    }
    
    public void DisplayCurrentState(){
        SmartDashboard.putNumber("current state", GetCurrentState());
    }

    public void DisplayButtonX(){
        SmartDashboard.putBoolean("button X", XboxController.GetButtonX());
        //return XboxController.GetButtonX();
    }

}
