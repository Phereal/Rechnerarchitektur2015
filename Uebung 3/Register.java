/**
 * Das Register für die Variante des Y86-Prozessors. Nur über diese Klasse
 * können Register beschrieben und gelesen werden.
 *
 */
public class Register {

	/**
	 * Ein 8-Bit-Datenregister. Die Register 0-5 dienen Ausschließlich zur
	 * Datenspeicherung. Die Register 6-7 werden zur Indizierung von
	 * Datenfeldern genutzt.
	 */
	private int[] dataRegister = new int[8];

	/**
	 * Der Instruction Pointer speichert die Adresse des nächsten auszuführenden
	 * Befehls im Speicher.
	 */
	private int instructionPointer;

	/**
	 * Die Zero Flag wird auf True gesetzt, wenn die letzte arithmetische
	 * Operation 0 ergab.
	 */
	private boolean zeroFlag;

	

	public Register() 
	{
	}
	
	
	public int readDataRegister ( int index)
	{
		return dataRegister[index];
	}
	
	public int readInstructionPointer()
	{
		return instructionPointer;
	}
	
	public boolean readZeroFlag()
	{
		return zeroFlag;
	}

	/**
	 * Schreibt in einen 8-Bit-Wert in ein Register.
	 * Es werden beliebige Integer akzeptiert, allerdings erzeugen
	 * Werte außerhalb des Wertebereiches von 0-255 eine Exception.
	 * 
	 * @param index - Index des zu schreibenden Registers
	 * @param value - Ins Register zu schreibender Wert
	 */
	public void writeDataRegister (int index, int value)
	{
		//index gültig?
		if (index < 0 || 7 < index)
		{
			throw new IllegalArgumentException
			("Der Integer 'index' (" + index + ") ist kein gültiges Register.");
		}
		
		//value gültig?
		else if (value < 0x00 || 0xFF < value)
		{
			throw new IllegalArgumentException
			("Der Integer 'value' (" + value + ") ist kein 8-Bit-Wert und kann nicht vom Register gespeichert werden.");
		}
		
		else 
		{
			dataRegister[index] = value;
		}
	}
	
	
	/**
	 * Schreibt auf den Instruction Pointer.
	 * 
	 * @param value
	 */
	public void writeInstructionPointer(int value)
	{		
		if (value < 0x00) //Integer overflow von Java behandelt
		{
			throw new IllegalArgumentException
			("Der Integer 'value' (" + value + ") ist signed, obwohl ein unsigned Wert erwartet wird.");
		}
		else
		{
			instructionPointer = value;
		}
	}
	
	
	public void writeZeroFlag(boolean value)
	{
		zeroFlag = value;
	}
}
