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

  public int execute() throws Exception
  {
    int result;
    this.init();

    do
    {
      result = executeStep();
    } while (result > 0);

    return result;
  }

  private int executeStep() throws Exception
  {
    int result;
    Instruction inst;

    inst = parseSingleInstruction();
    if (inst.isComplete())
    {
      dumpMem();

      result = inst.execute(m_IP, m_ZF, m_RegisterSet, m_Memory);

      printInstruction(inst);
      printIP();
      printRegisterSet();
      printZF();
      printMemDump();
    }
    else
    {
      result = -1;
    }

    return result;
  }

  private Instruction parseSingleInstruction()
  {
    int result;
    Instruction inst = new Instruction();

    // check range of ip
    if (m_IP.read() >= m_Memory.length)
    {
      result = -1;
    }
    // more than 3 bytes until mem end (including mem[ip]) => full instruction width
    else if (m_Memory.length - m_IP.read() >= 2)
    {
      result = inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], m_Memory[m_IP.read() + 2], 3);
    }
    // only 2 bytes until mem end (including mem[ip]) => max. instruction width = 2 byte
    else if (m_Memory.length - m_IP.read() >= 1)
    {
      result = inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], (byte) 0, 2);
    }
    // only 1 byte until mem end (including mem[ip]) => max. instruction width = 1 byte
    else
    {
      result = inst.set(m_Memory[m_IP.read()], (byte) 0, (byte) 0, 1);
    }

    // cleanup if something went wrong
    if (result == -1)
    {
      inst.clear();
    }

    return inst;
  }

  private void printInstruction(Instruction inst)
  {
    System.out.println(inst.getInstructionString(m_RegisterSet));
    System.out.println();
  }

  private void printIP()
  {
    System.out.println(m_IP.getName().toUpperCase() + ":  " + String.format("%2x", m_IP.read()));
    System.out.println();
  }

  private void printRegisterSet() throws Exception
  {
    for (int i = 0; i < 2; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        System.out.print(m_RegisterSet.getRegisterName((byte) ((i * 4) + j)).toUpperCase() + ":       " + String.format("%2x", m_RegisterSet.read((byte) ((i * 4) + j))) + " ");
      }
      System.out.print("\n");
    }
  }

  private void printZF()
  {
    System.out.println(m_ZF.getName().toUpperCase() + ":" + String.format("%2x", m_ZF.read()));
    System.out.println();
  }

  private void printMemDump()
  {
    for (int i = 0; i < m_MemoryDump.length; ++i)
    {
      if (i == 0)
      {
        System.out.print("MEM:");
      }
      else if ((i % 16) == 0)
      {
        System.out.print("\n    ");
      }
      else if ((i % 4) == 0)
      {
        System.out.print(' ');
      }
      System.out.print(" " + String.format("%2x", m_MemoryDump[i]).replace(' ', '0'));
    }
    System.out.print("\n\n\n");
  }

  private void init()
  {
    m_IP.write((byte) 0x00);
    m_ZF.write((byte) 0x00);
    m_RegisterSet.init((byte) 0x00);
  }

  private void dumpMem()
  {
    for (int i = 0; i < m_MemoryDump.length; ++i)
    {
      m_MemoryDump[i] = m_Memory[i];
    }
  }

}
