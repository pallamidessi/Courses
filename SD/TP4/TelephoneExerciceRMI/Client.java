import java.io.*;
import org.omg.CORBA.*;
import java.awt.*; 
import java.applet.Applet;
import java.awt.event.*;
import java.net.InetAddress.* ;
import java.net.* ;

public class Client 
extends Applet
implements ActionListener {

  TextField texte = new TextField("",10);
  Label intitule = new Label("Nom a rechercher : ");
  Button bouton = new Button("Envoie");
  Annuaire annuaire;

  public void init() {
    add(intitule);
    add(texte);
    add(bouton);
    bouton.addActionListener(this);
    String ior;
    try {
      String hostname = super.getParameter("serveur");
      // initialiser l'ORB.
      //org.omg.CORBA.IntHolder intH = new org.omg.CORBA.IntHolder(); 
      ORB orb = ORB.init( new String[0], null );
      System.out.println( "0) ORB initialise'");

      //read the ior from a file
      FileReader file = new FileReader(iorfile.value) ;
      BufferedReader in = new BufferedReader(file) ;
      ior = in.readLine() ;
      file.close() ;
      System.out.println( "1) IOR lue : " + ior );

      System.out.println("2) Reference obtenue a partir de l'IOR");
      org.omg.CORBA.Object obj = orb.string_to_object(ior) ;

      Annuaire annuaire = AnnuaireHelper.narrow(obj);

    } catch (Exception e) {
      System.err.println(e);
      e.printStackTrace();
    }
  }

  public void actionPerformed(ActionEvent ae) {
    try {
      StringHolder message=new StringHolder();
      annuaire.chercheNom(texte.getText(),message);
      intitule.setText(message.value);
    } catch (Exception e) {
      System.out.println(e.getMessage());
      e.printStackTrace();
    }
  }
}
