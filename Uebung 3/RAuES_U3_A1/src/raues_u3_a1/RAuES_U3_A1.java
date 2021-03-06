package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class RAuES_U3_A1
{

  /**
   * @param args [0] path to the bin mem file
   */
  public static void main(String[] args)
  {
    Processor processor;
    byte memBuffer[];
    
    // check whether a mem file and no other parameters where set, otherwise
    // the programm will exit with an error message.
    if ((args == null) || (args.length != 1))
    {
      System.out.println("Fehler: Kein memFile angegeben.");
      System.out.println("Richtiger Programmaufruf: java programm dateiname");
      System.exit(1);
    }

    // read the mem file and buffer it in an byte array
    memBuffer = MemReader.readMemFile(args[0]);
    // check whether a mem file was successfully read and isnt empty, otherwise
    // the programm will exit with an error message.
    if ((memBuffer == null) || (memBuffer.length <= 0))
    {
      System.out.println("Fehler: Konnte memFile nicht lesen.");
      System.exit(1);
    }
    
    // mem file is read, try to parse it.
    try
    {
      // Instanciate Processor to parse the mem buffer
      System.out.println("==================================================");
      System.out.println(" Lese Speicherdatei ein:");
      System.out.println("==================================================");
      processor = new Processor(memBuffer);
      System.out.println("==================================================");
    }
    catch (Exception e)
    {
      System.out.println(e);
      System.exit(1);
    }

    // successful program end
    System.exit(0);
  }
}
