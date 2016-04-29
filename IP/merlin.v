(* Technical help

negb               boolean negation
Even.even_odd_dec  parity test
count_occ          occurence counter
++                 list concatenation
length             list length
fold_left          see documentation

Tactics: destruct, omega, ...
Lemmas:  negb_involutive, ...

*)

Require Import Bool.
Require Import List.
Require Import NArith.
Require Import Omega.

Definition couple := (nat * nat)%type.

(* Game Modeling *)

(* PREVIOUS GARBAGE

Inductive array (A: Type) : Type
 := mkArray(elems: list A) (default: A).


Inductive plateau (n: nat) : Type
 := ca: forall l: list Z, length l = n -> plateau n.

Hypothesis Aeq_dec : forall x y:bool, {x = y} + {x <> y}.

Definition set := list bool.

Fixpoint set_add (a: bool) (x:set) : set :=
    match x with
    | nil => a :: nil
    | a1 :: x1 =>
        match Aeq_dec a a1 with
        | left _ => a1 :: x1
        | right _ => a1 :: set_add a x1
        end
    end.

Definition plateau9 := set_add true (set_add true (set_add true nil)).

Print plateau9.

let pla := set_add true nil.

*)

(* https://www.lri.fr/~paulin/LASER/course-notes.pdf FIN PAGE 16 *)

Inductive color : Type :=  White | Black.
Inductive triple M := Triple : M -> M -> M -> triple M.
Definition board :=  triple (triple  color).
Notation "[ x | y | z ]" := (Triple x y z).
Definition start : board
 := [ [White | White | Black] |
      [Black | White | White] |
      [Black | Black | Black] ].