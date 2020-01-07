package aquarium;

import starter.Starter;

import java.util.*;
import java.awt.Graphics;
import java.awt.Image;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JPanel;
import java.lang.*;


public class Fish extends JPanel{
  private Size size;
  private Point position;
  private String id;


  private Queue<Destination> destinations;
  private Image img;
  private Size screenSize;
  public double moveTime=0;
  static int FPS = 100;
  private boolean moveFish = false;
  private boolean finishedMoving = false;

  public Fish (Size size, Point position, String id, Size screenSize){
    this.size = size;
    this.position = position;
    this.id = id;
    this.screenSize = screenSize;


    try {
      Image im = ImageIO.read(new File(Starter.pathToImages + id + ".jpg"));
      img = im.getScaledInstance((int)size.getWidth() , (int)size.getHeight(),Image.SCALE_DEFAULT);
    }
    catch (IOException e) {
      try{
      Image em = ImageIO.read(new File(Starter.pathToImages + "blobfish.jpg"));
      img = em.getScaledInstance((int)size.getWidth() , (int)size.getHeight(),Image.SCALE_DEFAULT);
      }
        catch(IOException e2){
          System.out.println(e2);
        }

    }

    destinations = new LinkedList<Destination>();
    this.setBounds(0 , 0, (int) size.getWidth(), (int) size.getHeight());
    moveTime = (new Date()).getTime();
  }

  public void addDestination(int x, int y, double time){
    if(finishedMoving){
      this.moveTime=(new Date()).getTime();
    }
    destinations.add(new Destination(new Point(x,y, screenSize), this.moveTime + time));
    this.moveTime +=time;
  }

  public void startFish(){

    this.moveTime=(new Date()).getTime();
    for (Iterator i = destinations.iterator(); i.hasNext();) {
      ((Destination) i.next()).setTime(((Destination)i.next()).getTime() + this.moveTime);
    }
    moveFish = true;

  }

  public boolean isStarted(){
    return moveFish;
  }

  public void setPosition(Point position){
    this.position=position;
  }

  public void calcPos(){


    if(!destinations.isEmpty() && moveFish){
      finishedMoving = false;
      Date date = new Date();

      double diffX = destinations.element().getAddress().getX() - position.getX();
      double diffY = destinations.element().getAddress().getY() - position.getY();
      double timeDiff = Math.abs(destinations.element().getTime() - date.getTime());

      if (((Math.abs(diffX) < 1) & (Math.abs(diffY) < 1)) || (timeDiff<10)){
        this.setPosition(destinations.element().getAddress());
        destinations.remove();

      }else{


        double framesLeft = Math.ceil((timeDiff/1000)*FPS);


        double addX = diffX/framesLeft;
        double addY = diffY/framesLeft;

        this.setPosition(new Point(position.getX()+addX, position.getY() + addY));

      }
    }

    if(destinations.isEmpty() && moveFish){
      finishedMoving = true;
    }
  }

  public String getId(){
    return this.id;
  }

  public void paintComponent(Graphics g){
    calcPos();
    g.drawImage(img, 0,0, null);
    this.setBounds((int) position.getX(), (int) position.getY(), (int) size.getWidth(), (int) size.getHeight());
    //g.drawRoundRec(position.getX(), position.getY(),10,10,10,10);

  }

  public String getFishData(){
    String ret = "";
    ret += id + " at " ;
    ret += position.getPercentX() +"x" + position.getPercentY();
    ret += "," + size.getPercentWidth() + "x" + size.getPercentHeight();
    if (moveFish){
      ret += " started";
    }else{
      ret += " notStarted";
    }
    return ret;
  }



}
