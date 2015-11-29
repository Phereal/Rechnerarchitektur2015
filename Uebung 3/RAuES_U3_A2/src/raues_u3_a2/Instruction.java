package raues_u3_a2;

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
   * Returns the instruction enum.
   * @return m_Instruction.
   */
  public InstructionSet getInstruction()
  {
    return m_Instruction;
  }

  /**
   * Returns the instruction string formatted as stated in Aufgabe 2 for prints.
   * @param registerSet Register set of the Y86 (used to get the register name).
   * @return formatted instruction string. Null if instruction is invalid.
   */
  public String getInstructionString(RegisterSet registerSet)
  {
    String result;

    // create the instruction string corresponding to the instruction.
    // Invalid instructions (ERROR) return a null reference.
    // Default shouldnt be possible and returns a null reference.
    switch (m_Instruction)
    {
      case ADD:
        result = "add ";
        result += registerSet.getRegisterName(m_RD);
        result += ", ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case SUB:
        result = "sub ";
        result += registerSet.getRegisterName(m_RD);
        result += ", ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case JNZ:
        result = "jnz [IP+0x";
        result += String.format("%2x", m_Dist).replace(' ', '0');
        result += "]";
        break;

      case RRMOV:
        result = "RRmov ";
        result += registerSet.getRegisterName(m_RD);
        result += ", ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case RMMOV:
        result = "RMmov ";
        result += "[EDI+0x";
        result += String.format("%2x", m_Displacement).replace(' ', '0');
        result += "], ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case MRMOV:
        result = "MRmov ";
        result += registerSet.getRegisterName(m_RD);
        result += ", [ESI+0x";
        result += String.format("%2x", m_Displacement).replace(' ', '0');
        result += "]";
        break;

      case HLT:
        result = "hlt";
        break;

      case ERROR:
      default:
        result = null;
        break;
    }

    return result;
  }

  /**
   * Returns the instruction string formatted as stated in Aufgabe 1 for prints.
   * @param registerSet Register set of the Y86 (used to get the register name).
   * @param useHex changes the displacement to hex.
   * @return formatted instruction string. Null if instruction is invalid.
   */
  public String getInstructionString(RegisterSet registerSet, boolean useHex)
  {
    String result;

    // create the instruction string corresponding to the instruction.
    // Invalid instructions (ERROR) return a null reference.
    // Default shouldnt be possible and returns a null reference.
    switch (m_Instruction)
    {
      case ADD:
        result = registerSet.getRegisterName(m_RD);
        result += " := ";
        result += registerSet.getRegisterName(m_RD);
        result += " + ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case SUB:
        result = registerSet.getRegisterName(m_RD);
        result += " := ";
        result += registerSet.getRegisterName(m_RD);
        result += " - ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case JNZ:
        result = "if !ZF then IP := IP + ";
        if (useHex)
        {
          result += String.format("%2x", m_Dist);
        }
        else
        {
          result += m_Dist;
        }
        break;

      case RRMOV:
        result = registerSet.getRegisterName(m_RD);
        result += " := ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case RMMOV:
        result = "MEM[";
        result += registerSet.getRegisterName(m_RD);
        result += " + ";
        if (useHex)
        {
          result += String.format("%2x", m_Displacement);
        }
        else
        {
          result += m_Displacement;
        }
        result += "] := ";
        result += registerSet.getRegisterName(m_RS);
        break;

      case MRMOV:
        result = registerSet.getRegisterName(m_RD);
        result += " := MEM[";
        result += registerSet.getRegisterName(m_RS);
        result += " + ";
        if (useHex)
        {
          result += String.format("%2x", m_Displacement);
        }
        else
        {
          result += m_Displacement;
        }
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
   * Sets all bytes of an instruction. Added for Aufgabe 2.
   * @param opCode 1th byte of the instruction containing the opcode.
   * @param middle 2nd byte of the instruction for 2 or 3 byte instructions.
   * @param displacement 3rd byte of the instruction containing the displacement
   * for 3 byte instructions.
   * @param argCnt number ob bytes that have to be checked. Used when less than
   * 3 bytes are available due to memory range.
   * @return result of parsing.
   * -1 = passed bytes dont form a valid instruction.
   * 1 = instruction was parsed successfully.
   */
  public int set(byte opCode, byte middle, byte displacement, int argCnt)
  {
    int result = 0;

    // check if the argCnt is in range (1 to 3 byte instructions)
    if ((argCnt <= 0) || (argCnt > 3))
    {
      result = -1;
    }

    // if no error occured previously, try to set the opcode.
    // return -1 if it fails.
    if ((result == 0) && (!this.setOpCode(opCode)))
    {
      result = -1;
    }

    // if no error occured previously, check if instruction is complete.
    // return 1 if instruction is complete.
    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }

    
    // if no error occured previously and instruction is incomplete, check if
    // a 2nd byte was passed from the caller.
    // return 1 if no 2nd byte is available.
    if ((result == 0) && (argCnt == 1))
    {
      result = -1;
    }
    
    // if no error occured previously and instruction is incomplete and 2nd byte
    // is available, try to set the 2nd byte.
    // return -1 if it fails.
    if ((result == 0) && (!this.set2ndByte(middle)))
    {
      result = -1;
    }

    // if no error occured previously, check if instruction is complete.
    // return 1 if instruction is complete.
    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }

    
    // if no error occured previously and instruction is incomplete, check if
    // a 3rd byte was passed from the caller.
    // return 1 if no 3rd byte is available.
    if ((result == 0) && (argCnt == 2))
    {
      result = -1;
    }
    
    // if no error occured previously and instruction is incomplete and 3rd byte
    // is available, try to set the 3rd byte.
    // return -1 if it fails.
    if ((result == 0) && (!this.setDisplacement(displacement)))
    {
      result = -1;
    }

    // if no error occured previously, check if instruction is complete.
    // return 1 if instruction is complete.
    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }
    
    // return -1 if instruction isnt complete after 3 bytes.
    if (result == 0)
    {
      result = -1;
    }

    // cleanup if an error occured
    if (result == -1)
    {
      this.clear();
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

    // check if the opcode is valid.
    if (opCodeValid(opCode) == true)
    {
      // opcode is valid, clear previous content of this instruction (other data
      // is invalid since one part of the instruction changed).
      this.clear();
      this.m_OpCode = opCode; // save opcode
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
    
    this.m_Instruction = InstructionSet.ERROR;
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
   * Returns the estimeated instruction size.
   * @return instruction size.
   * returns -1 if the instruction is invalid.
   * returns 0 if the instruction isnt complete yet and the instruction size.
   * cant be determined with only the opcode(RRmov, RMmov with the same opcode).
   * returns 1-3 (instruction size in byte) otherwise.
   */
  public int getInstructionSize()
  {
    int result = -1;

    if (m_OpCodeOk)
    {
      switch (m_OpCode)
      {
        case K_OP_CODE_ADD:
        case K_OP_CODE_SUB:
        case K_OP_CODE_JNZ:
          result = 2;
          break;

        case K_OP_CODE_RRM_RMM:
          if (m_2ndByteOk)
          {
            if ((m_2ndByte & 0x80) == 0x80)
            {
              result = 2;
            }
            else
            {
              result = 3;
            }
          }
          else
          {
            // cant be sure if 2 or 3 bytes, but opcode valid => return 0.
            result = 0;
          }
          break;

        case K_OP_CODE_MRM:
          result = 3;
          break;

        case K_OP_CODE_HLT:
          result = 1;
          break;

        default:
          break;
      }
    }

    return result;
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

  /**
   * Executes the instruction if its complete and valid.
   * @param ip instruction pointer of the processor (inout).
   * @param zf zeroflag of the processor (inout).
   * @param regs working registers of the processors (inout).
   * @param mem memory of the processors (inout).
   * @return result of execution.
   * -1 = execution was canceled due to an error.
   * 0 = execution was orderly haltet via hlt instruction.
   * 1 = step was successfully executed, no hlt instruction detected.
   * @throws Exception 
   */
  public int execute(Register ip, Register zf, RegisterSet regs, byte[] mem) throws Exception
  {
    int result = -1;
    // variable to temporarily hold the relative address for memory access.
    int tmpEa;
    // variable to save the instruction pointer, so that the same address will
    // be returned if an error occured.
    int tmpIp = ip.read();
    // variable to temporarily hold the instruction pointer calculated bye jnz.
    int tmpIpJnz;

    // only execute if the instruction is complete an valid.
    if (this.isComplete())
    {
      // pre calculate the instruction pointer for the next instruction.
      // Therefore only jnz has to change it if its executed (reduced code size).
      tmpIp += this.getInstructionSize();
      switch (m_Instruction)
      {
        case ADD:
          // rd = rd + rs;
          regs.write(m_RD, (byte) (regs.read(m_RD) + regs.read(m_RS)));
          // set zeroflag if rd == 0
          if (regs.read(m_RD) == 0)
          {
            zf.write((byte) 1);
          }
          // clear zeroflag if rd != 0
          else
          {
            zf.write((byte) 0);
          }
          result = 1;
          break;

        case SUB:
          // rd = rd - rs;
          regs.write(m_RD, (byte) (regs.read(m_RD) - regs.read(m_RS)));
          // set zeroflag if rd == 0
          if (regs.read(m_RD) == 0)
          {
            zf.write((byte) 1);
          }
          // clear zeroflag if rd != 0
          else
          {
            zf.write((byte) 0);
          }
          result = 1;
          break;

        case JNZ:
          // if zf == 0 then jump[ip + dist]
          // same as:
          // if lastCalc != 0 then jump[ip + dist]
          tmpIpJnz = (int) ip.read() + (int) m_Dist;
          // make sure we arent exceeding our memory range.
          if ((tmpIpJnz >= 0) && (tmpIpJnz < mem.length))
          {
            // jump destination in range, check zeroflag.
            if (zf.read() == (byte) 0)
            {
              // jump! overwrite the previous calculated ip.
              tmpIp = tmpIpJnz;
            }
            result = 1;
          }
          break;

        case RRMOV:
          // rd = rs;
          regs.write(m_RD, regs.read(m_RS));
          result = 1;
          break;

        case RMMOV:
          // MEM[rd + displacement] = rs
          // same as:
          // MEM[esi + displacement] = rs
          tmpEa = (int) regs.read(m_RD) + m_Displacement;
          // make sure we arent exceeding our memory range.
          if ((tmpEa >= 0) && (tmpEa < mem.length))
          {
            mem[tmpEa] = regs.read(m_RS);
            result = 1;
          }
          break;

        case MRMOV:
          // rd = MEM[rs + displacement]
          // same as:
          // rd = MEM[edi + displacement]
          tmpEa = (int) regs.read(m_RS) + m_Displacement;
          // make sure we arent exceeding our memory range.
          if ((tmpEa >= 0) && (tmpEa < mem.length))
          {
            regs.write(m_RD, mem[tmpEa]);
            result = 1;
          }
          break;

        case HLT:
          // return 0 to stop the processor.
          result = 0;
          break;

        default:
          break;
      }
    }

    // write instruction pointer if no error occured
    if (result >= 0)
    {
      ip.write((byte) tmpIp);
    }

    return result;
  }

}
