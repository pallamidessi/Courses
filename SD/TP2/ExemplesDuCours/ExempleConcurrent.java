public class ExempleConcurrent extends Thread {

    private static int compte = 0;

    public void run() {
	int tmp = compte;
	for (int i=0; i<1000000; i++)
	tmp = tmp + 1;
	compte = tmp;
    }

    public static void main(String args[]) throws InterruptedException {
	final int nbt = 6;
	Thread Ts[] = new Thread[nbt];
	for (int i=0; i< nbt; i++) { Ts[i] = new ExempleConcurrent(); }
	for (int i=0; i< nbt; i++) { (Ts[i]).start(); Thread.yield();}
	for (int i=0; i< nbt; i++) { (Ts[i]).join(); }
        System.out.println( "compteur=" + compte );
    }
}
