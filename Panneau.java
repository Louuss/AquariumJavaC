

import java.awt.Graphics;
import java.awt.Image;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JPanel;
import java.util.Properties;

public class Panneau extends JPanel {
  String s="";
  int x = 0;
  int y = 0;

void setX(int c)
{
  this.x = c;
}

  public Panneau(String s)
  {
    this.s = s;
  }
  void setS(String s)
  {
    this.s=s;
  }
  String getS()
  {
    return s;
  }
  int getXs()
  {
    return this.x;
  }

  public void paintComponent(Graphics g){

    try {
      Image img = ImageIO.read(new File("home.jpg"));
      g.drawImage(img, x, x, this);
    }
    catch (IOException e) {
      e.printStackTrace();
    }
      //Pour une image de fond
      //g.drawImage(img, 0, 0, this.getWidth(), this.getHeight(), this);
      g.drawString(this.getS(), 10, 10);


  }
}
