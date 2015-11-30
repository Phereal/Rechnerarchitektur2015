package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class MemParser
{
  // internal switch to add extra prints used for debugging (printDebug())
  private static final boolean K_DEBUG = false;

  // Possible states during instruction parsing. used in parse()
  private enum ParseStates
  {
    ERROR, // state used when an instruction was invalid 
    OPCODE, // state used to process the first byte of an instruction
    MIDDLE, // state used to process the second byte of an instruction
    DISPLACEMENT // state used to process the last(third) byte of an instruction
  }

  /**
   * Reads the memory passed via memBuffer and prints the instructions.
   * The prints are formated as stated in Aufgabe 1.
   * @param memBuffer memory to be parsed (caller has to make sure it doesnt
   * exceeds the wanted size of 80 bytes).
   * @param registerSet contains the registers of the processor, used to print
   * the register names.
   * @return returns true if no error occured during the parsing, otherwise false.
   */
  public static boolean parse(byte memBuffer[], RegisterSet registerSet)
  {
    // holds the current state. start with the first byte of an instruction.
    ParseStates parseState = ParseStates.OPCODE;
    // variable to hold the instruction that is currently being parsed.
    Instruction instruction = new Instruction();
    // used address the currently parsed byte of the memory
    int currentByte = 0;
    // used to save where the last beginning of an instruction was found (state 
    // = OPCODE). If an error occured during the 2nd or 3rd byte we continue
    // parsing after the saved address.
    int lastInstruction = 0;
    // returned result of this method. stays false if an error ocurres.
    boolean result = false;

    try
    {
      // make sure the passed memory is valid and not empty
      if (memBuffer == null)
      {
        throw new Exception("Fehler in MemParser.parse(): memBuffer ist null");
      }
      if (memBuffer.length <= 0)
      {
        throw new Exception("Fehler in MemParser.parse(): memBuffer ist leer");
      }

      // check every byte of the memory
      while (currentByte < memBuffer.length)
      {
        switch (parseState)
        {
          // first byte of a possible instruction
          case OPCODE:
            instruction.clear(); // clean up the previously changed instruction.
            lastInstruction = currentByte; // save address for error handling.
            // try to parse the opcode.
            if (instruction.setOpCode(memBuffer[currentByte]))
            {
              // opcode is valid, check whether the instruction is already
              // complete (1 byte instructions like hlt).
              if (!instruction.isComplete())
              {
                // instruction incomplete, check if we already exceed the memory
                // range.
                if ((currentByte + 1) >= memBuffer.length)
                {
                  // Expecting more bytes, but end of memory reached => error.
                  // make sure we dont leave the loop yet (for error handling).
                  currentByte--;
                  // next state: error handling.
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  // Expecting more bytes, proceed with the 2nd byte.
                  parseState = ParseStates.MIDDLE;
                }
              }
              else
              {
                // instruction complete, print the instruction.
                printDebug(lastInstruction);
                printInstruction(instruction, registerSet);
                // next state stays OPCODE.
              }
            }
            else
            {
              // invalid opcode => error.
              // next state: error handling.
              parseState = ParseStates.ERROR;
            }
            // next byte!
            currentByte++;
            break;

          // second byte of a possible instruction
          case MIDDLE:
            // try to parse the second byte.
            if (instruction.set2ndByte(memBuffer[currentByte]))
            {
              // 2nd byte is valid, check whether the instruction is already
              // complete (2 byte instructions like add, sub, ...).
              if (!instruction.isComplete())
              {
                // instruction incomplete, check if we already exceed the memory
                // range.
                if ((currentByte + 1) >= memBuffer.length)
                {
                  // Expecting more bytes, but end of memory reached => error.
                  // make sure we dont leave the loop yet (for error handling).
                  currentByte--;
                  // next state: error handling.
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  // Expecting more bytes, proceed with the 3rd byte.
                  parseState = ParseStates.DISPLACEMENT;
                }
              }
              else
              {
                // instruction complete, print the instruction.
                printDebug(lastInstruction);
                printInstruction(instruction, registerSet);
                // next instruction => state OPCODE.
                parseState = ParseStates.OPCODE;
              }
            }
            else
            {
              // invalid opcode => error.
              // next state: error handling.
              parseState = ParseStates.ERROR;
            }
            // next byte!
            currentByte++;
            break;

          // third byte of a possible instruction
          case DISPLACEMENT:
            // try to parse the third byte.
            if (instruction.setDisplacement(memBuffer[currentByte]))
            {
              // 3rd byte is valid, check whether the instruction is complete
              // (3 byte instructions like RMmov, ...).
              if (instruction.isComplete())
              {
                // instruction complete, print the instruction.
                printDebug(lastInstruction);
                printInstruction(instruction, registerSet);
                // next instruction => state OPCODE.
                parseState = ParseStates.OPCODE;
              }
              else
              {
                // instruction has more than 3 bytes => should not be possible
                // error in program code.
                throw new Exception("Fehler in MemParser.parse(): Instruction Parsing error");
              }
            }
            else
            {
              // invalid opcode => error.
              // next state: error handling.
              parseState = ParseStates.ERROR;
            }
            // next byte!
            currentByte++;
            break;

          // a byte wasnt a valid part of an instruction, error handling
          case ERROR:
            // print error message
            printDebug(lastInstruction);
            printError();
            // continue after the beginning of the last instruction (important
            // when an error occured at the 2nd or 3rd byte of an instruction.)
            currentByte = lastInstruction + 1;
            // next instruction. (inst is cleared in state OPCODE)
            parseState = ParseStates.OPCODE;
            break;

          // impossible to reach since all states of ParseStates are cased.
          default:
            // do nothing
            break;
        }
      }

      // make sure that there are no unfinished bytes due to memory range.
      if (parseState != ParseStates.OPCODE)
      {
        if (parseState != ParseStates.ERROR)
        {
          throw new Exception("Fehler in MemParser.parse(): Parsing-Schleife wurde mit ungueltigem Zustand beendet.");
        }
        printDebug(lastInstruction);
        printError();
      }

      // no exception => all bytes could be parsed (error byte counts as ok, 
      // since they may be data and the program may never run into them when
      // using jumps).
      result = true;
    }
    catch (Exception e)
    {
      System.out.println(e);
    }

    return result;
  }

  /**
   * Prints the passed line if K_DEBUG is true. 
   * @param line memory address to be printed.
   */
  private static void printDebug(int line)
  {
    if (K_DEBUG)
    {
      System.out.print(String.format("%2d:", line).replace(' ', '0') + " ");
    }
  }

  /**
   * Prints the error message for an invalid byte.
   */
  private static void printError()
  {
    System.out.println("Error");
  }
  
  /**
   * Prints the instruction formatted as stated in Aufgabe 1.
   * @param inst instruction to print.
   * @param registerSet holds the Y86 registers and there names (used for their
   * names).
   * @throws Exception 
   */
  private static void printInstruction(Instruction inst, RegisterSet registerSet) throws Exception
  {
    String s;

    // delegate the string building to inst.
    s = inst.getInstructionString(registerSet);

    // check if a valid string was returned.
    if (s == null)
    {
      throw new Exception("Fehler in MemParser.printInstruction(): Ungueltiger Instruction String.");
    }
    else
    {
      // print the instruction.
      System.out.println(s);
    }
  }

}
