package fishnet;

import java.net.*;
import java.io.*;

import starter.*;


public class FishNet implements Runnable{

  public Thread pingerThread;
  public static Pinger pinger;
  String hostname;
  int port;
  private String id;
  private PrintWriter out;
  private Boolean connected = false;

  public FishNet(String hostname,  int port, String id /*Starter parent*/){
    this.hostname = hostname;
    this.port = port;
    this.id = id;
    pinger = new Pinger();

    pingerThread = new Thread(pinger);
    pingerThread.start();

  }
  private void sayHello(){
    if(id != "noId"){
      send("hello in as " + id);
    }else{
      send("hello");
    }

  }

  private void ping(){
      send("ping "+ port);
  }
  private void logOut(){send("log out");}
  private void ls(){send("ls");}
  private void getFishesContinuously(){ send("getFishesContinuously");}

  public void setId(String id){
    this.id = id;
  }
  public void run(){
    System.out.println("Connecting to " + hostname + ":" + port +  "...");

    try (Socket socket = new Socket(hostname, port)) {
        System.out.println("Connected.");

        connected = true;


        InputStream input = socket.getInputStream();
        InputStreamReader reader = new InputStreamReader(input);
        out = new PrintWriter(socket.getOutputStream(), true);

        sayHello();



        int character;
        StringBuilder data = new StringBuilder();
        send("getFishesContinuously");
        while(true){
          while ((character = reader.read()) != '\0' ) {

              data.append((char) character);

          }

          //System.out.println(data.toString());
          Starter.parser.parse(data.toString());
          data.delete(0, data.length());
          data.setLength(0);

        }


    } catch (UnknownHostException ex) {

        System.out.println("Server not found: " + ex.getMessage());

    } catch (IOException ex) {

        System.out.println("I/O error: " + ex.getMessage());
    }

    connected = false;
  }

  public Boolean isConnected(){
    return connected;
  }
  public void send(String msg){
    if(isConnected()){
      out.println(msg);
      pinger.setPinged(1);
    }else{
      System.out.println("Not Connected");
    }

  }

  public void printStatus(){
    if(connected){
      System.out.println("OK connected to controler.");
    }else{
      System.out.println("Not connected");
    }


  }


}
