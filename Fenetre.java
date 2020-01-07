import java.awt.Color;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Fenetre extends JFrame {

  Panneau pan;
  public Fenetre(){
    this.setTitle("Java");
    this.setSize(1000, 1000);
    this.setLocationRelativeTo(null);

    //Instanciation d'un objet JPanel
    pan = new Panneau("aaaaaaaaaaaaa");
    //Définition de sa couleur de fond

    //On prévient notre JFrame que notre JPanel sera son content pane
    pan.setBackground(Color.ORANGE);
    this.setContentPane(pan);
    this.pack()
    this.setVisible(true);

    this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

  }
  void go()
  {
      pan.repaint();
      System.out.print("pan repainted"+pan.getS());

  }

}
