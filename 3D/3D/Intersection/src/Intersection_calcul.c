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
  bool first_edge=true;

  if(are_segment_intersected(a,b,c,d,m)){
    r->is_intersect=true;
    Vector AB=V_substract(b,a);
    Vector DC=V_substract(c,d);
    Vector CA=V_substract(a,c);
    Vector DB=V_substract(b,d);

    discriminant=(AB.x*DC.y)-(AB.y*DC.x);

    if (discriminant==0) {
      //determinant de ABC pour vérifier l'alignement  
      if(det_from_vectors(a,b,c)=0 ){
        
        //A entre C et D
        if(V_dot(V_substract(c,a),V_dot(V_substract(d,a)))<=0){
          r->egde[0]=a;
          first_edge=false;
        }
        
        //B entre C et D
        if(V_dot(V_substract(c,b),V_dot(V_substract(d,b)))<=0){
          r->singular_point='b';
          if(first_edge){
            r->egde[0]=b;
            first_edge=false;
          }
          else{
            egde[1]=b;
            return ;
          }
        }
        
        //C entre A et B
        if(V_dot(V_substract(a,c),V_dot(V_substract(b,c)))<=0){
          r->singular_point='a';
          if(first_edge){
            r->egde[0]=c;
            first_edge=false;
          }
          else{
            egde[1]=c;
            return ;
          }
        }
        
        //D entre A et B
        if(V_dot(V_substract(a,d),V_dot(V_substract(b,d)))<=0){
          egde[1]=b;
          return ;
        }
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
