package Exo2;

public class Camion extends Vehicule{
	private int nbreEssieux;
	private int poidsTotal;
	
	public Camion(){
		super();
	}
	
	public Camion(int essieux,int poids){
		super();
		nbreEssieux=essieux;
		poidsTotal=poids;
	}
	
	public int getNbreEssieux(){
		return nbreEssieux;
	}
	
	public int getPoidsTotal(){
		return poidsTotal;
	}
	
	public String getType(){
		return "camion";
	}

}
