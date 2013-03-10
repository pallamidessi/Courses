
public class Mammifere extends Animal{
	
	public Mammifere(){
		super();
	} 
	
	public Mammifere(String newName){
		super();
		name=newName.substring(0,sizeName);
	}
	
	public String getPresentation(){
		if(name!=null)
			return super.getPresentation()+"de nom"+name+".Je suis un mammifere.";
		else 
			return super.getPresentation()+"sans nom.Je suis un mammifere.";
	}
}
