package raues_u3_a2;

/**
 *
 * @author Steffen
 */
public class MemParser
{

  private enum ParseStates
  {
    ERROR, OPCODE, MIDDLE, DISPLACEMENT
  }

  //private enum Instruction
  //{
  //  ADD, SUB, JNZ, RRMOV, RMMOV, MRMOV, HLT, ERROR
  //}
  public static boolean parse(byte memBuffer[] ,RegisterSet registerSet)
  {
    ParseStates parseState = ParseStates.OPCODE;
    Instruction instruction = new Instruction();
    int currentByte = 0;
    int lastInstruction = 0;
    boolean result = false;

    try
    {
      if (memBuffer == null)
      {
        throw new Exception("Fehler in MemParser.parse(): memBuffer ist null");
      }
      if (memBuffer.length <= 0)
      {
        throw new Exception("Fehler in MemParser.parse(): memBuffer ist leer");
      }
      
      

      while (currentByte < memBuffer.length)
      {
        switch (parseState)
        {
          case OPCODE:
            instruction.clear();
            lastInstruction = currentByte;
            if (instruction.setOpCode(memBuffer[currentByte]))
            {
              if (!instruction.isComplete())
              {
                if ((currentByte + 1) >= memBuffer.length)
                {
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  parseState = ParseStates.MIDDLE;
                }
              }
              else
              {
                printInstruction(instruction, registerSet);
              }
            }
            else
            {
              parseState = ParseStates.ERROR;
            }
            currentByte++;
            break;

          case MIDDLE:
            if (instruction.set2ndByte(memBuffer[currentByte]))
            {
              if (!instruction.isComplete())
              {
                if ((currentByte + 1) >= memBuffer.length)
                {
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  parseState = ParseStates.DISPLACEMENT;
                }
              }
              else
              {
                printInstruction(instruction, registerSet);
                parseState = ParseStates.OPCODE;
              }
            }
            else
            {
              parseState = ParseStates.ERROR;
            }
            currentByte++;
            break;

          case DISPLACEMENT:
            if (instruction.setDisplacement(memBuffer[currentByte]))
            {
              if (instruction.isComplete())
              {
                printInstruction(instruction, registerSet);
                parseState = ParseStates.OPCODE;
              }
              else
              {
                // should not be possible
                throw new Exception("Instruction Parsing error");
              }
            }
            else
            {
              parseState = ParseStates.ERROR;
            }
            currentByte++;
            break;

          case ERROR:
            printError();
            currentByte = lastInstruction + 1;
            parseState = ParseStates.OPCODE;
            break;

          default:
            break;
        }
      }
      
      if(parseState != ParseStates.OPCODE)
      {
        if(parseState != ParseStates.ERROR)
        {
          throw new Exception("Fehler in MemParser.parse(): Parsing-Schleife wurde mit ungueltigem Zustand beendet.");
        }
        printError();
      }
      
      result = true;
    }
    catch (Exception e)
    {
      System.out.println(e);
    }
    
    return result;
  }

  private static void printError()
  {
    System.out.println("Error");
  }

  private static void printInstruction(Instruction inst, RegisterSet registerSet) throws Exception
  {
    String s;
    
    s = inst.getInstructionString(registerSet);

    if (s == null)
    {
      throw new Exception("Fehler");
    }
    else
    {
      System.out.println(s);
    }
  }

}
