package org.janksters.CommonClassesThisYear;

import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class AutoSelector {

    //selectors 
    SendableChooser<Integer> autoPaths = new SendableChooser<>();
    SendableChooser<Integer> actualAutoPaths = new SendableChooser<>(); //= new SendableChooser<>();
    //SendableChooser<Integer> robotSelector = new SendableChooser<>();


    // paths 
    private int ZeroDelay = AutoConstants.ZeroDelay;
    private int OneDelay = AutoConstants.OneDelay;
    private int  TwoDelay = AutoConstants.TwoDelay;
    private int ThreeDelay = AutoConstants.ThreeDelay;
    private int FourDelay = AutoConstants.FourDelay;
    private int FiveDelay = AutoConstants.FiveDelay;

    private int SimpleTarmac = AutoConstants.SimpleTarmac;
    private int Standard = AutoConstants.Standard;
    private int  HailJulie = AutoConstants.HailJulie;

    // private int doNothing = AutoConstants.DO_NOTHING;
    // private int testPath1 = AutoConstants.TEST_PATH_1;
    // private int testPath2 = AutoConstants.TEST_PATH_2;
    // private int testPath3 = AutoConstants.TEST_PATH_3;

    //robot used:
    //private int usingJankybot = AutoConstants.JANKYBOT;
    //private int usingLuca = AutoConstants.LUCA;

    private int autoMode;
    private int pathUsed;

    public AutoSelector() {

        autoMode = AutoConstants.ZeroDelay; //set default mode to do nothing
        pathUsed = AutoConstants.Standard;
    }

    public void DisplayAutoOptions() {
        autoPaths.setDefaultOption("0", ZeroDelay);
        autoPaths.addOption("1", OneDelay);
        autoPaths.addOption("2", TwoDelay);
        autoPaths.addOption("3", ThreeDelay);
        autoPaths.addOption("4", FourDelay);
        autoPaths.addOption("5", FiveDelay);
        
        
        SmartDashboard.putData("Auto Paths", autoPaths);
        
    
        // robotSelector.setDefaultOption("Jankybot", usingJankybot);
        // robotSelector.addOption("Luca", usingLuca);
        // SmartDashboard.putData("Selected Robot", robotSelector);
    }

    public void DisplayActualAutoOptions(){
        actualAutoPaths.setDefaultOption("Simple Tarmac", SimpleTarmac);
        actualAutoPaths.addOption("Standard", Standard);
        actualAutoPaths.addOption("Hail Julie", HailJulie);

        SmartDashboard.putData("Auto Paths for Real", actualAutoPaths);
    }
    
    public void PrintValues() {

        //paths
        // System.out.println("simple path: " + simplePath);
        // System.out.println("bounce path: " + bouncePath);
        // System.out.println("barrel racing path: " + barrelPath);
        // System.out.println("slalom path " + slalomPath);
        // System.out.println("do nothing: " + doNothing);
        // System.out.println("test path 1: " + testPath1);
        // System.out.println("test path 2: " + testPath2);
        // System.out.println("test path 3:" + testPath3);
    
        // System.out.println("Jankybot:" + usingJankybot);
        // System.out.println("Luca: " + usingLuca);
    }

    public int getAutoMode() {
        // variables for selected positions
        int autoMode = autoPaths.getSelected();
        //pathUsed = actualAutoPaths.getSelected();
      
        // print values
        SmartDashboard.putNumber("selected delay: ", autoMode);
        SmartDashboard.putNumber("selected actual auto path: ", autoPaths.getSelected());
        return autoMode;
    }

    public int getActualAutoMode(){
        int pathUsed = actualAutoPaths.getSelected(); 
        SmartDashboard.putNumber("selected actual auto path: ", pathUsed);
        return pathUsed;
    }

//    public int getSelectedRobot() {
//          robotUsed = robotSelector.getSelected();
    
//         System.out.println ("Selected robot: " + robotUsed);
    
//         return robotUsed;
//     }
}