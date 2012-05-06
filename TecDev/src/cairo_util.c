#include<cairo_util.h>


void Affiche_jeu(cairo_surface_t *surface,grille l,grille Ligne,grille Col,int decalX,int
decalY,int Sx,int Sy,int bouton){
	cairo_t* mask;
  mask=cairo_create(surface);
	int resetX=decalX,resetY=decalY;
	int i=0,v=0,j=0,k=0;
	char val[4];
	int originX=decalX*20+25;


	cairo_set_source_rgb(mask,1 ,1 ,1); 
  cairo_paint(mask);

	/*affiche les carre de selection*/
	carreSelection(mask,Sx,Sy,l,decalX,decalY,bouton);

	/*tracer le cadre */ 
	tracer_grille(mask,l,decalX,decalY);	

	//affiche les valeurs de la grille
	decalX=resetX;
	decalY=resetY;

  cairo_set_source_rgb(mask,0.1,0.1,0.1);
	cairo_select_font_face(mask, "sans-serif", CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(mask, 13);
	
	decalY=decalY*20+25;

		for(i=0;i<l->N;i++){
			decalX=originX;
    	for(v=0;v<l->M;v++){
				if (l->matrice[i][v]=='.'){
      		cairo_move_to(mask,decalX+v*50,decalY+i*50);
					cairo_show_text(mask,".");
				}
				else {
      		cairo_move_to(mask,decalX+v*50-3,decalY+i*50-3);
					cairo_show_text(mask,"+");
				}
				decalX++;
			}
    decalY++;
		}



	/*affiche les sommes des suites sur les lignes*/
	decalX=resetX;
	decalY=resetY;

	decalY=decalY*20+25;

  cairo_set_source_rgb(mask,0.1,0.1,0.1);
	cairo_select_font_face(mask, "sans-serif", CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(mask, 13);

	for(i=0;i<Ligne->N;i++){
   	for(v=0;v<Ligne->M;v++){
	  	if (Ligne->matrice[i][v]!=0){
					sprintf(val,"%d",Ligne->matrice[i][v]);
      		cairo_move_to(mask,v*15,decalY+i*50);
					cairo_show_text(mask,val);
			}
		}
	}


 
	/*affiche les sommes des suites sur les colonnes*/

	cairo_set_source_rgb(mask,0.1,0.1,0.1);
	cairo_select_font_face(mask, "sans-serif", CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(mask, 13);
  
	
  decalX=resetX;
	decalY=resetY;

	decalX=decalX*20+25;

		for(j=0;j<Col->N;j++){
			for(k=0;k<Col->M;k++){
		  	if (Col->matrice[j][k]!=0){
							sprintf(val,"%d",Col->matrice[j][k]);
  	    			cairo_move_to(mask,(decalX+k*50),10+j*15);
							cairo_show_text(mask,val);
				}
			}
		}	

 cairo_destroy(mask);
}

void tracer_grille(cairo_t* mask,grille l,int decalX,int decalY){
int i=0;

	cairo_set_source_rgb(mask,0.0,0.0,0.0); 
	cairo_set_line_width (mask, 1.0);
	
	/*tracages des lignes horizontals*/

		for ( i=0;i<=l->N;i++){
			cairo_move_to(mask,decalX*20,decalY*20+i*50);
		  cairo_line_to(mask,decalX*20+l->M*50, decalY*20+i*50);
			cairo_stroke(mask);
		}
																				   
	/*tracages des lignes verticals*/

		for ( i=0;i<=l->M;i++){
			cairo_move_to(mask,decalX*20+i*50,decalY*20);
		  cairo_line_to(mask,decalX*20+i*50, decalY*20+l->N*50);
			cairo_stroke(mask);
		}
}


 //carre de selection
void carreSelection(cairo_t* mask,int Sx,int Sy,grille l,int decalX,int decalY,int bouton){

cairo_set_source_rgba (mask,1,0.1,0.1,0.5);
/*affiche un carre de selection d'apres Sx et Sy*/
if (bouton==0){
	if (Sx>decalX*20 && Sx<decalX*20+50*l->M &&
			Sy>decalY*20 && Sy<decalY*20+50*l->N){
				cairo_rectangle(mask,((Sx-decalX*20)/50)*50+decalX*20,((Sy-decalY*20)/50)*50+decalY*20,50,50);
				cairo_fill(mask);
	}
}
else
	/*affiche un carre de selection d'apres Sx et Sy,et modifie la matrice d'apres Sx et Sy*/
	if (bouton==1){
		if (Sx>decalX*20 && Sx<decalX*20+50*l->M &&
				Sy>decalY*20 && Sy<decalY*20+50*l->N){
					cairo_rectangle(mask,((Sx-decalX*20)/50)*50+decalX*20,((Sy-decalY*20)/50)*50+decalY*20,50,50);
					cairo_fill(mask);
					if (l->matrice[(Sy-decalY*20)/50][(Sx-decalX*20)/50]=='.')						
							l->matrice[(Sy-decalY*20)/50][(Sx-decalX*20)/50]='+';
					else 
							l->matrice[(Sy-decalY*20)/50][(Sx-decalX*20)/50]='.';
 		}
	}
}


grille menu(cairo_surface_t *surface,XEvent e,Display* dpy,KeySym keysym){
	int i=0,n=0,m=0;
	char buffer[100];
	char* nom;
	int choix=0,deja=0,swt=0;
	grille l;

nom=(char*) malloc (15*sizeof(char));
cairo_t* mask=cairo_create(surface);

	cairo_set_source_rgb(mask,1,1,1);
	cairo_paint(mask);
while(choix==0){

	cairo_set_source_rgb(mask,0.1,0.1,0.1);
	cairo_select_font_face(mask,"sans-serif",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(mask,13);

/*1er Choix : Image ou fichier */

	if(deja==0){
		cairo_move_to(mask,10,20);
		cairo_show_text(mask,"F:charger un fichier");
		cairo_move_to(mask,10,35);
		cairo_show_text(mask,"I:charger une image");
		deja=1;
	}

XNextEvent(dpy,&e);
if (e.type==KeyPress && XLookupString(&e,buffer,100,keysym,0)==1){
	if (buffer[0]=='f'){
		cairo_set_source_rgb(mask,1,1,1);
		cairo_paint(mask);

		cairo_set_source_rgb(mask,0.1,0.1,0.1);
		cairo_move_to(mask,10,20);
		cairo_show_text(mask,"Nom du fichier(defaut : toto.txt)");

		/*recupere le nom du fichier char par char et l'affiche au furs et a mesure pour l'utilisateur*/

		while(buffer[0]!='\r'){
			XNextEvent(dpy,&e);
			if (e.type==KeyPress && XLookupString(&e,buffer,100,keysym,0)==1){
				nom[i]=buffer[0];
				cairo_move_to(mask,10+(i*6),35);
				cairo_show_text(mask,(char*)&buffer[0]);
				i++;
			}
		}

		nom[i-1]='\0';				 //fini le string
		cairo_move_to(mask,30,40);
		cairo_show_text(mask,nom);
		/*on clear le fenetre*/

		cairo_set_source_rgb(mask,1,1,1);
		cairo_paint(mask);

		cairo_set_source_rgb(mask,0.1,0.1,0.1);
		cairo_move_to(mask,10,20);
		cairo_show_text(mask,"Taille du fichier (largeur hauteur)(maximun 9*9)");

		buffer[0]=48;
//			XNextEvent(dpy,&e);
		/*recupere les taille du fichier char par char et l'affiche au furs et a mesure pour l'utilisateur*/	
		/*On utilise un espace comme separateur entre les deux donnees*/
		cairo_set_source_rgb(mask,0.1,0.1,0.1);
		i=0;
		while(buffer[0]!='\r'){
			XNextEvent(dpy,&e);
			if (e.type==KeyPress && XLookupString(&e,buffer,100,keysym,0)==1){
				if(swt==0){
					if (buffer[0]==' ')
						swt=1;                          //on transforme les entrees en un seul int 
					else{
						n*=10;
						n+=buffer[0]-48;
						cairo_move_to(mask,10+i*7,35);
						cairo_show_text(mask,(char*)&buffer[0]);
					}
				} 
				else{
					if(buffer[0]==' ')
							buffer[0]=48;
						else 
							if(buffer[0]=='\r')
								break ;
							else
							{
							m*=10;
							m+=buffer[0]-48; 						   //on transforme les entrees en un seul int 
							cairo_move_to(mask,25+i*7,35);
							cairo_show_text(mask,(char*)&buffer[0]);
						}
					}
				}
			i++;
			}

		l=alloue_grille(n,m);
		
		l=charger_grille(l, nom, "r");
	  choix=1;

		//if (l==NULL)
		//	exit(1);
	}
 else 
	if (buffer[0]=='i'){
		cairo_set_source_rgb(mask,1,1,1);
		cairo_paint(mask);

		cairo_move_to(mask,10,20);
		cairo_show_text(mask,"Nom du fichier(defaut : couleur.ppm)");
		
		/*recupere le nom du fichier char par char et l'affiche au furs et a mesure pour l'utilisateur*/
		cairo_set_source_rgb(mask,0.1,0.1,0.1);
		while(buffer[0]!='\r'){
			XNextEvent(dpy,&e);
			if (e.type==KeyPress && XLookupString(&e,buffer,100,keysym,0)==1){
				nom[i]=buffer[0];
				cairo_move_to(mask,10+i,21);
				cairo_show_text(mask,(char*)&buffer[0]);
				i++;
			}
		}

    
		l=charger_image(nom,"r",Seuil(nom));

		choix=1;
		}
	}	
}
return l;
}
