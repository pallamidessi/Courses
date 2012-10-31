#include"arbint.h"


arbint lambda(){
	return NULL;
}



arbin e(arbint gauche,S x,arbint droit){
	arbin r=(arbin) malloc(sizeof(str_arbin));

	r->ag=gauche;
	r->ad=droit;
	r->etiquette=x;

	return r;
}


arbin ag(arbint a){
	return a->ag;
}


arbin ad(arbint a){
	return a->ad;
}



S racine(arbint a){
	return a->etiquette;
}


bool vide(arbint a){
	return (a==NULL);
}


		arbin insf(arbint a,S x){
			arbin racine=a;

			while(!v(a)){
				if(r(a)>x)
						a=ag(a)
				else 
						a=ad(a);
			}
			
			if(r(a)>x)
				a->ag=(lambda(),x,lambda());
			else
				a->ad=(lambda(),x,lambda());

			return racine;
		}


		S min(arbint a){
	
			while(!v(ag(a)))
				a=ag(a);

			return r(a);
		}

		S max(arbint a){
	
			while(!v(ad(a)))
				a=ad(a);

			return r(a);
}


		bool rech(arbint a,S x){
			arbin racine=a;

			while(!v(a)){
				if (x>r(a)){
					a=ad(a);
				}
				else 
				if (x<r(a)){
					a=ag(a);
				}
				else
					return a;
			}
			
			return lambda();
		}

		arbin otermin(arbint a){
			arbin racine=a;
			arbin tmp;

			while(!v(ag(ag(a))))
				a=ag(a);

			tmp=ad(ag(a));
			free(a->ag);
			a->ag=tmp;

		return racine;
		}
	
		arbin otermax(arbint a){
			arbin racine=a;
			arbin tmp;

			while(!v(ad(ad(a))))
				a=ad(a);

			tmp=ag(ad(a));
			free(a->ad);
			a->ad=tmp;

		return racine;
		}


	
		arbint sup(arbint a,S x){
			int chemin;
			arbin racine=a;
			arbin pere;

			while(x!=r(a)){
				if (x>r(a)){
					pere=a;
					a=ad(a);
					chemin=1;
				}
			else{
				pere=a;
				a=ag(a);
				chemin=0;
			}

			nouv_racine=r(eg(ad(a)));
			otermin(ad(a));

			if(chemin==0)
				pere->ag=e(ag(a),nouv_racine,ad(a));
			else
				pere->ad=e(ag(a),nouv_racine,ad(a));

			free(a);		

		return racine;
	}	
}


