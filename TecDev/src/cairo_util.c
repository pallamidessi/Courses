#include<cairo_util.h>


cairo_t* tracer_grille(cairo_surface_t *surface,grille l,int decalX,int decalY){
	cairo_t* mask;
  mask=cairo_create(surface);

	 int i=0;//j=0;
	  
	cairo_set_source_rgb(mask,223 ,223 ,223); 
  cairo_paint(mask);
	
	cairo_set_source_rgb(mask,0.1,0.1,0.1); 

	cairo_set_line_width (mask, 0.5);
		 for ( i=0;i<=l->N;i++){
			 cairo_move_to(mask,decalX*20,decalY*20+i*50);
		   cairo_line_to(mask,decalX*20+l->N*50, decalY*20+i*50);
			 cairo_stroke_preserve(mask);
				}
																				   
		 for ( i=0;i<=l->M;i++){
			 cairo_move_to(mask,decalX*20+i*50,decalY*20);
		   cairo_line_to(mask,decalX*20+i*50, decalY*20+l->M*50);
			 cairo_stroke_preserve(mask);
				}
return mask;				
}


