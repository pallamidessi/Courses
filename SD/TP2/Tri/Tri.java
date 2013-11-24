public class Tri extends Thread{
  int t[];
  int m;
  int n;

  public Tri(int[] t,int m,int n){
    this.t=t;
    this.m=m;
    this.n=n;
  }

  static void echangerElements(int[] t, int m, int n) {
    int temp = t[m];
    t[m] = t[n];
    t[n] = temp;
  }

  static int partition(int[] t, int m, int n) {
    int v = t[m];                 // valeur pivot
    int i = m-1;
    int j = n+1;                  // indice final du pivot

    while (true) {
      do {
        j--;
      } while (t[j] > v);
      do {
        i++;
      } while (t[i] < v);
      if (i<j) {
        echangerElements(t, i, j);
      } else {
        return j;
      }
    }
  }

  public void run() {
    if (m<n) {
      int p = partition(t, m, n);
      Thread t1=new Tri(t,m,p);
      Thread t2=new Tri(t,p+1,n);

      t1.start();
      t2.start();
      try{
        t1.join();
        t2.join();
      }
      catch(InterruptedException e){}
    }
  }

  public static void main(String args[]) {
    int entree[] = {9, 8, 3, 20, 4 , 0, 19, 24, 5, 7 };
    Tri t=new Tri(entree,0,entree.length-1);
    t.start();
    try{t.join();}catch(InterruptedException e){}
    for (int i=0; i<entree.length; i++) System.out.println(entree[i]);
  }

}
