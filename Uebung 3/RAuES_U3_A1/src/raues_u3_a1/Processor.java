package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class Processor
{
  // this class only contains the registers necessary for Aufgabe 1. Internal
  // memory or ZF, ..., or functions to execute are not yet implemented.

  // contains the eight working register (eax, ebx, ...)
  private RegisterSet m_RegisterSet;
  
  /**
   * Constructor.
   * @param memBuffer byte array containing the initial memory of Processor
   * (max. length = 80).
   * @throws Exception when memBuffer couldnt be parsed.
   */
  Processor(byte memBuffer[]) throws Exception
  {
    try
    {
      // init registers
      this.m_RegisterSet = new RegisterSet();
      
      // Parse the memory and print its contents (Aufgabe 1)
      // If the parsing went wrong, we will cancel the creation of Processor.
      if (!MemParser.parse(memBuffer, this.m_RegisterSet))
      {
        throw new Exception("Fehler in Processor.Processor(): MemFile konnte nicht geparsed werden.");
      }
    }
    catch (Exception e)
    {
      throw e;
    }
  }

}
