public class Main {
  public static void main(String[] args) {
    System.out.println("This code is outside of the thread");
    Test test1 = new Test();

    Task500 task500Test = new Task500();
    Task1000 task1000Test = new Task1000();

    System.out.println("Task500's name is " + task500Test.getName());
    System.out.println("Task1000's name is " + task1000Test.getName());

    System.out.println("calling whichThread in Main before start: ");

    System.out.println("Main calls Pause");
    task500Test.Pause();
    task1000Test.Pause();

    for (int i = 0; i < 5; i++){
      System.out.println("Main is running");
      try {
          Thread.sleep(1000);
      }
      catch (Exception e) {
          System.out.println(e);
      }
    }

    System.out.println("Main calls Start");
    task500Test.start();
    task1000Test.start();

    for (int i = 0; i < 5; i++){
      System.out.println("Main is running");
      try {
          Thread.sleep(1000);
      }
      catch (Exception e) {
          System.out.println(e);
      }
    }

    System.out.println("Main calls Pause");
    task500Test.Pause();
    task1000Test.Pause();

    for (int i = 0; i < 5; i++){
      System.out.println("Main is running");
      try {
          Thread.sleep(1000);
      }
      catch (Exception e) {
          System.out.println(e);
      }
    }
    
    System.out.println("Main calls Start()");
    task500Test.start();
    task1000Test.start();

    for (int i = 0; i < 5; i++){
      System.out.println("Main is running");
      try {
          Thread.sleep(1000);
      }
      catch (Exception e) {
          System.out.println(e);
      }
    }

    System.out.println("Main calls Terminate()");
    task1000Test.Terminate();
    task500Test.Terminate();

    for (int i = 0; i < 5; i++){
      System.out.println("Main is running");
      try {
          Thread.sleep(1000);
      }
      catch (Exception e) {
          System.out.println(e);
      }
    }
  }
}