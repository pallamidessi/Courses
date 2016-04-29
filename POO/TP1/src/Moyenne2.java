
public class Moyenne2 {
	static int min(int a, int b){
	if (a > b )
		return b;
	else
		return a;
	}
	
	static int max(int a, int b){
	if (a > b )
		return a;
	else
		return b;
	}
	
	public static void main(String[] args) {
		Integer moyenne=new Integer(0);
		int i=0;
		
		if (args.length == 0 ) {
			System.out.println("Nombre d'arguments incorrect ! ");
			}
		else {
			for(i=0;i < args.length;i++){
				Integer tmp=new Integer(args[i]);
				moyenne+=tmp;
			}
		}
		moyenne /=args.length;
		System.out.println(moyenne);
	}

}
