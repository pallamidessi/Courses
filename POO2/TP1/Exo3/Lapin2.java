package Exo3;

public class Lapin2{
	static int nbreLapin;
	static int nbreLapinVivant;
	private boolean vivant;

	private Lapin2(){
		if(nbreLapin < 50){
			vivant=true;
			nbreLapin++;
			nbreLapinVivant++;
		}else{
			vivant=false;
			nbreLapin++;
		} 
	}

	public Lapin2 creationLapin(){
		if(nbreLapin < 50){
			return new Lapin2();
		}else{
			return null;
		} 
	} 

	public void passeALaCasserole(){
		if(vivant)
			vivant=false;
	}
}
