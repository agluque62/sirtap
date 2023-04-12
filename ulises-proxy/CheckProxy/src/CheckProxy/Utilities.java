package CheckProxy;

//import java.util.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
//import java.net.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Utilities {	
	
	
	public static boolean validateIPV4(final String ip) {
	    String PATTERN = "^((0|1\\d?\\d?|2[0-4]?\\d?|25[0-5]?|[3-9]\\d?)\\.){3}(0|1\\d?\\d?|2[0-4]?\\d?|25[0-5]?|[3-9]\\d?)$";

	    return ip.matches(PATTERN);
	}
	
	/***********************************************************************************/
	/* Retorna la IpVirtual de la máquina virtual Linux.
	 * Si no tiene activa la Ip virtual, entonces retorna null
	 */
	public static String GetIpVirtual()
	{
		/*
		
		String ret = null;
		
		try {
			NetworkInterface nic = NetworkInterface.getByName("eth0");
			Enumeration<NetworkInterface> subinterfaces = nic.getSubInterfaces();  //Buscamos eth0:0, la cual tiene la ip virtual
			
			while (subinterfaces.hasMoreElements())
			{
				NetworkInterface interf = subinterfaces.nextElement();
				if (interf.getName().equals("eth0:0"))
				{
					List<InterfaceAddress> int_list = interf.getInterfaceAddresses();
					for (int i = 0; i < int_list.size(); i++)
					{
						ret = int_list.get(i).getAddress().getHostAddress();
					}
				}				
			}
		} catch (Exception e) {
			Main.MylogError(e.toString()+" "+e.getMessage());
			Main.Milog("GetIpVirtual: No se pueden leer la interfaz eth0 ni eth0:0");
			ret = null;
		}
		
		if (ret != null)
		{
			if (validateIPV4(ret) == false)
			{
				ret = null;
			}
		}
		
		return ret;
		
		*/
		
		/*Invocando executecommand*/
		String ret = null;
		try
		{
			StringBuffer output = executeCommand("ifconfig eth0:0");
			int ind = output.indexOf("inet addr:");
			
			if (ind != -1)
			{
				//Extraemos la ip virtual
				int indstart = output.indexOf(":", ind);
				indstart += 1;
				
				int indend = output.indexOf(" ", indstart);
				ret = output.substring(indstart, indend);
			}
		}
		catch (Exception e)
		{
			ret = null;
		}
		
		return ret;
		
		
	}
	
	/***********************************************************************************/
	/* Ejecuta un comando del sistema operativo
	 * Retorna la salida que se produce
	 */
	public static StringBuffer executeCommand(String command) {

		StringBuffer output = new StringBuffer();

		Process p;
		try {
			p = Runtime.getRuntime().exec(command);
			p.waitFor();
			BufferedReader reader =
                            new BufferedReader(new InputStreamReader(p.getInputStream()));

                        String line = "";
			while ((line = reader.readLine())!= null) {
				output.append(line + "\n");
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		return output;
	}	
	
	/**************************************************************************************************/
	/* Obtiene el MD5 de un fichero
	 * Retorna null si se produce un error 							
	 */
		
	public static String byteToHex(byte num) {
	    char[] hexDigits = new char[2];
	    hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
	    hexDigits[1] = Character.forDigit((num & 0xF), 16);
	    return new String(hexDigits);
	}
	
	public static String encodeHexString(byte[] byteArray) {
	    StringBuffer hexStringBuffer = new StringBuffer();
	    for (int i = 0; i < byteArray.length; i++) {
	        hexStringBuffer.append(byteToHex(byteArray[i]));
	    }
	    return hexStringBuffer.toString();
	}
	
	public static String GetMD5(String path)
	{
		String checksum = null;
		FileInputStream fis = null;
        try {
            fis = new FileInputStream(path);
            MessageDigest md = MessageDigest.getInstance("MD5");
          
            //Using MessageDigest update() method to provide input
            byte[] buffer = new byte[8192];
            int numOfBytesRead;
            while (true)
            {
            	numOfBytesRead = fis.read(buffer);
            	if (numOfBytesRead > 0)
            		md.update(buffer, 0, numOfBytesRead);
            	else
            		break;
            }
            byte[] hash = md.digest();
            checksum = encodeHexString(hash);
        } catch (IOException ex) {
        	Main.MylogError(ex.toString()+" "+ex.getMessage());
        	Main.MylogError("GetMD5: No se pueden acceder al fichero "+path);
        } catch (NoSuchAlgorithmException ex) {
        	Main.MylogError(ex.toString()+" "+ex.getMessage());
        	Main.MylogError("GetMD5: NoSuchAlgorithmException MD5 "+path);
        }
        
        if (fis != null)
        {
        	try {
				fis.close();
			} catch (IOException e) {
			}
        }
          
       return checksum;
    }
}
