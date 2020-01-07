package fishnet;
import starter.Starter;


public class Pinger implements Runnable{
    int pinged;

    public Pinger()
    {
        pinged = 0;
    }

    public void setPinged(int pinged) {
        this.pinged = pinged;
    }

    public void run(){

        while (true)
        {
            try {
                Thread.sleep(Starter.timeout * 1000);
            }
            catch(Exception e){
                System.out.println(e);
            }
            finally {
                if(pinged == 0)
                    Starter.fishNet.send("ping " + Starter.port);
                else
                    setPinged(0);

            }

        }


    }




}
