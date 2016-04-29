class ZonePartage extends Thread {
  int nLecteurs;
  int nEcrivainsEnAttente;
  Object accesslock;

  ZonePartage() {
    accesslock = new Object();
  }

  public void accesPartage() {
    synchronized(accesslock) {
      while (nEcrivainsEnAttente>0) {
        try{accesslock.wait();}
        catch(InterruptedException e){}
      }
      nLecteurs++;
    }
  }

  public void retourPartage() {
    synchronized(accesslock) {
      nLecteurs--;
      if (nLecteurs==0) {
        accesslock.notifyAll();
      }
    }
  }

  public void accesExclusif() {
    synchronized(accesslock) {
      nEcrivainsEnAttente++;
      if (nLecteurs>0) {
        try{accesslock.wait();}
        catch(InterruptedException e){}
      }
    }
  }

  public void retourExclusif() {
    synchronized(accesslock) {
      nEcrivainsEnAttente--;
      if(nEcrivainsEnAttente==0){
        accesslock.notifyAll();
      }
    }
  }
}    

class Reader extends Thread {
  ZonePartage z;
  Reader (ZonePartage z, String name) {
    super(name);
    this.z = z;
  }
  public void run() {
    while (true) {
      lecture();
    }
  }
  public void lecture() {
    z.accesPartage();
    System.out.println("Lecture");
    try{Thread.sleep(300);} catch(Exception e){}
    z.retourPartage();
  }

}

class Writer extends Thread {
  ZonePartage z;
  Writer (ZonePartage z, String name) {
    super(name);
    this.z = z;
  }
  public void run() {
    while (true) {
      ecriture();
    }
  }
  public void ecriture() {
    z.accesExclusif();
    System.out.println("Ecriture");
    z.retourExclusif();
    try{Thread.sleep(500);} catch(Exception e){}
  }
}

public class LectEcrivain {
  public static void main(String args[]) {
    ZonePartage z = new ZonePartage();
    Thread r1 = new Reader(z,"r1");
    Thread r2 = new Reader(z,"r2");
    Thread r3 = new Reader(z,"r3");
    Thread r4 = new Reader(z,"r4");
    Thread w1 = new Writer(z,"w1");
    Thread w2 = new Writer(z,"w2");
    r1.start();
    r2.start();
    r3.start();
    r4.start();
    w1.start();
    w2.start();
  }
}
