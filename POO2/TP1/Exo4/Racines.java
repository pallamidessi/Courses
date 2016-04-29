import java.lang.Math.* ;

/**
 *  Définition de la classe Racines.
 */

/**
 *  Classe de base de toutes les racines.
 */
public abstract class Racines
{
  /// Coefficients ax^2+bx+c
  private double m_a, m_b, m_c ;
        
  /// Discriminant.
  private double m_dis ;

  /**
    *  Constructeur d'une racine.
    *  @param  a   Premier coefficient.
    *  @param  b   Second coefficient.
    *  @param  c   Troisième coefficient.
    *  @param  dis Discriminant.
    */
  protected Racines(double a, double b, double c, double dis)
  {
    m_a = a ;
    m_b = b ;
    m_c = c ;
    m_dis = dis ;
  } 
        
  /**
    *  Get un coefficient de l'équation.
    *  @param  i   0, 1 ou 2, si on veut le terme constant, en X, ou en X2.
    *  @return Le coefficient souhaité.
    */
    public double getCoeff(int i)
    {
      switch (i)
      {
        case 0 : 
          return m_c ;
        case 1 : 
          return m_b ;
        case 2 : 
          return m_a ;
      }
      return 0.0 ;
    }

		public double getDiscr(){
			return m_dis;
		}
		
    /**
      *  Renvoie le nombre de racines.
      *  @return Le nombre de racines.
      */
    public abstract int nbRacines() ;

    /**
      *  Renvoie la valeur d'une racine.
      *  @param  i   1 ou 2 (première ou deuxième racine).
      *  @return La valeur de la racine.
      */
    public abstract double valeurRacine(int i) throws Exception ;
}
