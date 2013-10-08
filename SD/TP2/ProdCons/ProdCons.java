class Entrepot { 
    private int entrepot[];
    private int curseur;
    
		Entrepot (int n) {
			entrepot = new int[n];
			curseur = 0;
    }
		
		int getsize() {
			return (entrepot.length);
    }
    
		synchronized public int get(){ 
			if(curseur==0)
				return -1;
				
			System.out.println(Thread.currentThread().getName()+": getit "+entrepot[curseur]);
			curseur--;
			return(entrepot[curseur]);
			}

		synchronized public void put(int value){
			if (curseur==getsize()){
				System.out.println("ouchi,entrepot plein!\n");
				return;
			}
			else{
				curseur++;
				entrepot[curseur]=value;
				System.out.println(Thread.currentThread().getName()+": putit "+value);
				notifyAll();
			}
    }
}

class Prod extends Thread {
    Entrepot e;
    
		Prod ( Entrepot e, String name ) {
			super(name);
        this.e = e;
    }

    public void run() {
			while (true) {
					try {
				Thread.sleep((int)(Math.random() * 100)); // ms
					} catch (InterruptedException e) {
				System.out.println("ouch!\n");
					}
				e.put((int)(Math.random() * 100));
			}
    }
}

class Cons extends Thread {
    Entrepot e;
    
		Cons ( Entrepot e, String name ) {
			super(name);
        this.e = e;
    }

    public synchronized void run() {
			int value;

			while (true) {
				try {
					Thread.sleep((int)(Math.random() * 100)); // ms
				} catch (InterruptedException e) {
					System.out.println("ouch!\n");
				}

				if((value=e.get())==0)
					try{
						wait();
					} catch (InterruptedException e){
						System.out.println("Attend la production");
					}
				else{
					System.out.println("Consomme "+value);
				}
			}
    }
}

class ProdCons {
    public static void main(String args[]) {
			Entrepot e1 = new Entrepot(100);
			Prod p1 = new Prod(e1, "prod1");
			Prod p2 = new Prod(e1, "prod2");
			Cons c1 = new Cons(e1, "cons1");
			System.out.println("Capacite' entrepot "+e1.getsize());
			p1.start();
			p2.start();
			c1.start();
    }
}
