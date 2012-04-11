#include<cairo_util.h>


void Affiche_jeu(cairo_surface_t *surface,grille l,grille Ligne,grille Col,int decalX,int
decalY,int Sx,int Sy,int bouton){
	cairo_t* mask;
  mask=cairo_create(surface);
	int resetX=decalX,resetY=decalY;
	int i=0,v=0,j=0,k=0;
	char val[4];
	int originX=decalX*20+25;


//tracer le cadre  
	cairo_set_source_rgb(mask,1 ,1 ,1); 
  cairo_paint(mask);
carreSelection(mask,Sx,Sy,l,decalX,decalY,bouton);
	tracer_grille(mask,l,decalX,decalY);	

	//affiche_grille
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



	//afficheCountLigne
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


	//afficheCountColonne
 

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

		for ( i=0;i<=l->N;i++){
			cairo_move_to(mask,decalX*20,decalY*20+i*50);
		  cairo_line_to(mask,decalX*20+l->M*50, decalY*20+i*50);
			cairo_stroke(mask);
		}
																				   
		for ( i=0;i<=l->M;i++){
			cairo_move_to(mask,decalX*20+i*50,decalY*20);
		  cairo_line_to(mask,decalX*20+i*50, decalY*20+l->N*50);
			cairo_stroke(mask);
		}
}


 //carre de selection
void carreSelection(cairo_t* mask,int Sx,int Sy,grille l,int decalX,int decalY,int bouton){

cairo_set_source_rgba (mask,1,0.1,0.1,0.5);

if (bouton==0){
	if (Sx>decalX*20 && Sx<decalX*20+50*l->M &&
			Sy>decalY*20 && Sy<decalY*20+50*l->N){
				cairo_rectangle(mask,((Sx-decalX*20)/50)*50+decalX*20,((Sy-decalY*20)/50)*50+decalY*20,50,50);
				cairo_fill(mask);
	}
}
else
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
