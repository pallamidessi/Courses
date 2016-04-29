

public class TestChaines {
	public static void main(String[] args){
		//String s1=new String ("abcd");
		//String s2=new String ("abcd");
		//String s1=new String ("abcd");
		//String s2=new String ("AbcD");
		//String s1=args[0];
		//String s2=args[1];
	
		/*
		System.out.println(s1 + "longueur :" + s1.length());
		System.out.println(s1 + s2);
		
		System.out.println("s1==s2	"+						s1.equals(s2));
		System.out.println("s1.compareTo(s2)	"+				s1.compareTo(s2));
		System.out.println("s1.compareToIgnoreCase(s2)	"+	s1.compareToIgnoreCase(s2));
		
		
		if (args[0].charAt(0)==args[1].charAt(0))
			System.out.println("ces deux chaine commencent par le meme character");
		else
			System.out.println("ces deux chaine ne commencent pas par le meme character");
		
		if(s1.startsWith(s2))
			System.out.println("s1 commence par la chaine s2");
		else
			System.out.println("s1 ne commence pas par la chaine s2");
		
		if(s1.endsWith(s2))
			System.out.println("s1 fini par la chaine s2");
		else
				System.out.println("s1 ne fini pas par la chaine s2");
		
		if(s1.contains(s2))
			System.out.println("s1 contient  la chaine s2");
		else
				System.out.println("s1 ne contient pas par la chaine s2");
		
		s1+=s2;
		System.out.println(s1);
		
		s1=s1.concat(s2);
		System.out.println(s1);
		
		System.out.println(s1.replace('o', 'O'));
		
		
		if(s1.contains(s2))
			System.out.println(s1.substring(s1.indexOf(s2)));
		else
			System.out.println(s1);
		
		
		
		System.out.println(s1.toUpperCase());
		
		System.out.println(s1.trim());
		
		
		
		String[] tab=s1.split(" ");
		
		int i;
		
		for(i=0;i<tab.length;i++)
			System.out.println(tab[i]);
		
		
		
		s1+='A';
		System.out.println(s1);
		
		s1=s1.concat("A");
		System.out.println(s1);
		*/
		
		StringBuffer test=new StringBuffer();
		StringBuffer test2=new StringBuffer(15);
		StringBuffer test3=new StringBuffer("test");
		
		test3.append('c');
		test3.insert(2, 'c');
		test3.setCharAt(0, 'm');
		
		
		
		
	}
}
