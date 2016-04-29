Require Import Bool.
Require Import List.
Require Import NArith.
Require Import Omega.
Import ListNotations.


Open Scope list_scope.

(* COQ doesn't accept type that begin with literal number *)
Inductive coord_aux : Type := 
| _1 : coord_aux
| _2 : coord_aux
| _3 : coord_aux.

Definition coord := (coord_aux * coord_aux)%type.


(*
The board is define as : 
bool1 bool2 bool3
bool4 bool5 bool6
bool7 bool8 bool9  
*)
Definition plateau := (bool * bool * bool * bool * bool * bool * bool * bool * bool )%type.
Check (plateau). 

(* Game defined as a list *)
Definition partie : Type := list coord .


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


Fixpoint applique_partie (pl :plateau) (p: partie) : plateau :=
    match p with 
    | nil => pl
    | x::p_tail => applique_partie (applique_clic pl x) p_tail
end.


Definition plateau_init_test : plateau := (false,false,false,false,false,false,false,false,false).
Check plateau_init_test.


Definition plateau_gagnant (pl: plateau): bool :=
    match pl with 
    | (true,true,true,true,true,true,true,true,true) => true
    | (_,_,_,_,_,_,_,_) => false
end.


Definition partie_gagnante (pl: plateau) (p : partie): bool :=
   match (plateau_gagnant ( applique_partie pl p)) with 
   | true => true
   | false => false
end.

(* Not proved *)
Lemma same_click : forall pl:plateau , forall c:coord,
applique_clic (applique_clic pl c) c = pl.
Proof.
admit.
Qed.

(* Not proved *)
Lemma independent_action : forall pl:plateau , forall c1:coord, forall c2:coord,
applique_clic (applique_clic pl c2) c1 = applique_clic (applique_clic pl c1) c2.
Proof.
admit.
Qed.

(* The function to change only one case does NOT implement the game's rule and thus should be considered as placeholders *)
Definition change_une_coord (c: coord): partie :=
    match c with 
    | (_1, _1) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_1, _2) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_1, _3) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_2, _1) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_2, _2) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_2, _3) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_3, _1) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_3, _2) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
    | (_3, _3) =>   [(_1,_1);(_1,_1);(_1,_1);(_1,_1)]
end.

(* 
Dirty hack in order to define the recurisve function by decrementing an useless value  >= 9
The function must be called that way : liste_blanches plateau [] 9 
*)
Fixpoint liste_blanches (pl: plateau) (p: partie) (n: nat): partie :=
    match n with | 0 =>p | S sub =>
        match pl with 
        |(bool1, bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 ) => if (eqb bool1 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool2 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool3 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool4 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool5 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool6 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool7 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool8 false)
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else if (eqb bool9 false) 
                                                                     then liste_blanches (negb (bool1), bool2, bool3, bool4, bool5, bool6, bool7, bool8, bool9 )(p ++ [(_1,_1);(_2,_2)]) sub
                                                                     else p

        end
    end.

Theorem exists_partie_gagnante_pour_init_test :
exists p, partie_gagnante plateau_init_test p.