
public class Animal {
	protected String name=null;
	protected int sizeName;
	
	public Animal(){	
	}
	
	public Animal(int size){
		sizeName=size;
	}
	
	public String getPresentation(){
		return "Je suis un animal ";
	}
}
