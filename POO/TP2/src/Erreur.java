
// Une classe qui reprM-CM-)sente un point 2D qui bouge
class PointMobile
{
	int x =0;
	int y =0;
	int XMAX=100;
	int YMAX=100;
	public void afficher()
	{
		System.out.println("Je me trouve a :"+ x + ","+ y);
	}
	
	
	public void deplacer()
	{
		x += Math.random()*100;
		y += Math.random()*100;
		if (x>XMAX) {x -=XMAX;}
		if (y>YMAX) {y -=YMAX;}
		if (x<0) {x += XMAX;}
		if (y<0) {y += YMAX;}
	}
}

public class Erreur
{
	public static void main (String[] args)
	{
		System.out.println("Bonjour. Je ne suis plus plein d'erreurs!?");
		PointMobile point = new PointMobile();
		point.x = 50 ;
		point.y = 50 ;
		for (int i=0; i<100; i++)
		{
			point.deplacer();
			point.afficher();
		}
	}
}
