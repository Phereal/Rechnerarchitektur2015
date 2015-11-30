package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class RegisterSet
{

  // count of our registers.
  public static final byte K_REGISTER_COUNT = 8;
  // holds the registers.
  private Register[] m_RegisterSet;
  
  /**
   * Constructor. Initialised the eigth registers of the Y86 as stated in scope
   * of this exercise.
   * @throws Exception 
   */
  RegisterSet() throws Exception
  {
    try
    {
      // create the registers.
      m_RegisterSet = new Register[K_REGISTER_COUNT];
      m_RegisterSet[(byte) 0x00] = new Register("eax");
      m_RegisterSet[(byte) 0x01] = new Register("ecx");
      m_RegisterSet[(byte) 0x02] = new Register("edx");
      m_RegisterSet[(byte) 0x03] = new Register("ebx");
      m_RegisterSet[(byte) 0x04] = new Register("eex");
      m_RegisterSet[(byte) 0x05] = new Register("efx");
      m_RegisterSet[(byte) 0x06] = new Register("esi");
      m_RegisterSet[(byte) 0x07] = new Register("edi");
    }
    catch (Exception e)
    {
      throw e;
    }
  }

  /**
   * Returns the name of the register addressed with the passed index.
   * @param register index of the register as stated in the scope of the exercise.
   * @return register name
   */
  public String getRegisterName(byte register)
  {
    // make sure the passed index is valid.
    if(register >= 0 && register < K_REGISTER_COUNT)
    {
      return m_RegisterSet[register].getName();
    }
    else
    {
      return null;
    }
  }

}
