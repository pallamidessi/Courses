class Point2 {
	private int x ;
	private int y ;
	
	public Point2(int x0, int y0) {
		x=x0 ;
		y=y0;
	}
	
	public void affiche(){
		System.out.println("x :" + x);
		System.out.println("y :" + y);
	}
}

public class CreationPoint2 {
	public static void main (String [] args) {
		System.out.println("Bonjour !") ;
		Point2 point1 = new Point2(0,0) ;
		Point2 point2 = new Point2(3,4) ;
		Point2 point3 = new Point2(7,1);
		
		point1.affiche();
		point2.affiche();
		point3.affiche();
	}
}