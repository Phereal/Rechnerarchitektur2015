package raues_u3_a2;

/**
 *
 * @author Steffen
 */
public class RegisterSet
{

  public static final byte K_REGISTER_COUNT = 8;
  private Register[] m_RegisterSet;

  RegisterSet() throws Exception
  {
    try
    {
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
  
  public String getRegisterName(byte register)
  {
    if(register >= 0 && register < K_REGISTER_COUNT)
    {
      return m_RegisterSet[register].getName();
    }
    else
    {
      return null;
    }
  }
  
  public void init(byte value)
  {
    for(int i = 0; i < m_RegisterSet.length; ++i)
    {
      m_RegisterSet[i].write(value);
    }
  }

}
