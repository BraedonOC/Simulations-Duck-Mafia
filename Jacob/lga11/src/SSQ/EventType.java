package SSQ;

public enum EventType {
	JOB_ARRIVAL("ARRIVAL"),
	FEEDBACK_ARRIVAL("FEEDBACK"),
	JOB_COMPLETION("COMPLETE");
	
	private final String text;
	
	
	EventType(final String text) {
		this.text = text;
	}
	
	@Override
	public String toString() {
		return text;
	}
}
