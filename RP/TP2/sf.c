#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define Nr 5
#define SAMPLES_PER_PERIOD	100

FILE *fp ;
float pi = M_PI;

float coef_sf(int nb_harm,float a[],float b[])
{
  int i;

  
  a[0]=0.5; 	
  for(i=1;i<=nb_harm;i++) a[i] = ((2./(pi*(float)i))*(-cos(pi*(float)i)+1)); 
 
  b[0] = 0.;
  for(i=1;i<=nb_harm;i++) b[i]=(sin(pi*i))/(pi*i) ;// 

}




float somcos(int nb_harm, float f, float x,float bb[])
{
  float sc=0.0;
  int i;
  for(i=0;i<=nb_harm;i++) sc += bb[i]*cos(2*pi*(float)i*f*x)  ;  
  return sc;
}
		    
float somsin(int nb_harm, float f, float x,float aa[])
{
  float ss=0.0;
  int i;
  for(i=0;i<nb_harm;i++) ss += aa[i]*sin((float)i*x*2*pi*f) ; 
  return ss;
}


main()
{
  int i,nb_harm, nb_periodes;
  float periode, fondamentale, t, dt, *aa, *bb, sa, sb, ss;
  
  printf(" Nb harmoniques = ");
  scanf("%d",&nb_harm);
  aa = (float*)malloc((unsigned)(nb_harm+1)*sizeof(float)) ;
  bb = (float*)malloc((unsigned)(nb_harm+1)*sizeof(float)) ;
  coef_sf(nb_harm,aa,bb);
  
  for(i=0;i<=nb_harm;i++)printf(" a[%d] = %8.4f ; b[%d] = %8.4f \n",i,aa[i],i,bb[i]);
  
  printf(" Période = ");
  scanf("%f",&periode);
  fondamentale=1./periode;
  
  printf(" Nb périodes = ");
  scanf("%d",&nb_periodes);

  dt=periode/(float)(nb_harm*SAMPLES_PER_PERIOD); 
  t=0.;

  fp=fopen("sf.dat","w");
  while(t<(nb_periodes*periode)) {
      sa=somsin(nb_harm,fondamentale,t,aa);
      sb=somcos(nb_harm,fondamentale,t,bb);
      ss=sa+sb;
      fprintf(fp,"%f %f\n",t,ss);
      t += dt;
 }
    
 fclose(fp);
 printf(" Nb harmoniques = %d\n",nb_harm);
 printf("Sortie dans sf.dat\n");

  
}

