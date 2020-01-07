package aquarium;

import java.util.*;
import java.awt.Color;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Aqua extends JFrame implements Runnable {


  private List<Fish> fishies;
  private JPanel rootPanel;

  public Aqua(Size screenSize){
    this.setTitle("Aqua");
    this.setSize((int) screenSize.getWidth(), (int) screenSize.getHeight());

    fishies = new ArrayList<Fish>();

    this.setLocationRelativeTo(null);

    rootPanel = new JPanel();
    rootPanel.setBackground(Color.BLUE);
    rootPanel.setLayout(null);
    this.setContentPane(rootPanel);

    this.setVisible(true);
    this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

  }

  public void run()
  {
    System.out.println(Thread.currentThread().getName()
    + ", executing run() method!");

    while(true){
      //if(d.getTime() - prevtime > (1000/FPS)){

      this.rePaint();
      //}


      try{
        Thread.sleep(10);
      }catch(InterruptedException e) {
        e.printStackTrace();
      }

    }
  }

  public void startFish(String fish)
  {
      if (getFish(fish) != null)
      {
          getFish(fish).startFish();
          System.out.println("    -> OK");
      }else{
          System.out.println("    -> Ce poison n'est pas present");
      }

  }


  public void removeFish(String fish)
  {
      if (getFish(fish) != null)
      {
        Fish theFish = getFish(fish);
        fishies.remove(theFish);
        rootPanel.remove(theFish);
        rootPanel.revalidate();
        rootPanel.repaint();
        System.out.println("    -> OK");

      }else{
        System.out.println("    -> Ce poisson n'est pas present");
      }

  }

  public int getNumFish(){
    return fishies.size();
  }

  public String status(){
    String s = "";
    for (Fish fish : fishies) {
			s = s + fish.getFishData() + '\n';
		}
    return s;
  }
  public void addFish(Fish fish){

    if(getFish(fish.getId()) == null){
      fishies.add(fish);
      rootPanel.add(fish);
      fish.setVisible(true);
      System.out.println("    --> OK");
    }else{
      System.out.println("Fish with id <" + fish.getId() + "> already exists.");
    }


  }

  public Fish getFish(String fishName){
    for (Fish fish : fishies) {
			if(fish.getId().equals(fishName)){
        return fish;
      }
		}
    return null;
  }

  public void rePaint()
  {
    rootPanel.repaint();
  }

  public List<Fish> getFishes()
  {
    return fishies;
  }

  public void printStatus()
  {

      if (fishies.isEmpty())
      {
          System.out.println("  -> Aucun poisson");
          return; // empty
      }

      System.out.println(fishies.size() + " poisson(s) trouvé(s)" );

      for(Fish fish : fishies)
      {
          System.out.println("  Fish : " + fish.getFishData());
      }
  }


}
