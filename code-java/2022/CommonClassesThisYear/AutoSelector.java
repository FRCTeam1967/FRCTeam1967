package org.janksters.CommonClassesThisYear;

import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class AutoSelector {

    //selectors 
    SendableChooser<Integer> autoPaths = new SendableChooser<>();
    SendableChooser<Integer> actualAutoPaths = new SendableChooser<>(); //= new SendableChooser<>();


    // paths for the delay
    private int ZeroDelay = AutoConstants.ZeroDelay;
    private int OneDelay = AutoConstants.OneDelay;
    private int  TwoDelay = AutoConstants.TwoDelay;
    private int ThreeDelay = AutoConstants.ThreeDelay;
    private int FourDelay = AutoConstants.FourDelay;
    private int FiveDelay = AutoConstants.FiveDelay;
    //paths for the moving
    private int SimpleTarmac = AutoConstants.SimpleTarmac;
    private int Standard = AutoConstants.Standard;
    private int  HailJulie = AutoConstants.HailJulie;


    //idk what is happening here but we are keeping it just in case
    private int autoMode;
    private int pathUsed;

    public AutoSelector() {

        autoMode = AutoConstants.ZeroDelay; //set default mode to do nothing
        pathUsed = AutoConstants.Standard;
    }


    //display the delays
    public void DisplayAutoOptions() {
        autoPaths.setDefaultOption("0", ZeroDelay);
        autoPaths.addOption("1", OneDelay);
        autoPaths.addOption("2", TwoDelay);
        autoPaths.addOption("3", ThreeDelay);
        autoPaths.addOption("4", FourDelay);
        autoPaths.addOption("5", FiveDelay);
        
        
        SmartDashboard.putData("Auto Paths", autoPaths);
        
    
       
    }

    //displays the auto paths
    public void DisplayActualAutoOptions(){
        actualAutoPaths.setDefaultOption("Simple Tarmac", SimpleTarmac);
        actualAutoPaths.addOption("Standard", Standard);
        actualAutoPaths.addOption("Hail Julie", HailJulie);

        SmartDashboard.putData("Auto Paths for Real", actualAutoPaths);
    }
    
    //which one is selected -- prints out the delay
    public int getAutoMode() {
        int autoMode = autoPaths.getSelected();
       
        SmartDashboard.putNumber("selected delay: ", autoMode);
        SmartDashboard.putNumber("selected actual auto path: ", autoPaths.getSelected());
        return autoMode;
    }

    //prints out the path
    public int getActualAutoMode(){
        int pathUsed = actualAutoPaths.getSelected(); 
        SmartDashboard.putNumber("selected actual auto path: ", pathUsed);
        return pathUsed;
    }
}