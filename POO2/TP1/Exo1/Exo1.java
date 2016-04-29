public class Exo1 {
		public static void main ( String [] args )
		{
		Animal [] animaux = new Animal [6] ;
		animaux [0] = new Chien (" Ziggy ") ;
		animaux [1] = new Homme (" David ") ;
		animaux [2] = new Araignee () ;
		animaux [3] = new Scorpion () ;
		animaux [4] = new Chien () ;
		animaux [5] = new Araignee (" Bowie ") ;
		for ( int i = 0; i < 6; i++)
		{
		System .out. println ( animaux [i]. getPresentation ()) ;
		}
	}

}
