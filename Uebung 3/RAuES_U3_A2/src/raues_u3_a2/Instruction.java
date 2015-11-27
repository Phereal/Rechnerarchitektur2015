package raues_u3_a2;

/**
 *
 * @author Steffen
 */
public class Instruction
{

  public enum InstructionSet
  {
    ERROR, ADD, SUB, JNZ, RRMOV, RMMOV, MRMOV, HLT
  }

  private enum InstructionTypes
  {
    ERROR, REGISTER, PSEUDODIRECT, BASE, SPECIAL
  }

  private static final byte K_OP_CODE_ADD = (byte) 0x01;
  private static final byte K_OP_CODE_SUB = (byte) 0x29;
  private static final byte K_OP_CODE_JNZ = (byte) 0x75;
  private static final byte K_OP_CODE_RRM_RMM = (byte) 0x89;
  private static final byte K_OP_CODE_MRM = (byte) 0x8B;
  private static final byte K_OP_CODE_HLT = (byte) 0xF4;

  // Machine code
  private byte m_OpCode = 0;
  private byte m_2ndByte = 0;
  private byte m_Displacement = 0;

  private boolean m_OpCodeOk = false;
  private boolean m_2ndByteOk = false;
  private boolean m_DisplacementOk = false;

  private byte m_RS = 0;
  private byte m_RD = 0;
  private byte m_Dist = 0;

  private InstructionSet m_Instruction = InstructionSet.ERROR;

  Instruction()
  {
  }

  public InstructionSet getInstruction()
  {
    return m_Instruction;
  }

  public String getInstructionString(RegisterSet registerSet)
  {
    String result;

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
        result = "RMmov ";
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
  
  
  public String getInstructionString(RegisterSet registerSet, boolean useHex)
  {
    String result;

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
        if(useHex)
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
        if(useHex)
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
        if(useHex)
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

  public int set(byte opCode, byte middle, byte displacement, int argCnt)
  {
    int result = 0;

    if ((argCnt <= 0) || (argCnt > 3))
    {
      result = -1;
    }

    if ((result == 0) && (!this.setOpCode(opCode)))
    {
      result = -1;
    }

    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }

    // check 2ndByte
    if ((result == 0) && (argCnt == 1))
    {
      result = -1;
    }

    if ((result == 0) && (!this.set2ndByte(middle)))
    {
      result = -1;
    }

    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }
    
    
    if ((result == 0) && (argCnt == 2))
    {
      result = -1;
    }

    if ((result == 0) && (!this.setDisplacement(middle)))
    {
      result = -1;
    }

    if ((result == 0) && (this.isComplete()))
    {
      result = 1;
    }

    // cleanup
    if(result == -1)
    {
      this.clear();
    }
    
    return result;
  }

  public boolean setOpCode(byte opCode)
  {
    boolean result = false;

    if (opCodeValid(opCode) == true)
    {
      this.clear();
      this.m_OpCode = opCode;
      this.m_OpCodeOk = true;
      if (this.isComplete())
      {
        this.m_Instruction = InstructionSet.HLT;
      }
      result = true;
    }

    return result;
  }

