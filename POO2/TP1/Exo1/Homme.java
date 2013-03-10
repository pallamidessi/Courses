
public class Homme extends Mammifere{
	
	public Homme(){
		super();
	}
	
	public Homme(String newName){
		super(newName);
	}
	
	public String getPresentation(){
		return super.getPresentation()+"je suis un Homme";
	}
}
