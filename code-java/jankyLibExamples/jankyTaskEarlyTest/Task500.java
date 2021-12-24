public class Task500 extends JankyTask{
    public Task500(){
        System.out.println("called Task 500 constructor");
    }
    public void Run(){
        System.out.println("Task 500 is running");
        System.out.println("About to terminate Task 500 within Run()");
        Terminate();
        try {
            Thread.sleep(500);
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }
}
