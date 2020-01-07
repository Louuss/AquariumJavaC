package aquarium;
import java.lang.*;
public class Point{
  private double x;
  private double y;
  private Size screenSize;

  public Point(double x, double y, Size screenSize){
    this.screenSize=screenSize;
    this.x = x/100 * screenSize.getWidth();
    this.y = y/100 * screenSize.getHeight();

  }

  public Point(double x, double y){
    this.x = x;
    this.y = y;

  }

  public double getX(){
    return x;
  }
  public double getY(){
    return y;
  }
  public void setX(double x){
    this.x = x;
  }
  public void setY(double y){
    this.y = y;
  }
  public int getPercentX(){
    return (int) Math.floor(x / screenSize.getWidth() * 100);
  }
  public int getPercentY(){
    return (int) Math.floor(y / screenSize.getWidth() * 100);
  }
}
