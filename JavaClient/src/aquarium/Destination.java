package aquarium;
import java.util.Date;

public class Destination{
  private Point address;
  private double time;

  public Destination(Point address, double time){
    Date date = new Date();

    this.address = address;
    this.time = time;
  }

  public Point getAddress(){
    return address;
  }
  public double getTime(){
    return time;
  }
  public void setAddress(Point address){
    this.address = address;
  }
  public void setTime(double time){
    this.time = time;
  }
}
