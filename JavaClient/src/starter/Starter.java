package starter;


import java.io.InputStream;
import java.io.FileInputStream;
import java.util.Properties;
import java.io.IOException;

import aquarium.*;
import prompt.*;
import parser.*;
import fishnet.*;



public class Starter {

    public static int FPS = 100;
    public static FishNet fishNet;
    public static Parser parser;
    public static Aqua aqua;
    public static Prompt prompt;
    public static String pathToImages;
    public static int timeout;
    public static String id;
    public static int port = 8080;



    public void start(){

      Properties prop = new Properties();
      InputStream inputStr = null;


      timeout = 30;
      pathToImages = "./";
      id = "noId";


      String address = "127.0.0.1";



      try {
        inputStr = new FileInputStream("affichage.cfg");
        prop.load(inputStr);
        address = prop.getProperty("controller-address","127.0.0.1");
        port = Integer.parseInt(prop.getProperty("controller-port","8080"));
        id = prop.getProperty("id","noId");
        timeout = Integer.parseInt(prop.getProperty("display-timeout-value","30"));
        pathToImages = prop.getProperty("resources","./");
       }
       catch (IOException ex)
       {
         System.out.println("Fichier de configuration introuvable");
       }
       finally
       {
         if (inputStr != null) {

           try
           {
               inputStr.close();
           }
           catch (IOException e)
           {
               e.printStackTrace();
           }
          }
       }

          Size screenSize = new Size(500,500);

          aqua = new Aqua(screenSize);
          aqua.setVisible(true);
          aqua.rePaint();



          Thread t1 = new Thread(aqua);
          t1.start();


          parser = new Parser(screenSize);

          prompt = new Prompt(parser);

          Thread tPrompt = new Thread(prompt);
          tPrompt.start();


          fishNet = new FishNet(address, port, id);

          Thread fishNetThread = new Thread(fishNet);
          fishNetThread.start();



      }
}
