package Servers;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Vector;

import Tools.AudioChunk;
import Tools.HardCodedValues;

//TODO ADD Spectator MODE
public class Server implements Runnable {
	private ServerSocket connexionServerSocket;
	private ServerSocket dataServerSocket;
	private Vector<ServeClient> clients;
	private Vector<DataOutputStream> musicienConnCanal;
	private Vector<DataOutputStream> musicienDataCanal;
	private HashMap<DataOutputStream, HashMap<Float, byte[]>> mixer;

	private final int MAX;
	private final int timeout;
	private Integer numberOfClients = 0;
	private String style = null;
	private int tempo = HardCodedValues.DEFAULT_TEMPO;
	private float tick = 0;
	private float currentTick = 0;

	private Boolean jamCanBePlayed = new Boolean(false);

	public Server(int maximum, int timeout, int port) {
		this.MAX = Math
				.max(maximum, HardCodedValues.DEFAULT_MAX_NMB_OF_CLIENTS);
		this.timeout = Math.max(timeout, HardCodedValues.DEFAULT_TIMEOUT);
		clients = new Vector<ServeClient>(MAX);
		musicienConnCanal = new Vector<DataOutputStream>(MAX);
		musicienDataCanal = new Vector<DataOutputStream>(MAX);
		mixer = new HashMap<DataOutputStream, HashMap<Float, byte[]>>(MAX);
		try {
			connexionServerSocket = new ServerSocket(port);
			dataServerSocket = new ServerSocket(0);
			dataServerSocket.setSoTimeout(HardCodedValues.SOCKET_TIMEOUT);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public int getTimeout() {
		return this.timeout;
	}

	public void setStyleAndTempo(String style, int tempo) {
		this.style = style;
		if (this.tempo < tempo) {
			this.tempo = tempo;
		}
		tick = this.tempo / HardCodedValues.DEFAULT_TEMPO;
		HardCodedValues.WAITING_TIME *= tick;
	}

	public int getTempo() {
		return tempo;
	}

	public String getStyle() {
		return this.style;
	}

	public ServerSocket getDataServer() {
		return this.dataServerSocket;
	}

	public Vector<String> getNames() {
		Vector<String> names = new Vector<String>();
		synchronized (clients) {
			for (ServeClient client : clients) {
				names.add(client.getName());
			}
		}
		return names;
	}

	public boolean nameExist(String name) {
		return getNames().contains(name);
	}

	@Override
	public void run() {
		try {
			while (true) {
				HardCodedValues.sleep(1);
				Socket socket = connexionServerSocket.accept();
				ServeClient serveClient = new ServeClient(this, socket);
				Thread sc = new Thread(serveClient);
				sc.start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public boolean isFull() {
		synchronized (numberOfClients) {
			return numberOfClients > MAX;
		}
	}

	public boolean isFirstClient(DataOutputStream connOut) {
		return musicienConnCanal.indexOf(connOut) == 0;
	}

	public void writeConnectionMessageToAll(String s) throws IOException {
		for (DataOutputStream outs : musicienConnCanal) {
			outs.writeBytes(s + HardCodedValues.endLine);
		}
	}

	public void writeMyMessageToAllExceptMe(String s, DataOutputStream me)
			throws IOException {
		for (DataOutputStream outs : musicienConnCanal) {
			if (me != outs)
				outs.writeBytes(s + HardCodedValues.SEPARATOR
						+ HardCodedValues.endLine);
		}
	}

	public int nmbMusicien() {
		return numberOfClients;
	}

	public boolean firstConnectionEstablished() {
		return (numberOfClients > 0) && (style != null);
	}

	public void addConnectionStream(DataOutputStream connectionCanal) {
		musicienConnCanal.add(connectionCanal);
	}

	private void removeStream(DataOutputStream connCanal) {
		musicienConnCanal.remove(connCanal);
		try {
			connCanal.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void addClient(ServeClient client) {
		numberOfClients++;
		clients.add(client);

	}

	public void removeClient(ServeClient client, DataOutputStream connCanal,
			DataOutputStream dataCanal) {
		int indexOfClientToRemove = clients.indexOf(client);
		clients.remove(client);
		removeStream(connCanal);
		removeDataCanal(dataCanal);
		mixer.remove(dataCanal);

		numberOfClients--;
		if (numberOfClients > 0) {
			if (indexOfClientToRemove == 0) {
				// TODO Ask the next client to set style and tempo
			}
		} else {
			style = null;
			tempo = HardCodedValues.DEFAULT_TEMPO;
		}

		if (numberOfClients <= 1) {
			jamCanBePlayed = false;
		}
	}

	private void removeDataCanal(DataOutputStream dataCanal) {
		musicienDataCanal.remove(dataCanal);
	}

	public void addDataCanal(DataOutputStream dataCanal) {
		musicienDataCanal.add(dataCanal);
		if (numberOfClients > 1 && !jamCanBePlayed) {
			jamCanBePlayed = true;
		}
	}

	public Boolean getJamCanBePlayed() {
		return jamCanBePlayed;
	}

	public boolean canPlayJam() {
		return jamCanBePlayed && (style != null);
	}

	public boolean mixAudioExceptMine(DataOutputStream dataOutCanal,
			AudioChunk audioChunk) {
		byte[] audio = audioChunk.getAudioBuff();
		float tickToMix = audioChunk.getTick();
		if (!musicienDataCanal.contains(dataOutCanal)) {
			return false;
		}
		setCurrentTick(tickToMix);
		int currentNumbOfClient;
		synchronized (numberOfClients) {
			currentNumbOfClient=numberOfClients;
		}
		DataOutputStream out;
		
		for (int i=0; i<currentNumbOfClient;i++) {
			synchronized (musicienDataCanal) {
				if(i>=musicienDataCanal.size()){
					break;
				} else {
					out = musicienDataCanal.get(i);
				}
			}
			
			if (out == dataOutCanal) {
				continue;
			}
			synchronized (mixer) {
				if (!mixer.containsKey(out)) {
					HashMap<Float, byte[]> map = new HashMap<Float, byte[]>();
					map.put(tickToMix, audio);
					mixer.put(out, map);
					continue;
				}
			}

			int len = audio.length;
			byte[] result = new byte[len];
			HashMap<Float, byte[]> map;
			byte[] current = null;

			synchronized (mixer) {
				map = mixer.get(out);
				current = map.get(tickToMix);
			}
			result = HardCodedValues.mixBuffer(current, audio);
			synchronized (mixer) {
				map.put(tickToMix, result);
				mixer.put(out, map);
			}
		}
		return true;
	}

	public boolean sendToMeBuffAtTick(DataOutputStream myData, float tickToSend) throws IOException {
		HashMap<Float, byte[]> map;
		synchronized (mixer) {
			map = mixer.get(myData);
		}

		if (map != null) {
			byte[] temp;
			synchronized (map) {
				temp = map.get(tickToSend);
			}
			if (temp != null) {
				int len = temp.length;
				if (len > 0) {
						sendBuffer(myData, temp);
						synchronized (mixer) {
							mixer.get(myData).remove(tickToSend);
						}
				}
			} else {
				return false;
			}
		}
		return true;
	}

	private void sendBuffer(DataOutputStream myData, byte[] buffer)
			throws IOException {
		myData.writeBytes(HardCodedValues.MixingSetting.MIX
				+ HardCodedValues.SEPARATOR);
		for (int i = 0; i < buffer.length; i++) {
			if (buffer[i] == HardCodedValues.endlineToByte || buffer[i] == 11
					|| buffer[i] == 12 || buffer[i] == 13) {
				buffer[i]= 14;
			} else if (buffer[i] == HardCodedValues.SepToByte) {
				buffer[i] += 1;
			}  
				myData.write(buffer[i]);
		}
		
		myData.write(HardCodedValues.SepToByte);
		myData.write(HardCodedValues.endlineToByte);
	}

	public float getCurrentTick() {
		return currentTick;
	}

	public void setCurrentTick(float lastTick) {
		if (this.currentTick < lastTick) {
			this.currentTick += tick;
		}
	}
}
