package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class Processor
{

  private RegisterSet m_RegisterSet;

  Processor(byte memBuffer[]) throws Exception
  {
    try
    {
      this.m_RegisterSet = new RegisterSet();

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
