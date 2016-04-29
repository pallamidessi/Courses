
public class Chien extends Mammifere{
	
	public Chien(){
		super();
	}
	
	public Chien(String newName){
		super(newName);
	}
	
	public String getPresentation(){
		return super.getPresentation()+"je suis un Chien";
	}
}
