package Tools;

public class AudioChunk {
	private byte[] audioBuff;
	private float tick;
	
public AudioChunk(byte[] audio, float myTick) {
	this.setAudioBuff(audio);
	this.setTick(myTick);
}

public byte[] getAudioBuff() {
	return audioBuff;
}

public void setAudioBuff(byte[] audioBuff) {
	this.audioBuff = audioBuff;
}

public float getTick() {
	return tick;
}

public void setTick(float tick) {
	this.tick = tick;
}	

}
