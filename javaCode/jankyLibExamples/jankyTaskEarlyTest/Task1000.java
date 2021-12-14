public class Task1000 extends JankyTask{
    public Task1000(){
        super("Task1000");
        System.out.println("called Task 1000 constructor");
    }
    public void Run(){
        System.out.println("Task 1000 is running");
        try {
            Thread.sleep(1000);
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }
}
