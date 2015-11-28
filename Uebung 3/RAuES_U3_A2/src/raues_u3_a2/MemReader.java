package raues_u3_a2;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 *
 * @author Steffen
 */
public final class MemReader
{

  private static final int K_MAX_BUFFER_LENGTH = 80;

  private static File memFile;
  private static BufferedInputStream memBIS = null;

  private static long memSize = 0;
  private static int byteRead = 0;

  private MemReader()
  {
  }

  public static byte[] readMemFile(String fileName)
  {
    byte memBuffer[];

    try
    {
      memFile = new File(fileName);
      memBIS = new BufferedInputStream(new FileInputStream(memFile));
      memSize = memFile.length();

      if (memSize > K_MAX_BUFFER_LENGTH)
      {
        memSize = K_MAX_BUFFER_LENGTH;
        System.out.println("Achtung: memFile > 80. Zusaetzliche Bytes werden ignoriert!");
      }

      if (memSize <= 0)
      {
        throw new Exception("Fehler in MemReader.readMemFile(): MemFile ist leer.");
      }

      memBuffer = new byte[(int) memSize];
      for (byteRead = 0; byteRead < memSize; ++byteRead)
      {
        if (memBIS.available() < 1)
        {
          throw new Exception("Fehler: Lesen von memFile Buffer fehlgeschlagen");
        }
        memBuffer[byteRead] = (byte) memBIS.read();
      }

    }
    catch (NullPointerException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    catch (FileNotFoundException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    catch (IOException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    catch (Exception e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    finally
    {
      if (memBIS != null)
      {
        try
        {
          memBIS.close();
        }
        catch (Exception e)
        {
          // do nothing
        }
        memBIS = null;
      }
      memSize = 0;
      memFile = null;
    }

    return memBuffer;
  }
}
