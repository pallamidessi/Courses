public class ExemplePartage extends Thread {

    private static String chaineCommune = "Rien";
    private String nom;

    ExemplePartage ( String s ) {
        nom = s;
    }

    public void run() {
        chaineCommune = chaineCommune + nom;
    }

    public static void main(String args[])  throws InterruptedException{
        Thread T1 = new ExemplePartage( "T1" );
        Thread T2 = new ExemplePartage( "T2" );
        T1.start();
        T2.start();
	T1.join();
	T2.join();
        System.err.println( chaineCommune );
    }
}
