import java.io.*;
import org.omg.CORBA.*;
import OpMatricePackage.*;



public class Client {
  public static void print_matrice(int[][] m){
    int dim1=m.length;
    int dim2=m[0].length;
    int i,j;

    for (i = 0; i < dim1; i++) {
      for (j = 0; j < dim2; j++) {
        System.out.print(m[i][j]+" ");
      }
      System.out.println();
    }

  }

  public static void main( String args[] )  {
    int nombre;
    int res;
    if( args.length < 1 ) {
      System.out.println( "Usage: java Client <ior> " );
      System.exit( 1 );
    }

    try {
      // initialiser l'ORB.
      //org.omg.CORBA.IntHolder intH = new org.omg.CORBA.IntHolder(); 
      ORB orb = ORB.init( args, null );
      System.out.println( "0) ORB initialise'");

      //read the ior from the command-line
      String ior = args[0];
      System.out.println( "1) IOR lue : " + ior );


      org.omg.CORBA.Object obj = orb.string_to_object(args[0]);

      System.out.println("2) Reference obtenue a partir de l'IOR");
      OpMatrice service = OpMatriceHelper.narrow(obj);


      int[][] a = { {1, 0, 0}, {0, 2, 0}, {0, 0, 3}, {0, 0, 4} };
      int[][] b = { {1, 2, 3}, {1, 2, 3}, {1, 2, 3} };

      System.out.println("Soit la matrice A:");
      Client.print_matrice(a);
      System.out.println("Soit la matrice B:");
      Client.print_matrice(b);

      int[][] result= service.multiplicationMatrice(a,b);

      System.out.println("Le résultat de AB:");
      print_matrice(result);
    }
    catch( org.omg.CORBA.SystemException ex ) {
      System.err.println( "Erreur !!" );
      ex.printStackTrace();
    }
    catch(illegalMatriceMultiplication e){ 
      System.err.println( "Erreur !! Taille de matrice non conforme !" );
    }
  }
}
