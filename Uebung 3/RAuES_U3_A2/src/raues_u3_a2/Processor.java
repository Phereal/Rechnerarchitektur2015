package raues_u3_a2;

/**
 *
 * @author Steffen
 */
public class Processor
{
  // size of memory
  private static final int K_MEM_SIZE = 80;

  // contains the eight working register (eax, ebx, ...)
  private RegisterSet m_RegisterSet;
  // Instruction pointer, byte is enough, since max. mem. size = 80
  private Register m_IP;
  // no need to use extra boolean register for ZeroFlag, since we make sure only
  // 0 and 1 are written.
  private Register m_ZF;
  // internal memory (length = K_MEM_SIZE)
  private byte m_Memory[];
  // dump of internal memory, used for printing previous memory states
  private byte m_MemoryDump[];

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
      this.m_IP = new Register("ip");
      this.m_ZF = new Register("zf");

      // init of the memory and its dump with 0
      this.m_Memory = new byte[K_MEM_SIZE];
      this.m_MemoryDump = new byte[K_MEM_SIZE];
      for (int i = 0; i < this.m_Memory.length; ++i)
      {
        this.m_Memory[i] = 0;
        this.m_MemoryDump[i] = 0;
      }

      // Parse the memory and print its contents (Aufgabe 1)
      // If the parsing went wrong, we will cancel the creation of Processor.
      if (!MemParser.parse(memBuffer, this.m_RegisterSet))
      {
        throw new Exception("Fehler in Processor.Processor(): MemFile konnte nicht geparsed werden.");
      }

      // Make sure the read mem file fits into the internal memory, otherwise
      // cancel the creation of Processor.
      if (this.m_Memory.length < memBuffer.length)
      {
        throw new Exception("Fehler in Processor.Processor(): MemFile (" + memBuffer.length + ") zu gross fuer internen Speicher (" + this.m_Memory.length + ").");
      }

      // memBuffer ok, copy the passed memBuffer into our internal memory
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

  /**
   * Executes the internal memory until programm end reached or an error occured.
   * @return result of execution.
   * -1 = execution was canceled due to an error.
   * 0 = execution was orderly haltet via hlt instruction.
   * @throws Exception 
   */
  public int execute() throws Exception
  {
    int result;
    
    // init internal registers. mem is not cleared, since it also holds the
    // instructions.
    this.init();

    // execute the instructions until hlt or error
    do
    {
      result = executeStep();
    } while (result > 0); // still instructions left, continue

    // returns only -1 or 0. 1 is not poosible due too the while loop condition.
    return result;
  }

  /**
   * Executes the instruction in the internal memory referenced by m_IP.
   * @return result of execution.
   * -1 = execution was canceled due to an error.
   * 0 = execution was orderly haltet via hlt instruction.
   * 1 = step was successfully executed, no hlt instruction detected.
   * @throws Exception 
   */
  private int executeStep() throws Exception
  {
    int result;
    Instruction inst; // variable to temporarily hold the instruction.

    // get the instruction pointed at by m_IP.
    // Parsed every step and not via an buffered Instruction list since data and
    // instruction use the same memory and could potentially modify its own
    // programcode at runtime.
    inst = parseSingleInstruction();
    // make sure the instruction is valid, otherwise cancel execution,
    if (inst.isComplete())
    {
      // save memory to print the memory content before the execution of the
      // instruction.
      dumpMem();

      // execute the instruction.
      result = inst.execute(m_IP, m_ZF, m_RegisterSet, m_Memory);

      // print execution step as stated in Aufgabe 2.
      printInstruction(inst);
      printIP();
      printRegisterSet();
      printZF();
      printMemDump(); // previous memory state
    }
    else
    {
      // invalid instruction
      result = -1;
    }

    return result;
  }

  /**
   * Reads the memory pointed at by m_IP and tries to parse the instruction.
   * @return parsed instruction (empty instruction if parsing failed).
   */
  private Instruction parseSingleInstruction()
  {
    int result;
    Instruction inst = new Instruction();
    
    // Contrary to Aufgabe 1 MemParser.parse() we use the newly implemented
    // method inst.set(), which expects up to all three bytes of an instruction.
    // Thus we dont have to copy and edit the state machine of MemParser.parse()
    // and reduce the necessary code in Processor to execute the memory.

    // make sure m_IP doesnt point outside of our memory range
    if (m_IP.read() >= m_Memory.length)
    {
      result = -1;
    }
    // check how many bytes are left from IP to the end of the internal memory,
    // to make sure we arent getting an index out of bound exception while
    // calling inst.set() with something like m_Memory[81].
    // More than 3 bytes until mem end (including mem[ip]) => full instruction 
    // width of 3 bytes.
    else if (m_Memory.length - m_IP.read() >= 2)
    {
      result = inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], m_Memory[m_IP.read() + 2], 3);
    }
    // only 2 bytes until mem end (including mem[ip]) => max. instruction
    // width = 2 byte.
    else if (m_Memory.length - m_IP.read() >= 1)
    {
      result = inst.set(m_Memory[m_IP.read()], m_Memory[m_IP.read() + 1], (byte) 0, 2);
    }
    // only 1 byte until mem end (including mem[ip]) => max. instruction
    // width = 1 byte.
    else
    {
      result = inst.set(m_Memory[m_IP.read()], (byte) 0, (byte) 0, 1);
    }

    // cleanup the returned instruction if something went wrong.
    if (result != 1)
    {
      inst.clear();
    }

    return inst;
  }

  /**
   * Prints the instruction in the format stated in Aufgabe 2.
   * @param inst 
   */
  private void printInstruction(Instruction inst)
  {
    System.out.println(inst.getInstructionString(m_RegisterSet));
    System.out.println();
  }


  /**
   * Prints the instruction pointer in the format stated in Aufgabe 2.
   */
  private void printIP()
  {
    System.out.println(m_IP.getName().toUpperCase() + ":  " + String.format("%2x", m_IP.read()));
    System.out.println();
  }

  /**
   * Prints the internal registers in the format stated in Aufgabe 2.
   */
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


  /**
   * Prints the ZeroFlag in the format stated in Aufgabe 2.
   */
  private void printZF()
  {
    System.out.println(m_ZF.getName().toUpperCase() + ":" + String.format("%2x", m_ZF.read()));
    System.out.println();
  }

  /**
   * Prints the dumped memory in the format stated in Aufgabe 2.
   */
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

  /**
   * Initialises the internal registers with 0.
   */
  private void init()
  {
    m_IP.write((byte) 0x00);
    m_ZF.write((byte) 0x00);
    m_RegisterSet.init((byte) 0x00);
  }

  /**
   * Dumps the content of m_Memory in m_MemoryDump.
   */
  private void dumpMem()
  {
    for (int i = 0; i < m_MemoryDump.length; ++i)
    {
      m_MemoryDump[i] = m_Memory[i];
    }
  }

}
