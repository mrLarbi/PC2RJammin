package Servers;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;

import Tools.AudioChunk;
import Tools.HardCodedValues;
import Tools.UniqueUserName;

//TODO
public class ServeClient implements Runnable {
	private final int timeout;
	private Server server;
	private Socket socket;
	private Long start, end;
	final private ServerSocket dataServerSocket;

	private DataOutputStream connctOutCanal, dataOutCanal;

	private BufferedReader connctInputCanal;
	private BufferedReader dataInputCanal;

	private String clientName;
	private int audio_port;

	private String inputLine;
	private int countKO = 0;
	private float myTick = 0f;
	private Socket dataSocket;
	private float tickToSend;

	public ServeClient(Server server, Socket socket) {
		this.server = server;
		timeout = server.getTimeout();
		dataServerSocket = server.getDataServer();
		this.socket = socket;
		try {
			connctInputCanal = new BufferedReader(new InputStreamReader(
					(socket.getInputStream())));
			connctOutCanal = new DataOutputStream(socket.getOutputStream());

		} catch (IOException e) {
			e.printStackTrace();

		}
	}

	private void generateUniqueUserName() throws IOException {
		while ((inputLine = connctInputCanal.readLine()) != null) {
			if (inputLine.startsWith(HardCodedValues.ConnectDisconnect.CONNECT)) {
				break;
			}
		}
		clientName = inputLine.split(HardCodedValues.SEPARATOR)[1];
		if (server.nameExist(clientName)) {
			clientName = UniqueUserName.generateWord(server.getNames(),
					clientName, server.nmbMusicien());
		}
	}

	private void setStyleAndTempo() throws IOException {
		writeMessageToMe(HardCodedValues.JamSetting.EMPTY);
		String style = null;
		int tempo = 0;
		while ((inputLine = connctInputCanal.readLine()) != null) {
			if (inputLine.startsWith(HardCodedValues.JamSetting.SET_OPT)) {
				String options[] = inputLine.split(HardCodedValues.SEPARATOR);
				style = options[1];
				try {
					tempo = Integer.parseInt(options[2]);
				} catch (Exception e) {
					e.printStackTrace();
					System.exit(0);
				}
				break;
			}
		}
		synchronized (server) {
			server.setStyleAndTempo(style, tempo);
			writeMessageToMe(HardCodedValues.JamSetting.OPT_RECEIVED);
			server.notifyAll();
		}
	}

	private void welcomeMessage() throws IOException {
		writeMessageToMe(HardCodedValues.ConnectDisconnect.WELCOME
				+ HardCodedValues.SEPARATOR + clientName
				+ HardCodedValues.SEPARATOR);
	}

