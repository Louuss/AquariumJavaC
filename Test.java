import javax.swing.JFrame;
import java.util.Scanner;

public class Test {

  public static void main(String[] args){
    Fenetre fenetre = new Fenetre();
    fenetre.setVisible(true);




        String userName="";
        fenetre.go();


        try{
            Thread.sleep(1000); //Ici, une pause d'une seconde
          }catch(InterruptedException e) {
            e.printStackTrace();
          }



        fenetre.pan.setX(fenetre.pan.getXs()+10);
        fenetre.go();

      }


  }



}
