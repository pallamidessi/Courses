class Evenement { 
    private Boolean Etat; // etat de l'evenement  
    private int nomevt; // numero identifiant l'evt

    public Evenement() { 
	Etat = Boolean.FALSE; 
    } 
    public synchronized void set(int nomevt) { 
	Etat = Boolean.TRUE;  // debloque les threads qui 
	this.nomevt = nomevt; 	//attendent cet evenement:  
	notifyAll(); 
    } 
    public synchronized void reset() { 
	Etat = Boolean.FALSE; 
    } 
    public synchronized void attente() { 
	if(Etat==Boolean.FALSE) { 
	    try { 
		wait(); // bloque jusqu'a un notify()  
	    } catch(InterruptedException e) {}; 
	} 
	System.err.println(Thread.currentThread().getName()+" recoit un l'evt "+nomevt);
    } 
}

class EvtGenerator extends Thread {
    private Evenement e;
    private boolean conf;

    EvtGenerator (Evenement e, boolean conf) {
	this.e = e;
	this.conf = conf;
    }

    public void run() {
	int i;
	if (conf) 
	    while (true) {
		i=(int)(Math.random() * 1000);
		try {
		    Thread.sleep(i); // ms
		} catch (InterruptedException e) {}
		System.err.println(Thread.currentThread().getName()+" genere l'evt "+i);
		e.set(i);
		//		Thread.yield();
		e.reset();
	    }
	else 
	    while (true) 
		e.attente();
    }
    
    public static void main(String args[]) throws InterruptedException {
	Evenement e = new Evenement();
	Thread T1 = new EvtGenerator(e,true);
	Thread T2 = new EvtGenerator(e,false);
	Thread T3 = new EvtGenerator(e,false);
	T1.start();
	T2.start();
	T3.start();
    }

    
}

