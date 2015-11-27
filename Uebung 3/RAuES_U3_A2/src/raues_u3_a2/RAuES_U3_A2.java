package raues_u3_a2;




/**
 *
 * @author Steffen
 */
public class RAuES_U3_A2 {

  /**
   * @param args the command line arguments
   */
  public static void main(String[] args) {
    
    if (args.length != 1) {
      System.out.println("Fehler: Kein memFile angegeben.");
      System.out.println("Richtiger Programmaufruf: java programm dateiname");
      System.exit(1);
    }
    
    
    Processor processor;
    byte memBuffer[];
    
    memBuffer = MemReader.readMemFile(args[0]);
    if ( memBuffer.length <= 0) {
      System.out.println("Fehler: Konnte memFile nicht lesen.");
      System.exit(1);
    }
    
//    for(int i=0; i < memBuffer.length; ++i)
//    {
//      System.out.println(i + ":0x" + String.format("%2X", memBuffer[i]).replace(' ', '0') );
//    }
    
    
    try
    {
      processor = new Processor(memBuffer);
      
      System.out.println("\n\nExecute:");
      processor.execute();
    }
    catch(Exception e)
    {
      System.out.println(e);
      System.exit(1);
    }
    

    
    
    
    
    
    
    

    System.exit(0);
  }

  
}
