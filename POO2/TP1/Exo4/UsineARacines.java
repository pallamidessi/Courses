/**
 *  Définition de la classe UsineARacines.
 */

/**
 *  Crée une instance de Racines.
 */
public class UsineARacines
{
  /**
    *  Crée une instance de Racines.
    *  @param a    Le coefficient en X².
    *  @param b    Le coefficient en X.
    *  @param c    Le coefficient constant.
  */
  public static Racines createRacines(double a, double b, double c)
  {
    double dis = b*b - 4.0 * a*c ;
    if (dis > 0.0)
    {
      return new RacineSol2(a, b, c, dis) ;
    }
    else if (dis < 0.0)
    {
      return new RacineSol0(a, b, c, dis) ;
    }
    else 
    {
      return new RacineSol1(a, b, c, dis) ;
    }
  }
}
