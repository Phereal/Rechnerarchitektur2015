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
  // size of memory
  private static final int K_MAX_BUFFER_LENGTH = 80;
  
  // used to access the mem file.
  private static File memFile;
  private static BufferedInputStream memBIS = null;

  private static long memSize = 0;
  // holds the amount of actually read byte from the mem file.
  private static int byteRead = 0;

  /**
   * Forbid instanticiation via private constructor.
   */
  private MemReader()
  {
  }

  /**
   * Reads the given memory file (up to 80 byte) and returns it as a byte array.
   * @param fileName path to the binary coded memory file.
   * @return byte array of the memory file.
   */
  public static byte[] readMemFile(String fileName)
  {
    // holds the read data and will be returned at the end.
    byte memBuffer[];

    try
    {
      memFile = new File(fileName);
      // use of a buffered input stream may not be needed for such small files
      // and only one thread, but shouldt hurt.
      memBIS = new BufferedInputStream(new FileInputStream(memFile));
      memSize = memFile.length();

      // ignore more than 80 bytes, since our processor only has that much
      // internal memory.
      if (memSize > K_MAX_BUFFER_LENGTH)
      {
        // set mem size to our max size of 80 (used as loop condition).
        memSize = K_MAX_BUFFER_LENGTH;
        // no need to quit the program, we just print a warning for the user
        // and ignore the abundant bytes.
        System.out.println("Achtung: memFile > 80. Zusaetzliche Bytes werden ignoriert!");
      }

      // no need to process an empty mem file.
      if (memSize <= 0)
      {
          // error handling in catch block.
        throw new Exception("Fehler in MemReader.readMemFile(): MemFile ist leer.");
      }

      // allocate enough memory for our buffer to hold the mem file.
      memBuffer = new byte[(int) memSize];
      // read every single byte.
      for (byteRead = 0; byteRead < memSize; ++byteRead)
      {
        // abort process, if for some reason an error occured during read.
        if (memBIS.available() < 1)
        {
          // error handling in catch block.
          throw new Exception("Fehler: Lesen von memFile Buffer fehlgeschlagen");
        }
        // save the currently read byte.
        memBuffer[byteRead] = (byte) memBIS.read();
      }

    }
    // error handling if something wen wrong.
    // => clear static members and the returned buffer. Print the error.
    // e.g. empty string.
    catch (NullPointerException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    // e.g. file does not exist.
    catch (FileNotFoundException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    // e.g. error while reading the file.
    catch (IOException e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    // e.g. anything else and our own Exceptions.
    catch (Exception e)
    {
      byteRead = 0;
      memBuffer = null;
      System.out.println(e);
    }
    // always make sure we close the stream if we opened it.
    finally
    {
      if (memBIS != null)
      {
        // stream was opened, try to close it.
        try
        {
          // close
          memBIS.close();
        }
        catch (Exception e)
        {
          // closing not possible, assume its close.
        }
        memBIS = null;
      }
      // clean up the static members for the next function call.
      memSize = 0;
      memFile = null;
    }

    return memBuffer;
  }
}
