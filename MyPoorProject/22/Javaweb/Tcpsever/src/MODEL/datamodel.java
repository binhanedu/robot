package MODEL;

public class datamodel {
	public String temperature;
	public String Time;
	public String humidity;
	
	@Override
	public String toString() {
		return "datamodel [temperature=" + temperature + ", humidity=" + humidity + "]";
	}
	
	public String getTemperature() {
		return temperature;
	}
	public void setTemperature(String temperature) {
		this.temperature = temperature;
	}
	public String getHumidity() {
		return humidity;
	}
	public void setHumidity(String humidity) {
		this.humidity = humidity;
	}

	public String getTime() {
		return Time;
	}

	public void setTime(String time) {
		Time = time;
	}
	
}
