package prompt;
import java.util.Scanner;
import parser.*;

public class Prompt implements Runnable {

  private Parser p;

  public Prompt(Parser parser){
    this.p=parser;
  }


  public void run()
  {
    Scanner myObj = new Scanner(System.in);
    while(true)
    {
      // Enter username and press Enter
      System.out.print("$ ");

      String cmd = "";

       cmd = myObj.nextLine();
       try {p.parse(cmd);}
       catch(java.lang.ArrayIndexOutOfBoundsException e)
       {
         System.out.println("    Pas assez d'arguments");
       }
    }
  }

}
