package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class MemParser
{

  private static final boolean K_DEBUG = false;

  private enum ParseStates
  {
    ERROR, OPCODE, MIDDLE, DISPLACEMENT
  }

  public static boolean parse(byte memBuffer[], RegisterSet registerSet)
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
                  currentByte--; // make sure we dont leave the loop yet
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  parseState = ParseStates.MIDDLE;
                }
              }
              else
              {
                printDebug(lastInstruction);
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
                  currentByte--; // make sure we dont leave the loop yet
                  parseState = ParseStates.ERROR;
                }
                else
                {
                  parseState = ParseStates.DISPLACEMENT;
                }
              }
              else
              {
                printDebug(lastInstruction);
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
                printDebug(lastInstruction);
                printInstruction(instruction, registerSet);
                parseState = ParseStates.OPCODE;
              }
              else
              {
                // should not be possible
                throw new Exception("Fehler in MemParser.parse(): Instruction Parsing error");
              }
            }
            else
            {
              parseState = ParseStates.ERROR;
            }
            currentByte++;
            break;

          case ERROR:
            printDebug(lastInstruction);
            printError();
            currentByte = lastInstruction + 1;
            parseState = ParseStates.OPCODE;
            break;

          default:
            break;
        }
      }

      if (parseState != ParseStates.OPCODE)
      {
        if (parseState != ParseStates.ERROR)
        {
          throw new Exception("Fehler in MemParser.parse(): Parsing-Schleife wurde mit ungueltigem Zustand beendet.");
        }
        printDebug(lastInstruction);
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

  private static void printDebug(int line)
  {
    if (K_DEBUG)
    {
      System.out.print(String.format("%2d:", line).replace(' ', '0') + " ");
    }
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
      throw new Exception("Fehler in MemParser.printInstruction(): Ungueltiger Instruction String.");
    }
    else
    {
      System.out.println(s);
    }
  }

}
