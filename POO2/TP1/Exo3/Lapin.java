package Exo3;

public class Lapin {
	static int nbreLapin;
	static int nbreLapinVivant;
	private boolean vivant;
	
	public Lapin(){
		if(nbreLapin < 50){
			vivant=true;
			nbreLapin++;
			nbreLapinVivant++;
		}else{
			vivant=false;
			nbreLapin++;
		} 
	}
	
	public void passeALaCasserole(){
		if(vivant)
			vivant=false;
	}
}
