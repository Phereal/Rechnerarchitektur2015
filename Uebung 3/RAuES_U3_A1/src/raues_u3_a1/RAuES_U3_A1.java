package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class RAuES_U3_A1
{

  /**
   * @param args the command line arguments
   */
  public static void main(String[] args)
  {
    Processor processor;
    byte memBuffer[];

    if ((args == null) || (args.length != 1))
    {
      System.out.println("Fehler: Kein memFile angegeben.");
      System.out.println("Richtiger Programmaufruf: java programm dateiname");
      System.exit(1);
    }

    memBuffer = MemReader.readMemFile(args[0]);
    if ((memBuffer == null) || (memBuffer.length <= 0))
    {
      System.out.println("Fehler: Konnte memFile nicht lesen.");
      System.exit(1);
    }
    try
    {
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

    System.exit(0);
  }
}
