Require Import Bool.
Require Import List.
Require Import NArith.
Require Import Omega.
Import ListNotations.

Open Scope list_scope.


Inductive coord_aux : Type := 
| _1 : coord_aux
| _2 : coord_aux
| _3 : coord_aux.

Definition coord := (coord_aux * coord_aux)%type.

Definition plateau := (bool * bool * bool * bool * bool * bool * bool * bool * bool )%type.
Check (plateau). 

Inductive partie : Type :=
| partie_nil : partie 
| partie_cons : list coord -> partie -> partie.


(* Actions on click are not correct as they are just placeholders and does not implement the game's rules *)
Definition applique_clic (pl :plateau) (c:coord) : plateau :=
    match pl with 
    | (bool1, bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 ) => match c with 
                                                                              |(_1, _1) => ( (negb bool1),bool2,(negb bool3),bool4,bool5,bool6,(negb bool7),bool8,(negb bool9) )
                                                                              |(_1, _3) => ( (negb bool1),bool2,(negb bool3),bool4,bool5,bool6,(negb bool7),bool8,(negb bool9) )
                                                                              |(_3, _1) => ( (negb bool1),bool2,(negb bool3),bool4,bool5,bool6,(negb bool7),bool8,(negb bool9) )
                                                                              |(_3, _3) => ( (negb bool1),bool2,(negb bool3),bool4,bool5,bool6,(negb bool7),bool8,(negb bool9) )
                                                                              |(_2, _2) => ( bool1,(negb bool2),bool3,(negb bool4),(negb bool5),(negb bool6),bool7,(negb bool8),bool9 )
                                                                              |(_1, _2) => ( bool1,(negb bool2),bool3,(negb bool4),(negb bool5),(negb bool6),bool7,(negb bool8),bool9 )
                                                                              |(_2, _1) => ( bool1,(negb bool2),bool3,(negb bool4),(negb bool5),(negb bool6),bool7,(negb bool8),bool9 )
                                                                              |(_2, _3) => ( bool1,(negb bool2),bool3,(negb bool4),(negb bool5),(negb bool6),bool7,(negb bool8),bool9 )
                                                                              |(_3, _2) => ( bool1,(negb bool2),bool3,(negb bool4),(negb bool5),(negb bool6),bool7,(negb bool8),bool9 )
                                                                              end
end.

Definition applique_partie (pl :plateau) (p: partie) () : plateau :=
.