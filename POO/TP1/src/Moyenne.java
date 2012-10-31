
public class Moyenne {
	public static void main(String[] args) {
		int moyenne=0,i=0;
		int tmp=0;
		
		if (args.length == 0 ) {
			System.out.println("Nombre d'arguments incorrect ! ");
			}
		else {
			for(i=0;i < args.length;i++){
				tmp= Integer.parseInt(args[i]);
				moyenne+=tmp;
			}
		}
		moyenne /=args.length;
		System.out.println(moyenne);
	}

}
