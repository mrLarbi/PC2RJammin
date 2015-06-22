package Mains;

import Servers.Server;
import Tools.HardCodedValues;

public class ServerMain {
	int max = 0;
	int timeout = 0;
	int port = 0;

	public ServerMain (String[] args) {
		treatCommands(args); 
	}
	private void treatCommands(String[] args) {
		if (args != null) {
			int len = args.length;
			if (len > 0) {
				if ((len % 2) == 0) {
					// Treats command line arguments
					if (args[0].equalsIgnoreCase("-max")) {
						max = Integer.parseInt(args[1]);
						len-=2;
						if(len>0)
						if (args[2].equalsIgnoreCase("-timeout")) {
							timeout = Integer.parseInt(args[3]);
							len-=2;
							if(len>0)
							if (args[4].equalsIgnoreCase("-port")) {
								port =  Integer.parseInt(args[5]);
							}
						} else if (args[2].equalsIgnoreCase("-port")) {
							port = Integer.parseInt(args[3]);
						}
					} else if (args[0].equalsIgnoreCase("-timeout")) {
						timeout = Integer.parseInt(args[1]);
						len-=2;
						if(len>0)
						if (args[2].equalsIgnoreCase("-port")) {
							port = Integer.parseInt(args[3]);
						}
					} else if (args[0].equalsIgnoreCase("-port")) {
						port = Integer.parseInt(args[1]);
					}
				}
				if (max == 0 && port == 0 && timeout == 0
						& args.length > 0) {
					System.out.println("Error: Arguments missing");
					System.out
							.println("System launched with default commmands");
				}
			} 
			}
		else {
				System.out.println("Server launched with default commands");
			} 
		checkOptionsAndSetToDefault();
	}

	public void checkOptionsAndSetToDefault() {
		max = Math.max(max, HardCodedValues.MINIMUM_NB_MUSICIEN); 
		timeout= Math.max(HardCodedValues.DEFAULT_TIMEOUT, timeout);
		if((port <= 0) || (port> HardCodedValues.MAX_NB_PORTS))
		{
			port=HardCodedValues.DEFAULT_PORT;
		}
	}
	
	public static void main(String[] args) {
		ServerMain arg = new ServerMain(args);
		Thread server = new Thread(new Server(arg.max, arg.timeout, arg.port));
		server.start();
	}
}
