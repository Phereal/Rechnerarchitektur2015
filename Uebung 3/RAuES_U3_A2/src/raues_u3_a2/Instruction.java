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
    
    switch(this.m_Instruction)
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

  public boolean set(byte opCode, byte middle, byte displacement, int argCnt)
  {
    boolean result = true;
    
    if(!this.setOpCode(opCode))
    {
      result = false;
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
      if(this.isComplete())
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
            this.m_RS = (byte)((middle >> 3) & 0x07);
            this.m_Dist = 0;

            result = true;
          }
          break;

        case K_OP_CODE_MRM:
          if (((middle & 0xC0) == 0x40) && ((middle & 0x07) == 0x06)) // MRMOV
          {
            instruction = InstructionSet.MRMOV;
            this.m_RD = (byte)((middle >> 3) & 0x07);
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
    
    if(result)
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
      if( !this.isComplete() )
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

  public boolean isValid()
  {
    boolean valid = false;
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


  
}
