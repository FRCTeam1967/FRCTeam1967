package org.janksters.CommonClassesThisYear;


import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class AutoSelector {

    //selectors 
    SendableChooser<Integer> delaySelector = new SendableChooser<>();
    SendableChooser<Integer> pathSelector = new SendableChooser<>(); //= new SendableChooser<>();


//     // paths for the delay
    private int ZeroDelay = AutoConstants.ZeroDelay;
    private int OneDelay = AutoConstants.OneDelay;
    private int TwoDelay = AutoConstants.TwoDelay;
    private int ThreeDelay = AutoConstants.ThreeDelay;
    private int FourDelay = AutoConstants.FourDelay;
    private int FiveDelay = AutoConstants.FiveDelay;
    // //paths for the moving
    private int SimpleTarmac = AutoConstants.SimpleTarmac;
    private int twoBall = AutoConstants.twoBall;
    private int threeBall = AutoConstants.threeBall;


//     //idk what is happening here but we are keeping it just in case
    private int delaySelected;
 //private int pathUsed;

    public AutoSelector() {
        delaySelected = 0; //set default mode to do nothing
    }


//     //display the delays
    public void DisplayDelayOptions() {
        delaySelector.setDefaultOption("0", ZeroDelay);
        delaySelector.addOption("1", OneDelay);
        delaySelector.addOption("2", TwoDelay);
        delaySelector.addOption("3", ThreeDelay);
        delaySelector.addOption("4", FourDelay);
        delaySelector.addOption("5", FiveDelay);
        
        SmartDashboard.putData("Delay Options", delaySelector);
    }

    //displays the auto paths
    public void DisplayPathOptions(){
        pathSelector.setDefaultOption("Simple Tarmac", SimpleTarmac);
        pathSelector.addOption("2 ball", twoBall);
        pathSelector.addOption("3 ball", threeBall);

        SmartDashboard.putData("Auto Paths for Real", pathSelector);
    }
    
//     //which one is selected -- prints out the delay
    public int getDelaySelected() {
        int delaySelected = delaySelector.getSelected();
        SmartDashboard.putNumber("selected delay: ", delaySelected);
        return delaySelected;
    }

    //prints out the path
    public int getPathSelected(){
        int pathUsed = pathSelector.getSelected(); 
        SmartDashboard.putNumber("selected actual auto path: ", pathUsed);
        return pathUsed;
    }
}