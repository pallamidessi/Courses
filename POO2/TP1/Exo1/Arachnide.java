
public class Arachnide extends Animal{
	public Arachnide(){
		super();
	} 
	
	public Arachnide(String newName){
		super();
		name=newName.substring(0,sizeName);
	}
	
	public String getPresentation(){
		if(name!=null)
			return super.getPresentation()+"de nom"+ name +".Je suis un Arachnide.";
		else 
			return super.getPresentation()+"sans nom.Je suis un Arachnide.";
	}
}
