public interface Annuaire extends java.rmi.Remote {
    String chercheNom(String nom) throws java.rmi.RemoteException;
    void putNumero(String name,String num) throws java.rmi.RemoteException;
    void removeNumero(String name) throws java.rmi.RemoteException;
    void listeNumero() throws java.rmi.RemoteException;
}
