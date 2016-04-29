import java.lang.Math.*;

public class Carte{
	private final static int tailleCarte=8;
	private Case[][] grille=new Case[tailleCarte][tailleCarte];


	public Carte(){
		int i,j;

		for(i=0;i<tailleCarte;i++){
			for(j=0;j<tailleCarte;j++){
				if(Math.random()>0.75)
					grille[i][j]=new Mur();
				else
					grille[i][j]=new Terrain();
			}
		}
	}

	public int tailleCarte(){
		return tailleCarte;
	}

}
