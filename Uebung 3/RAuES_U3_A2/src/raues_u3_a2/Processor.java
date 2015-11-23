package raues_u3_a2;

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

  Processor(byte memBuffer[]) throws Exception
  {
    try
    {
      this.m_RegisterSet = new RegisterSet();
      this.m_IP = new Register("PC");
      this.m_ZF = new Register("ZF");

      this.m_Memory = new byte[K_MEM_SIZE];
      for (int i = 0; i < this.m_Memory.length; ++i)
      {
        this.m_Memory[i] = 0;
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

  public int execute()
  {
    int result = 0;
    Instruction inst = new Instruction();

    this.init();

    inst.clear();
    if (m_IP.read() > m_Memory.length)
    {
      result = -1;
    }
    else if (m_Memory.length - m_IP.read() >= 2)
    {
      inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], m_Memory[m_IP.read() + 2], 3);
    }
    else if (m_Memory.length - m_IP.read() >= 1)
    {
      inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], (byte) 0, 2);
    }
    else
    {
      inst.set(m_Memory[m_IP.read()], (byte) 0, (byte) 0, 1);
    }

    return result;
  }

  private void init()
  {
    m_IP.write((byte) 0x00);
    m_ZF.write((byte) 0x00);
    m_RegisterSet.init((byte) 0x00);
  }

}