  public boolean set2ndByte(byte middle)
  {
    boolean result = false;
    InstructionSet instruction = this.m_Instruction;

    if (this.m_OpCodeOk)
    {
      switch (this.m_OpCode)
      {
        case K_OP_CODE_ADD:
          if ((middle & 0xC0) == 0xC0)
          {
            instruction = InstructionSet.ADD;
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_SUB:
          if ((middle & 0xC0) == 0xC0)
          {
            instruction = InstructionSet.SUB;
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_JNZ:
          instruction = InstructionSet.JNZ;
          this.m_RD = 0;
          this.m_RS = 0;
          this.m_Dist = middle;

          result = true;
          break;

        case K_OP_CODE_RRM_RMM:
          if ((middle & 0xC0) == 0xC0) // RRMOV
          {
            instruction = InstructionSet.RRMOV;
            this.m_RD = (byte) (middle & 0x07);
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          else if (((middle & 0xC0) == 0x40) && ((middle & 0x07) == 0x06)) // RMMOV
          {
            instruction = InstructionSet.RMMOV;
            this.m_RD = (byte) (7); // edi
            this.m_RS = (byte) ((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_MRM:
          if (((middle & 0xC0) == 0x40) && ((middle & 0x07) == 0x06)) // MRMOV
          {
            instruction = InstructionSet.MRMOV;
            this.m_RD = (byte) ((middle >> 3) & 0x07);
            this.m_RS = (byte) (6); // esi
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_HLT:
        default:
          break;
      }
    }

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

  public boolean setDisplacement(byte displacement)
  {
    boolean result = false;

    if (this.m_OpCodeOk && this.m_2ndByteOk)
    {
      if (!this.isComplete())
      {
        this.m_Displacement = displacement;
        this.m_DisplacementOk = true;
        result = true;
      }
    }

    return result;
  }

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
  }

  public boolean opCodeValid()
  {
    boolean valid;

    switch (m_OpCode)
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



  public static boolean opCodeValid(byte opCode)
  {
    boolean valid;

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

  public static boolean middleValid(byte opCode, byte middle)
  {
    boolean valid = false;

    switch (opCode)
    {
      case K_OP_CODE_ADD:
      case K_OP_CODE_SUB:
        break;

      case K_OP_CODE_JNZ:
        break;

      case K_OP_CODE_RRM_RMM:
        break;

      case K_OP_CODE_MRM:
        break;

      case K_OP_CODE_HLT:
        valid = true;
        break;

      default:
        valid = false;
        break;
    }

    return valid;
  }

  public static boolean isValid(byte opCode, byte middle, byte displacement)
  {
    boolean valid;

    valid = opCodeValid(opCode);

    if (valid == true)
    {
      valid = true;
    }

    return valid;
  }

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

  public boolean isComplete()
  {
    boolean result = false;

    if (this.m_OpCodeOk)
    {
      switch (this.m_OpCode)
      {
        case K_OP_CODE_ADD:
        case K_OP_CODE_SUB:
        case K_OP_CODE_JNZ:
          if (m_2ndByteOk)
          {
            result = true;
          }
          break;

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

        case K_OP_CODE_MRM:
          if (this.m_2ndByteOk && this.m_DisplacementOk)
          {
            result = true;
          }
          break;

        case K_OP_CODE_HLT:
          result = true;
          break;

        default:
          break;
      }
    }
    return result;
  }

  
  public int execute(Register ip, Register zf, RegisterSet regs, byte[] mem) throws Exception
  {
    // todo: ZF nach redem befehl zuruecksetzen?
    int result = -1;
    int tmpEa;
    int tmpIp = ip.read();
    int tmpIpJnz;
    
    if(this.isComplete())
    {
      tmpIp += this.getInstructionSize();
      switch(m_Instruction)
      {
        case ADD:
          regs.write(m_RD, (byte)(regs.read(m_RD) + regs.read(m_RS)));
          if(regs.read(m_RD) == 0)
          {
            zf.write((byte)1);
          }
          else
          {
            zf.write((byte)0);
          }
          result = 1;
          break;
          
        case SUB:
          regs.write(m_RD, (byte)(regs.read(m_RD) - regs.read(m_RS)));
          if(regs.read(m_RD) == 0)
          {
            zf.write((byte)1);
          }
          else
          {
            zf.write((byte)0);
          }
          result = 1;
          break;
          
        case JNZ:
          tmpIpJnz = (int)ip.read() + (int)m_Dist;
          if((tmpIpJnz >= 0) && (tmpIpJnz < mem.length))
          {
            if(zf.read() != (byte)0)
            {
              tmpIp = tmpIpJnz;
            }
            result = 1;
          }
          break;
          
        case RRMOV:
          regs.write(m_RD, regs.read(m_RS));
          result = 1;
          break;
          
        case RMMOV:
          tmpEa = (int)regs.read(m_RD) + m_Displacement;
          if((tmpEa >= 0) && (tmpEa < mem.length))
          {
            mem[tmpEa] = regs.read(m_RS);
            result = 1;
          }
          break;
          
        case MRMOV:
          tmpEa = (int)regs.read(m_RS) + m_Displacement;
          if((tmpEa >= 0) && (tmpEa < mem.length))
          {
            regs.write(m_RD, mem[tmpEa]);
            result = 1;
          }
          break;
          
        case HLT:
          result = 0;
          break;
          
        default:
          break;
      }
    }
    
    if(result >= 0)
    {
      ip.write((byte)tmpIp);
    }
    
    return result;
  }
  
}
