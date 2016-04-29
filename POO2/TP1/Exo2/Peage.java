package Exo2;

public class Peage {
	private int nbreVehicule=0;
	private int totalCaisse=0;
	
	public Peage(){
	}
	
	public void passage(Vehicule v){
		if((v.getType().compareTo("voiture"))==0){
			totalCaisse+=4;
			nbreVehicule+=1;
		}else if ((v.getType().compareTo("camion"))==0){
			Camion c=(Camion) v;
			nbreVehicule+=1;
			totalCaisse+=c.getNbreEssieux()*7 + c.getPoidsTotal()*15;
		}
	}

}
