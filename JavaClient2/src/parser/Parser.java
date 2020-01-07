package parser;

import aquarium.Fish;
import aquarium.Point;
import aquarium.Size;
import starter.Starter;

import java.util.*;



public class Parser {

  Size sSize;

  public Parser(Size screenSize)
  {
    this.sSize = screenSize;

  }


  public void parse(String s) {
    try {
        String[] tokens = s.split(" ");
        String cmd = tokens[0];

        switch (cmd) {
            case "status":
                Starter.fishNet.printStatus();
                //Starter.aqua.printStatus();
                Starter.fishNet.send(s);
                break;
            case "list":
                //System.out.println(s);
                parseFishList(s);
                break;
            case "addFish":
                Starter.fishNet.send(s);
                break;
            case "delFish":
            try{
                Starter.aqua.removeFish(tokens[1]);
                Starter.fishNet.send(s);
              }
              catch(Exception e){
                System.out.println("");
              }
                break;
            case "startFish":
                Starter.aqua.startFish(tokens[1]);
                Starter.fishNet.send(s);
                break;
            case "pong":
                break;
            case "NOK":
                System.out.println("NOK\n $ ");
                break;
            case "bye":
                System.out.println("bye\n $ ");
                break;
            case "OK":
                System.out.println(s);
                break;
            case"fish":
                System.out.println(s);
                break;
            case "greeting":
                System.out.println(s);
                break;
            default:
                System.out.println("   Commande non reconnue ");
                break;
        }
    }
        catch(Exception e)
        {
            System.out.println(e);
        }


  }



  public void listStatus(String s)
  {

  }

  public void parseFishList (String msg)
  {
    //list [PoissonRouge at 90x4,10x4,5] [PoissonClown at 20x80,12x6,5]
    // PoissonNain at 61x52, 4x3, RandomWayPoint

    //System.out.println(msg);
      String[] list = msg.split(" ");


      int destx, desty, height, length, time;
      String name;
      ArrayList<String> fishiesToKeep = new ArrayList<String>();


      for (int i = 1; i < list.length; i = i + 3)
      {


        //String[] fish = list[i].split(" ");// PoissonRouge at 90x4,10x4,5
        name = (new StringBuilder(list[i])).deleteCharAt(0).toString();

        fishiesToKeep.add(name);

        String numbers = (new StringBuilder(list[i + 2])).deleteCharAt(list[i + 2].length() - 1).toString();

        String[] numbersList = numbers.split(","); //90x4,10x4,5
        String[] position = numbersList[0].split("x");// 90 4
        destx = Integer.parseInt(position[0]);
        desty = Integer.parseInt(position[1]);


        String[] taille = numbersList[1].split("x");
        length = Integer.parseInt(taille[0]);
        height = Integer.parseInt(taille[1]);





        time = Integer.parseInt(numbersList[2]);




        Size s = new Size(length, height, sSize);
        Point p = new Point(destx, desty, sSize);

        Fish fishy = Starter.aqua.getFish(name);

        if (fishy != null) {
          fishy.addDestination(destx, desty, time*1000);
        } else {
          Fish newFish = new Fish(s, p, name, sSize);
          Starter.aqua.addFish(newFish);
          newFish.startFish();
        }
      }

      // System.out.println("names" + fishies2);
      // System.out.println("aqua" + Starter.aqua.getFishes() );
      //
      List<Fish> fishies = new ArrayList(Starter.aqua.getFishes());

      for (Fish fishy : fishies) {

          int keep = 0;
          for (String fishname : fishiesToKeep) {
              if (fishname.equals(fishy.getId())) {
                  keep = 1;
                  break;
              }
          }
          if (keep == 0) {
              try {
                  Starter.aqua.removeFish(fishy.getId());
              } catch (Exception e) {
                  System.out.println("Poisson non présent");
              }
          }

      }

  }

    // for (Fish f :   Starter.aqua.getFishes())
    //     for (String fishname : fishies2) {
    //         if (fishname.equals(f.getId()))
    //             break;
    //         Starter.aqua.getFishes().remove(Starter.aqua.getFish(fishname));
    //     }



  }
