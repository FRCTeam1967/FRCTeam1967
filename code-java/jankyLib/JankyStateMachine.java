package org.janksters.jankyLib;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public abstract class JankyStateMachine<T extends Enum<T>> extends JankyTask{
	private T currentState;
	private T priorState;
	private String machineName;
	private boolean isFirstRun;

    public JankyStateMachine(T initialState) {
        System.out.println("JankyStateMachine() constructor\n");
        
        machineName = "DefaultMachine";
        currentState = initialState;
        priorState = initialState;
        isFirstRun = true;
    }
    
    
    public void SetMachineName(String machine){
        machineName = machine;
    }
    public T GetCurrentState(){
        return currentState;
    }
    
    public void Run(){
        if (priorState != currentState || isFirstRun){
            StateEngine(currentState, true);
            isFirstRun = false;
            priorState = currentState;
        } else {
            StateEngine(currentState, false);
        }
        //priorState = currentState;
    }
    
    public void NewState(T newState, String reason){
        System.out.println("STATECHANGE: " +machineName+" OLD: "+currentState.toString()+" NEW: "+newState.toString()+" REASON: "+reason);
        SmartDashboard.putString("new", newState.toString());
        SmartDashboard.putString("old", currentState.toString());
        currentState = newState;
    }
    
    protected abstract void StateEngine(T curState, boolean onStateEntered);
}
