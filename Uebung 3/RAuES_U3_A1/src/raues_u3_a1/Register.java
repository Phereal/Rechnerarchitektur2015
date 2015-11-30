package raues_u3_a1;

/**
 *
 * @author Steffen
 */
public class Register
{
  // name of the register (e.g. eax, zf, esi, ...)
  private String m_Name;
  // content of the register. Byte is used since it has 8 bit in java.
  private byte m_Value = 0;

  /**
   * Constructor.
   * @param name name of the register.
   * @throws Exception 
   */
  Register(String name) throws Exception
  {
    // make sure only valid names are used.
    if ((name == null) || (name.isEmpty()))
    {
      throw new Exception("Fehler: Kann Register aufgrund ungueltigem Namens nicht anlegen!");
    }

    m_Name = name;
  }

  /**
   * Returns the name of the register.
   * @return register name.
   */
  public String getName()
  {
    return m_Name;
  }

  /**
   * Reads the content of the register.
   * @return read content of the register.
   */
  public byte read()
  {
    return m_Value;
  }

  /**
   * Writes the content of the register.
   * @param value content to be written.
   */
  public void write(byte value)
  {
    m_Value = value;
  }
}
