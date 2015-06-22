package Tools;

public class HardCodedValues {
	public static final String SEPARATOR = "/";
	public static final String endLine = "\n";

	public static final byte endlineToByte = (byte)endLine.charAt(0);
	public static final byte SepToByte = (byte)HardCodedValues.SEPARATOR.charAt(0);
	
	public static final int BUFFER_SIZE = 128;
	public static float WAITING_TIME = 4f;
	public static final int DEFAULT_TEMPO = 60;
	public static final int DEFAULT_MAX_NMB_OF_CLIENTS = 4;
	public static final int DEFAULT_TIMEOUT = 1000;
	public final static int MAX_NB_PORTS = (int) (Math.pow(2, 16) - 1);
	public final static int DEFAULT_PORT = 2015;
	public final static int MINIMUM_NB_MUSICIEN = 4;

	public final static int SOCKET_TIMEOUT =HardCodedValues.DEFAULT_TIMEOUT*10;
	public static class ConnectDisconnect {
		static public final String CONNECT = "CONNECT";
		static public final String WELCOME = "WELCOME";
		static public final String PORT_AUDIO = "AUDIO_PORT";
		static public final String CONNECTED = "CONNECTED";
		static public final String EXIT = "EXIT";
		static public final String EXITED = "EXITED";
	}

	public static class JamSetting {
		static public final String EMPTY = "EMPTY_SESSION";
		static public final String FULL = "FULL_SESSION";
		static public final String CURRENT = "CURRENT_SESSION";
		static public final String SET_OPT = "SET_OPTIONS";
		static public final String OPT_RECEIVED = "ACK_OPTS";
	}

	public static class AudioSetting {
		static public final String SYNC = "AUDIO_SYNC";
		static public final String CHUNK = "AUDIO_CHUNK";
		static public final String OK = "AUDIO_OK";
		static public final String KO = "AUDIO_KO";
	}

	public static class MixingSetting {
		public static final String MIX = "AUDIO_MIX";
		public static final String ACK = "AUDIO_ACK";
	}

	public static class ChatSetting {
		public static final String TALK = "TALK";
		public static final String LISTEN = "LISTEN";
	}

	public static void sleep(long millisec) {
		try {
			Thread.sleep(millisec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public static byte charToByte(char c) {
		return (byte) c;
	}
	
	public static byte[] stringToByteArray(String audioPart) {
		if (audioPart == null) {
			return null;
		}
		int length = audioPart.length();
		byte[] result = new byte[length];
		for (int i = 0; i < length; i++) {
			result[i] = charToByte(audioPart.charAt(i));
		}
		return result;
	}

	public static byte mixByte(byte first, byte second) {
		return (byte)((first+second)>>1);
	}
	
	
	/**
	 * Mix depending on the size of the buffer received and the
	 * current buffer Just in case the size of the two buffers are
	 * different.
	 */

	public static byte[] mixBuffer(byte[] first, byte[] second) {
		byte[] result;
		if (first != null) {
			if(second==null){
				return first;
			}
			int len_current = first.length;
			int len = second.length;
			
			result = new byte[Math.max(len_current, len)];
			if (len < len_current) {
				int i = 0;
				for (; i < len; i++) {
					result[i] = mixByte(first[i], second[i]);
					}
				for (; i < len_current; i++) {
					result[i] = first[i];
				}
			} else if (len > len_current) {
				int i = 0;
				for (; i < len_current; i++) {
					result[i] = mixByte(first[i], second[i]);
				}
				for (; i < len; i++) {
					result[i] = second[i];
				}
			} else {
				for (int i = 0; i < len; i++) {
					result[i] = mixByte(first[i], second[i]);
				}
			}
		} else {
			return second;
		}
		return result;
	}

}