	private void informeCurrentStyleAndTempo() throws IOException {
		synchronized (server) {

			if (!server.firstConnectionEstablished()) {
				try {
					server.wait(); // Wait for the first client to set style
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			if (!server.canPlayJam()) {
				server.notify(); // Notify
			}
		}
		writeMessageToMe(HardCodedValues.JamSetting.CURRENT
				+ HardCodedValues.SEPARATOR + server.getStyle()
				+ HardCodedValues.SEPARATOR + server.getTempo()
				+ HardCodedValues.SEPARATOR + (server.nmbMusicien() - 1)
				+ HardCodedValues.SEPARATOR);
	}

	private void addMyStreamsToServerAndInformOther() throws IOException {
		String message;
		message = HardCodedValues.ConnectDisconnect.CONNECTED
				+ HardCodedValues.SEPARATOR + clientName;
		synchronized (server) {
			server.addDataCanal(dataOutCanal);
			server.writeConnectionMessageToAll(message);
		}
	}

	private void sendAudioPort() throws IOException {
		audio_port = dataServerSocket.getLocalPort();
		writeMessageToMe(HardCodedValues.ConnectDisconnect.PORT_AUDIO
				+ HardCodedValues.SEPARATOR + audio_port
				+ HardCodedValues.SEPARATOR);
	}

	private void waitForConnectionAndCreateDataCanal() throws IOException {

		synchronized (dataServerSocket) {
			try {
				dataSocket = dataServerSocket.accept();
			} catch (SocketTimeoutException io) {
				writeMessageToMe(HardCodedValues.ConnectDisconnect.EXITED
						+ HardCodedValues.SEPARATOR + clientName);
				server.removeClient(this, connctOutCanal, null);
				System.exit(0);
			}
		}

		sendAudioOk();
		dataInputCanal = new BufferedReader(new InputStreamReader(
				(dataSocket.getInputStream())));
		dataOutCanal = new DataOutputStream(dataSocket.getOutputStream());

	}

	private void sendAudioOk() throws IOException {
		writeMessageToMe(HardCodedValues.AudioSetting.OK);
	}

	private void sendAudioSync() throws IOException {
		tickToSend=myTick = server.getCurrentTick();
		
		writeMessageToMe(HardCodedValues.AudioSetting.SYNC
				+ HardCodedValues.SEPARATOR + myTick
				+ HardCodedValues.SEPARATOR);
	}

	private boolean timer() throws IOException {
		if (end - start > timeout) {
			if (countKO++ < 8) {
				writeMessageToMe(HardCodedValues.AudioSetting.KO);
			} else {
				countKO = 0;
				return false;
			}
		}
		return true;
	}

	private void informMeServerIsFull() {
		String message;
		try {
			inputLine = connctInputCanal.readLine();
			clientName = inputLine.split(HardCodedValues.SEPARATOR)[1];
			message = HardCodedValues.ConnectDisconnect.EXIT
					+ HardCodedValues.SEPARATOR + clientName;

			writeMessageToMe(HardCodedValues.JamSetting.FULL);
			writeMessageToMe(message);

			connctOutCanal.close();
			connctInputCanal.close();
			socket.close();
		} catch (IOException e) {
			return;
		}

		/*
		 * TODO Let client ask to close connection or opt to be a Spectator
		 */
	}

	private boolean treatConnectionInput() throws IOException {
		if (connctInputCanal.ready()) {
			inputLine = connctInputCanal.readLine();
			if (inputLine.startsWith(HardCodedValues.ConnectDisconnect.EXIT
					+ HardCodedValues.SEPARATOR)) {
				return false;
			} else if (inputLine.startsWith(HardCodedValues.ChatSetting.TALK
					+ HardCodedValues.SEPARATOR)) {
				String msg = inputLine
						.substring(HardCodedValues.ChatSetting.TALK.length());

				if (msg.length() == 2) {
					server.writeMyMessageToAllExceptMe(
							HardCodedValues.ChatSetting.LISTEN
									+ HardCodedValues.SEPARATOR + clientName
									+ HardCodedValues.SEPARATOR + "",
							connctOutCanal);
				} else {
					msg = msg.substring(1, msg.length() - 1);
					server.writeMyMessageToAllExceptMe(
							HardCodedValues.ChatSetting.LISTEN
									+ HardCodedValues.SEPARATOR + clientName
									+ HardCodedValues.SEPARATOR + msg,
							connctOutCanal);
				}
			} else if(inputLine.startsWith(HardCodedValues.MixingSetting.ACK)){
				
				//tickToSend+=(1.0f*server.getTempo()/HardCodedValues.DEFAULT_TEMPO);
			}
		}
		return true;
	}

	private void treatDataInput() throws IOException {
		if (dataInputCanal.ready()) {
			inputLine = dataInputCanal.readLine();

			if (!inputLine.startsWith(HardCodedValues.AudioSetting.CHUNK)) {
				// TODO Write Error message
				writeMessageToMe(HardCodedValues.AudioSetting.KO);
				return;
			}
			countKO = 0;
			String params = inputLine
					.substring(HardCodedValues.AudioSetting.CHUNK.length() + 1);
			int tickEndsAt = params.indexOf(HardCodedValues.SEPARATOR.charAt(0));

			if(tickEndsAt+1==params.length()) {
				writeMessageToMe(HardCodedValues.AudioSetting.KO);
				return;
			}
			
			String audioPart = params.substring(tickEndsAt + 1,
					params.length() - 1);
			
			byte[] audio = HardCodedValues.stringToByteArray(audioPart);
			myTick = Float.parseFloat(params.substring(0, tickEndsAt));
			server.mixAudioExceptMine(dataOutCanal, new AudioChunk(audio,
					myTick));
			writeMessageToMe(HardCodedValues.AudioSetting.OK);
		}
	}

	private void resetTimer() {
		start = System.currentTimeMillis();
		countKO = 0;
	}

	private void currentTime() {
		end = System.currentTimeMillis();
	}

	private void initialiseTimeVariables() {
		end = start = System.currentTimeMillis();
	}

	@Override
	public void run() {
		try {
			generateUniqueUserName();
			addMeToServer();
			welcomeMessage();

			sendAudioPort();

			waitForConnectionAndCreateDataCanal();

			addMyStreamsToServerAndInformOther();
			if(!canJoinSession()) {
				return;
			}
			initialiseTimeVariables();

			boolean tickSent = false;
			boolean connectionActive = true;
			while (connectionActive) {
				HardCodedValues.sleep(1);
				if (!canPlayJam()) {
					resetTimer();
				} else {
					if (!tickSent) {
						sendAudioSync();
						tickSent = true;
					}
					currentTime();
					if (!timer()) {
						break;
					}
					treatDataInput();
					sendMeBuffer();
					connectionActive = treatConnectionInput();
				}
			}
			InformAllOfClosedConnection();
		} catch (SocketException s) {
			informOtherOfClosedConnection();
		} catch (IOException io) {
			informOtherOfClosedConnection();
		} finally {
			removeMeFromServer();
		}
	}

	private void removeMeFromServer() {
		server.removeClient(this, connctOutCanal, dataOutCanal);
	}
	private boolean canJoinSession() throws IOException {
		if (server.isFirstClient(connctOutCanal)) {
			setStyleAndTempo();
		} else if (server.isFull()) {
			informMeServerIsFull();
			return false;
		} else {
			informeCurrentStyleAndTempo();
		}
		return true;
	}
	private void addMeToServer() {
		server.addClient(this);
		server.addConnectionStream(connctOutCanal);
	}

	private boolean canPlayJam() {
		synchronized (server) {
			return server.canPlayJam();
		}
	}

	private void InformAllOfClosedConnection() {
		try {
			synchronized (server) {
				server.writeConnectionMessageToAll(HardCodedValues.ConnectDisconnect.EXITED
						+ HardCodedValues.SEPARATOR + clientName);
			}
		} catch (IOException e) {
		}
	}

	private void informOtherOfClosedConnection() {
		try {
			synchronized (server) {
				server.writeMyMessageToAllExceptMe(
						HardCodedValues.ConnectDisconnect.EXITED
								+ HardCodedValues.SEPARATOR + clientName,
						connctOutCanal);
			}
		} catch (IOException e) {
			// TODO Error handling
		}
	}


	private void writeMessageToMe(String message) throws IOException {
		connctOutCanal.writeBytes(message + HardCodedValues.endLine);
	}

	public String getName() {
		return clientName;
	}

	private boolean canSend() {
		return (myTick - tickToSend) >= HardCodedValues.WAITING_TIME;
	}
	
	private boolean sendMeBuffer() throws IOException {
		if (canSend()) {
			boolean sent =server.sendToMeBuffAtTick(dataOutCanal, tickToSend);
			if(sent){
				tickToSend+= (1.0f*server.getTempo()/HardCodedValues.DEFAULT_TEMPO);
				return true;
			}
		} 
		return false;
	}
}
