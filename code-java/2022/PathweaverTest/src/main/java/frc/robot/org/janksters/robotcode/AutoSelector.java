package frc.robot.org.janksters.robotcode;

import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class AutoSelector {

    //selectors 
    SendableChooser<Integer> autoPaths = new SendableChooser<>();
    SendableChooser<Integer> robotSelector = new SendableChooser<>();


    // paths 
    private int simplePath = AutoConstants.SIMPLE_PATH;
    private int bouncePath = AutoConstants.BOUNCE_PATH;
    private int  barrelPath = AutoConstants.BARREL_PATH;
    private int slalomPath = AutoConstants.SLALOM_PATH;
    private int doNothing = AutoConstants.DO_NOTHING;
    private int testPath1 = AutoConstants.TEST_PATH_1;
    private int testPath2 = AutoConstants.TEST_PATH_2;
    private int testPath3 = AutoConstants.TEST_PATH_3;

    //robot used:
    private int usingJankybot = AutoConstants.JANKYBOT;
    private int usingLuca = AutoConstants.LUCA;

    private int autoMode;
    private int robotUsed;

    public AutoSelector() {
        autoMode = AutoConstants.DO_NOTHING; //set default mode to do nothing
        robotUsed = AutoConstants.JANKYBOT;
    }

    public void DisplayAutoOptions() {
        autoPaths.setDefaultOption("Do Nothing", doNothing);
        autoPaths.addOption("Simple Path", simplePath);
        autoPaths.addOption("Bounce Path", bouncePath);
        autoPaths.addOption("Barrel Path", barrelPath);
        autoPaths.addOption("Slalom Path", slalomPath);
        autoPaths.addOption("Test Path 1", testPath1);
        autoPaths.addOption("Test Path 2", testPath2);
        autoPaths.addOption("Test Path 3", testPath3);
        SmartDashboard.putData("Auto Paths", autoPaths);
    
        robotSelector.setDefaultOption("Jankybot", usingJankybot);
        robotSelector.addOption("Luca", usingLuca);
        SmartDashboard.putData("Selected Robot", robotSelector);
    }

    public void PrintValues() {

        //paths
        System.out.println("simple path: " + simplePath);
        System.out.println("bounce path: " + bouncePath);
        System.out.println("barrel racing path: " + barrelPath);
        System.out.println("slalom path " + slalomPath);
        System.out.println("do nothing: " + doNothing);
        System.out.println("test path 1: " + testPath1);
        System.out.println("test path 2: " + testPath2);
        System.out.println("test path 3:" + testPath3);
    
        System.out.println("Jankybot:" + usingJankybot);
        System.out.println("Luca: " + usingLuca);
    }

    public int getAutoMode() {
        // variables for selected positions
        int autoMode = autoPaths.getSelected();
    
        // print values
        System.out.println("selected path: "+ autoMode);
        return autoMode;
    }

   public int getSelectedRobot() {
         robotUsed = robotSelector.getSelected();
    
        System.out.println ("Selected robot: " + robotUsed);
    
        return robotUsed;
    }
}
