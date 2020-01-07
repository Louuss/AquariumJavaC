package aquarium;


public class Size{
  private double width;
  private double height;
  private Size screenSize;

  public Size(double width, double height, Size screenSize){
    this.screenSize=screenSize;
    this.width =  Math.floor(width/100 * screenSize.getWidth());
    this.height =  Math.floor(height/100 * screenSize.getHeight());
  }

  public Size(double width, double height){
    this.width = width;
    this.height = height;
  }

  public double getWidth(){
    return width;
  }
  public double getHeight(){
    return width;
  }
  public void setWidth(double width){
    this.width = width;
  }
  public void setHeight(double height){
    this.height = height;
  }

  public int getPercentWidth(){
    return (int) Math.floor(width / screenSize.getWidth() * 100);
  }
  public int getPercentHeight(){
    return (int) Math.floor(height / screenSize.getWidth() * 100);
  }
}
