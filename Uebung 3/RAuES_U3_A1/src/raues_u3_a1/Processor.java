package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class Processor
{

  private static final int K_MEM_SIZE = 80;

  private RegisterSet m_RegisterSet;
  private Register m_IP;
  private Register m_ZF;
  private byte m_Memory[];
  private byte m_MemoryDump[];

  Processor(byte memBuffer[]) throws Exception
  {
    try
    {
      this.m_RegisterSet = new RegisterSet();
      this.m_IP = new Register("ip");
      this.m_ZF = new Register("zf");

      this.m_Memory = new byte[K_MEM_SIZE];
      this.m_MemoryDump = new byte[K_MEM_SIZE];
      for (int i = 0; i < this.m_Memory.length; ++i)
      {
        this.m_Memory[i] = 0;
        this.m_MemoryDump[i] = 0;
      }

      if (!MemParser.parse(memBuffer, this.m_RegisterSet))
      {
        throw new Exception("Fehler in Processor.Processor(): MemFile konnte nicht geparsed werden.");
      }

      if (this.m_Memory.length < memBuffer.length)
      {
        throw new Exception("Fehler in Processor.Processor(): MemFile (" + memBuffer.length + ") zu gross fuer internen Speicher (" + this.m_Memory.length + ").");
      }

      for (int i = 0; i < memBuffer.length; ++i)
      {
        this.m_Memory[i] = memBuffer[i];
      }
    }
    catch (Exception e)
    {
      throw e;
    }
  }

}
