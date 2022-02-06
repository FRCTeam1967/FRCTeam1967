
package org.janksters.jankyLib;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public abstract class JankyStateMachine extends JankyTask{
    private final int MAX_NAMES = 30;

	private int currentState;
	private int priorState;
	private String machineName;

    protected String[] names = new String[MAX_NAMES];

    public JankyStateMachine(){
        System.out.println("JankyStateMachine() constructor\n");
        
        for (int i=0; i< MAX_NAMES; i++){
            SetName(i, "State-Default-"+i);
        }

        machineName = "DefaultMachine";
        currentState = 0;		// Make a sane initial choice.
        priorState = -1;
    }
    
    
    public void SetMachineName(String machine){
        machineName = machine;
    }
    public int GetCurrentState(){
        return currentState;
    }
    
    public void Run(){
        if (priorState != currentState){
            StateEngine(currentState, true);
        } else {
            StateEngine(currentState, false);
        }
        priorState = currentState;
    }
    
    public void NewState(int newState, String reason){
        if ( newState < MAX_NAMES){
            System.out.println("STATECHANGE: " +machineName+" OLD: "+names[currentState]+" NEW: "+names[newState]+" REASON: "+reason);
            SmartDashboard.putString("ew", names[newState]);
            SmartDashboard.putString("old", names[currentState]);
            currentState = newState;
        } else{
            System.out.println("JankyStateMachine NewState ERROR invalid newState");
        }
        
    }
    
    public void SetName(int state, String stateName) {
        if (state < MAX_NAMES){   
            names[state] = stateName;
        } else {
            System.out.println("JankyStateMachine SetName Bad state value/name");
        }
    }

    protected abstract void StateEngine(int curState, boolean onStateEntered);



}