typedef struct resultat_t {
  bool is_intersect;
  Vector point;
  Vector edge[2];
  char singular_point;
} Resultat;


void calcul_intersection(Vector a,Vector b,Vector c,Vector d,unsigned char m[4],
                         Resultat* r){
  //Pour la 3D il faut que a,b,c,d soit coplanaire
  
  int discriminant;
  float paramS;
  float paramT;

  if(are_segment_intersected(a,b,c,d,m)){
    r->is_intersect=true;
    Vector AB=V_substract(b,a);
    Vector DC=V_substract(c,d);
    Vector CA=V_substract(a,c);
    Vector DB=V_substract(b,d);

    discriminant=(AB.x*DC.y)-(AB.y*DC.x);

    if (discriminant==0) {
      //determinant de ABC pour vérifier l'alignement  
      if(det_from_vectors(a,b,c)=0){
        //A entre C et D
        //B entre C et D
        //C entre A et B
        //D entre A et B
      }  
    }else {
      paramS=((DC.x*CA.y)-(CA.x*DC.y))/discriminant;
      paramT=((CA.x*DB.y)-(DB.x*CA.y))/discriminant;

      if((paramS<=1 && paramS>=0) && (paramT<=1 && paramT>=0)){
        if (paramS==1) {
          //Intersection en A
          r->singular_point='a';
        }else if (paramS==0) {
          //Intersection en B
          r->singular_point='b';
        }else if (paramT==1) {
          //Intersection en C
          r->singular_point='c';
        }else if (paramT==0) {
          //Intersection en D
          r->singular_point='d';
        }
        
        r->point.x=paramS*AB.x;
        r->point.y=paramS*AB.y;
        return ;
      }
    }
  }
  r->is_intersect=false;
}
