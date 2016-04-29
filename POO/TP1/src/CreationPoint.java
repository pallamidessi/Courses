class Point {
	public int x ;
	public int y ;
	
	public Point(int x0, int y0) {
		x=x0 ;
		y=y0;
	}
	
	public void affiche(){
		System.out.println("x :" + x);
		System.out.println("y :" + y);
	}
}

public class CreationPoint {
	public static void main (String [] args) {
		System.out.println("Bonjour !") ;
		Point point1 = new Point(0,0) ;
		Point point2 = new Point(3,4) ;
		Point point3 = new Point(7,1);
		
		point1.affiche();
		point2.affiche();
		point3.affiche();
	}
}