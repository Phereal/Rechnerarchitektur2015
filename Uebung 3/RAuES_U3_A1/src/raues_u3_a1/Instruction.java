package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class Instruction
{
  /**
   * Contains the possible instructions of our Y86 implementation.
   */
  public enum InstructionSet
  {
    ERROR, ADD, SUB, JNZ, RRMOV, RMMOV, MRMOV, HLT
  }


  // opcodes of the available instructions.
  // RRmov and RMmov have the same opcode and are determined with the 2nd byte.
  private static final byte K_OP_CODE_ADD = (byte) 0x01;
  private static final byte K_OP_CODE_SUB = (byte) 0x29;
  private static final byte K_OP_CODE_JNZ = (byte) 0x75;
  private static final byte K_OP_CODE_RRM_RMM = (byte) 0x89;
  private static final byte K_OP_CODE_MRM = (byte) 0x8B;
  private static final byte K_OP_CODE_HLT = (byte) 0xF4;

  // these members hold the 1st, 2nd and 3rd bytes of the instruction. Bytes
  // that arent used shall be 0.
  private byte m_OpCode = 0;
  private byte m_2ndByte = 0;
  private byte m_Displacement = 0;

  // true if the corresponding byte was successfully set (parsed).
  private boolean m_OpCodeOk = false;
  private boolean m_2ndByteOk = false;
  private boolean m_DisplacementOk = false;

  // used to hold the information contained in the 2nd byte.
  // for RMmov m_RD will hold edi.
  // for MRmov m_RS will hold esi.
  // m_Dist is used to make the following code easier to understand, it holds
  // the same value as 2nd Byte for jnz.
  private byte m_RS = 0;
  private byte m_RD = 0;
  private byte m_Dist = 0;

  // used to identify the instruction and is set when all the necessary bytes
  // are set. Incomplete or invalid instruction shall be ERROR.
  private InstructionSet m_Instruction = InstructionSet.ERROR;
  
  /**
   * Returns the instruction string formatted as stated in Aufgabe 1 for prints.
   * @param registerSet Register set of the Y86 (used to get the register name).
   * @return formatted instruction string. Null if instruction is invalid.
   */
  public String getInstructionString(RegisterSet registerSet)
  {
    String result;

    // create the instruction string corresponding to the instruction.
    // Invalid instructions (ERROR) return a null reference.
    // Default shouldnt be possible and returns a null reference.
    switch (this.m_Instruction)
    {
      case ADD:
        result = registerSet.getRegisterName(this.m_RD);
        result += " := ";
        result += registerSet.getRegisterName(this.m_RD);
        result += " + ";
        result += registerSet.getRegisterName(this.m_RS);
        break;

      case SUB:
        result = registerSet.getRegisterName(this.m_RD);
        result += " := ";
        result += registerSet.getRegisterName(this.m_RD);
        result += " - ";
        result += registerSet.getRegisterName(this.m_RS);
        break;

      case JNZ:
        result = "if !ZF then IP := IP + ";
        result += this.m_Dist;
        break;

      case RRMOV:
        result = registerSet.getRegisterName(this.m_RD);
        result += " := ";
        result += registerSet.getRegisterName(this.m_RS);
        break;

      case RMMOV:
        result = "MEM[";
        result += registerSet.getRegisterName(this.m_RD);
        result += " + ";
        result += this.m_Displacement;
        result += "] := ";
        result += registerSet.getRegisterName(this.m_RS);
        break;

      case MRMOV:
        result = registerSet.getRegisterName(this.m_RD);
        result += " := MEM[";
        result += registerSet.getRegisterName(this.m_RS);
        result += " + ";
        result += this.m_Displacement;
        result += "]";
        break;

      case HLT:
        result = "Prozessor anhalten";
        break;

      case ERROR:
      default:
        result = null;
        break;
    }

    return result;
  }
  
  /**
   * Sets opcode of the instruction.
   * @param opCode opcode
   * @return returns true if opcode was set succesfully.
   */
  public boolean setOpCode(byte opCode)
  {
    boolean result = false;

    if (opCodeValid(opCode) == true)
    {
      // opcode is valid, clear previous content of this instruction (other data
      // is invalid since one part of the instruction changed).
      this.clear();
      this.m_OpCode = opCode;
      this.m_OpCodeOk = true;
      // set m_Instruction if it was a 1 byte instruction (hlt).
      if (this.isComplete())
      {
        this.m_Instruction = InstructionSet.HLT;
      }
      result = true;
    }

    return result;
  }

  /**
   * Sets second byte of the instruction.
   * @param middle second byte
   * @return returns true if 2nd byte was set succesfully.
   */
  public boolean set2ndByte(byte middle)
  {
    boolean result = false;
    // variable to temporarily save the instructionEnum, m_instruction is set
    // at the end of the method if previous parsing was succesfully.
    InstructionSet instruction = this.m_Instruction;

    // only continue if the opcode was set. no changes to this object if not.
    if (this.m_OpCodeOk)
    {
      // check what opcode is set an parse the 2nd byte correspondig to it.
      // the 1 byte instruction hlt will be ignored and returnes false, same for
      // default.
      switch (this.m_OpCode)
      {
        case K_OP_CODE_ADD:
          // check if the two msb of the 2nd byte are 11 binary.
          if ((middle & 0xC0) == 0xC0)
          {
            // set instructionEnum for add instruction.
            instruction = InstructionSet.ADD;
            // save source and destination register for later use. clear dist.
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_SUB:
          // check if the two msb of the 2nd byte are 11 binary.
          if ((middle & 0xC0) == 0xC0)
          {
            // set instructionEnum for sub instruction.
            instruction = InstructionSet.SUB;
            // save source and destination register for later use. clear dist.
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_JNZ:
          // set instructionEnum for jnz instruction.
          instruction = InstructionSet.JNZ;
          // clear source and destination register. save dist for later use.
          this.m_RD = 0;
          this.m_RS = 0;
          // dist isnt checked here, Instruction dont know the max size of the
          // memory or even the content of the instruction pointer.
          this.m_Dist = middle;

          result = true;
          break;

        case K_OP_CODE_RRM_RMM:
          // check if the two msb of the 2nd byte are 11 binary (=> RRmov).
          if ((middle & 0xC0) == 0xC0) // RRMOV
          {
            // set instructionEnum for RRmov instruction.
            instruction = InstructionSet.RRMOV;
            // save source and destination register for later use. clear dist.
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          // otherwise check if the two msb of the 2nd byte are 01 binary and
          // the three lsb are 110 binary (=> RMmov).
          else if (((middle & 0xC0) == 0x40) && ((middle & 0x07) == 0x06)) // RMMOV
          {
            // set instructionEnum for RMmov instruction.
            instruction = InstructionSet.RMMOV;
            // save source and destination register for later use. clear dist.
            // for RMmov m_RD will hold edi.
            this.m_RD = (byte) (7); // edi
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_MRM:
          // check if the two msb of the 2nd byte are 01 binary and
          // the three lsb are 110 binary (=> possible[if 3 bytes] MRmov).
          if (((middle & 0xC0) == 0x40) && ((middle & 0x07) == 0x06)) // MRMOV
          {
            // set instructionEnum for MRmov instruction.
            instruction = InstructionSet.MRMOV;
            // save source and destination register for later use. clear dist.
            // for MRmov m_RS will hold esi.
            this.m_RD = (byte) ((middle >> 3) & 0x07);
            this.m_RS = (byte) (6); // esi
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_HLT:
          // return default false since hlt only has 1 byte
        default:
          break;
      }
    }

    // save 2nd byte and instructionEnum and clear displacement if instruction
    // was successfully parsed. 
    if (result)
    {
      this.m_Instruction = instruction;
      this.m_2ndByte = middle;
      this.m_2ndByteOk = true;
      this.m_Displacement = 0;
      this.m_DisplacementOk = false;
    }

    return result;
  }

  /**
   * Sets displacement (last byte) of the instruction.
   * @param displacement displacement
   * @return returns true if 3rd byte was set succesfully.
   */
  public boolean setDisplacement(byte displacement)
  {
    boolean result = false;

    // only set the displacement if the previous byte where set and
    // the instruction isnt complete yet.
    if (this.m_OpCodeOk && this.m_2ndByteOk)
    {
      if (!this.isComplete())
      {
        // no need check displacement itself, since all values are allowed.
        this.m_Displacement = displacement;
        this.m_DisplacementOk = true;
        result = true;
      }
    }

    return result;
  }
  
  /**
   * Clears all members of Instruction.
   */
  public void clear()
  {
    this.m_OpCode = 0;
    this.m_2ndByte = 0;
    this.m_Displacement = 0;

    this.m_OpCodeOk = false;
    this.m_2ndByteOk = false;
    this.m_DisplacementOk = false;

    this.m_RS = 0;
    this.m_RD = 0;
    this.m_Dist = 0;

    m_Instruction = InstructionSet.ERROR;
  }

  /**
   * Checks if the passed opcode is valid.
   * opCode is passed as parameter to allow checking op the opcode without
   * the need to change m_Opcode. And since no members are accessed, the funtion
   * was done static.
   * @param opCode opcode to be checked.
   * @return returns true if the opcode is valid.
   */
  public static boolean opCodeValid(byte opCode)
  {
    boolean valid;

    // check if the passed opcode is known to us.
    switch (opCode)
    {
      case K_OP_CODE_ADD:
      case K_OP_CODE_SUB:
      case K_OP_CODE_JNZ:
      case K_OP_CODE_RRM_RMM:
      case K_OP_CODE_MRM:
      case K_OP_CODE_HLT:
        valid = true;
        break;

      default:
        valid = false;
        break;
    }

    return valid;
  }

  /**
   * Checks whether the instruction is complete and valid.
   * @return returns true if the instruction is complete.
   */
  public boolean isComplete()
  {
    boolean result = false;

    // checks whether all bytes that are needed by a specific opcode are set.
    // return false if opcode isnt set
    if (this.m_OpCodeOk)
    {
      switch (this.m_OpCode)
      {
        // check the 2nd byte for 2 byte instructions.
        // no need to check whether the 2 msb are right, since it was already
        // checked during the setting of the 2nd byte.
        case K_OP_CODE_ADD:
        case K_OP_CODE_SUB:
        case K_OP_CODE_JNZ:
          if (m_2ndByteOk)
          {
            result = true;
          }
          break;

        // check the 2nd byte and the (third) byte for RRmov and RMmov.
        // no need to check whether the 2 msb and the 3 lsb are right, since
        // it was already checked during the setting of the 2nd byte.
        case K_OP_CODE_RRM_RMM:
          if (this.m_2ndByteOk)
          {
            if ((this.m_2ndByte & 0xC0) == 0xC0)
            {
              result = true;
            }
            else if ((this.m_2ndByte & 0xC0) == 0x40)
            {
              if (this.m_DisplacementOk)
              {
                result = true;
              }
            }
          }
          break;

        // check the 2nd byte and the third byte for MRmov.
        // no need to check whether the 2 msb and the 3 lsb of the 2nd byte are
        // right, since it was already checked during the setting of the 2nd
        // byte.
        case K_OP_CODE_MRM:
          if (this.m_2ndByteOk && this.m_DisplacementOk)
          {
            result = true;
          }
          break;

        // no need to check 2nd or 3rd byte for an 1 byte instruction.
        case K_OP_CODE_HLT:
          result = true;
          break;

        default:
          break;
      }
    }
    return result;
  }

}
