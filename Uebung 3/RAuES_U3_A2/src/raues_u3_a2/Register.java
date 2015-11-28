package raues_u3_a2;

/**
 *
 * @author Steffen
 */
public class Register
{

  private String m_Name;
  private byte m_Value = 0;

  Register(String name) throws Exception
  {
    if ((name == null) || (name.isEmpty()))
    {
      throw new Exception("Fehler: Kann Register aufgrund ungueltigem Namens nicht anlegen!");
    }

    m_Name = name;
  }

  public String getName()
  {
    return m_Name;
  }

  public byte read()
  {
    return m_Value;
  }

  public void write(byte value)
  {
    m_Value = value;
  }
}
