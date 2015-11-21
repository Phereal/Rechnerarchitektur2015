import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by keszocze on 18.11.15.
 */
public class WriteMem {

    static byte[] memory= new byte[80];


    private static void writeMemory(byte[] mem, String file) throws IOException {
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(mem);
        fos.close();
    }


    // writes the example file as provided in Stud.IP
    private static void writeExample() throws IOException {
        byte[] example_mem = new byte[]{
                0x01, (byte) 0xF8, //eax := eax + edi
                0x01, (byte) 0xD1,                //ecx := ecx + edx
                0x29, (byte) 0xF8,                //eax := eax - edi
                0x29, (byte) 0xD1,                //ecx := ecx - edx
                0x75, (byte) 0x03,                //if !ZF then IP := IP + 3
                0x75, (byte) 0xF0,                //if !ZF then IP := IP + -16
                (byte) 0x89, (byte) 0xCC,        //eex := ecx
                (byte) 0x89, 0x66, 0x40,        //MEM[esi + 64] := eex
                (byte) 0x8B, 0x76, 0x04,        //esi := MEM[esi + 4]
                (byte) 0xF4,                    //Anhalten
                (byte) 0xFF,                    //Error
                0x01, (byte) 0xF8,                //eax := eax + edi
                0x01, (byte) 0xD1,                //ecx := ecx + edx
                0x29, (byte) 0xF8,                //eax := eax - edi
                0x29, (byte) 0xD1,                //ecx := ecx - edx
                0x75, (byte) 0x03,                //if !ZF then IP := IP + 3
                0x75, (byte) 0xF0,                //if !ZF then IP := IP + -16
                (byte) 0x89, (byte) 0xCC,        //eex := ecx
                (byte) 0x89, 0x66, 0x40,        //MEM[esi + 64] := eex
                (byte) 0x8B, 0x76, 0x04,        //esi := MEM[esi + 4]
                (byte) 0x89, 0x66, 0x40,        //MEM[esi + 64] := eex
                (byte) 0x8B, 0x76, 0x04,        //esi := MEM[esi + 4]
                (byte) 0x89, 0x66, 0x40,        //MEM[esi + 64] := eex
                (byte) 0x8B, 0x76, 0x04,        //esi := MEM[esi + 4]
                (byte) 0x89, 0x66, 0x40,        //MEM[esi + 64] := eex
                (byte) 0x8B, 0x76, 0x04,        //esi := MEM[esi + 4]
                (byte) 0xF4,                    //Anhalten
                (byte) 0xFF,                    //Error
                0x01, (byte) 0xF8,                //eax := eax + edi
                0x01, (byte) 0xD1,                //ecx := ecx + edx
                0x29, (byte) 0xF8,                //eax := eax - edi
                0x29, (byte) 0xD1,                //ecx := ecx - edx
                0x75, (byte) 0x03,                //if !ZF then IP := IP + 3
                0x75, (byte) 0xF0,                //if !ZF then IP := IP + -16
                (byte) 0x89, (byte) 0xCC,        //eex := ecx
                (byte) 0x89, (byte) 0x66,  (byte)0xF0,        //MEM[esi + -16] := eex
                0x01};
        writeMemory(example_mem,"testfile.mem");
    }

    public static void main(String[] args) throws IOException {

        // generate the example file for task 1
        writeExample();



        // otherwise creaty arbitrary memory like this
        memory[0]=0x01;
        memory[1]=(byte) 0xF8;

        writeMemory(memory,"testfile.mem");

    }
}
