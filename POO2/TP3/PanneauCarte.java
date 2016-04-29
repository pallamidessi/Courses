
public class PanneauCarte extends JPanel{
	private Carte aAfficher;
	private int tailleCarte;

	public PanneauCarte(Carte c){
		super();
		aAfficher=c;
	}

	public void paint(Graphics g){
		int i,j;

		tailleCarte=aAfficher.tailleCarte();
		
		setBackground(Color.white);

		for(i=0;i<tailleCarte;i++){
			for(j=0;j<tailleCarte;j++){
				if(aAfficher[i][j] instanceof Mur){
					g.setColor(Color.Black)
					fill(i*50,j*50,49,49);
				}
				else{
					g.setColor(Color.lightGray)
					fill(i*50,j*50,49,49);
				}
			
			}


		
		}
}
