Require Export ZArith.
Require Export String.
Require Export List.

Open Scope Z_scope.

Inductive aexpr: Type :=
Avar : string -> aexpr
|Anum : Z -> aexpr
|Amin : aexpr -> aexpr -> aexpr
|Amult : aexpr -> aexpr -> aexpr
|Aplus : aexpr -> aexpr -> aexpr.

Inductive bexpr: Type :=
Blt : aexpr -> aexpr -> bexpr
|Beq : aexpr -> aexpr -> bexpr
|Band : aexpr -> aexpr -> bexpr.

Inductive instr: Type :=
Assign : string -> aexpr -> instr
|Seq : instr -> instr -> instr
|While : bexpr -> instr -> instr
|If : bexpr -> instr -> instr -> instr
|Skip : instr.


Definition e1 : aexpr := (Aplus ((Aplus (Avar "x") (Anum 3))) (Avar "y")).
Definition b1 : bexpr := (Blt ((Aplus (Avar "x") (Anum 3))) (Anum 5)).

Inductive assert :Type :=
Lvar : bexpr -> assert
|Impl: assert -> assert -> assert  
|Neg: assert -> assert
|Conj: assert -> assert -> assert.


Definition env := list(string*Z).

Fixpoint a_substitution (y: string) (e: aexpr) (v: aexpr): aexpr :=
match v with   
               Avar a => if string_dec a y then e else Avar a
             | Anum n => Anum n
             | Aplus expr1 expr2 => Aplus (a_substitution y e expr1) (a_substitution y e expr2) 
             | Amin expr1 expr2 => Amin (a_substitution y e expr1) (a_substitution y e expr1)
             | Amult expr1 expr2 => Amult (a_substitution y e expr1) (a_substitution y e expr1)
end.

Fixpoint b_substitution (y: string) (e: aexpr) (v: bexpr): bexpr :=
match v with   
             Blt expr1 expr2 => Blt (a_substitution y e expr1) (a_substitution y e expr2) 
             | Beq expr1 expr2 => Beq (a_substitution y e expr1) (a_substitution y e expr1)
             | Band expr1 expr2 => Band (a_substitution y e expr1) (a_substitution y e expr1)
end.

Fixpoint assert_substitution (y: string) (e: aexpr) (v: assert): assert :=
match v with   
             Lvar expr => Lvar (b_substitution y e expr) 
             | Impl expr1 expr2 => Impl (assert_substitution y e expr1) (assert_substitution y e expr2) 
             | Neg expr => Neg (assert_substitution y e expr)
             | Conj expr1 expr2 => Conj (assert_substitution y e expr1) (assert_substitution y e expr1)
end.

Open Scope list_scope.

Fixpoint G (y: string) (e: env): Z:=
match e with nil => 0
             |List.cons h t  => match h with (a, b) => if string_dec a y then b else G y t end
end.

Fixpoint Zeval (g: string -> Z) (a: aexpr) := 
   match a with 
|Avar z => g z
|Anum n => n 
|Aplus a1 a2 => (Zeval g a1) + (Zeval g a2)
|Amin a1 a2 => (Zeval g a1) - (Zeval g a2)
|Amult a1 a2 => (Zeval g a1) * (Zeval g a2)
end.

Fixpoint beval (g: string -> Z) (b: bexpr) :Prop:= 
   match b with 
|Beq a1 a2 => (Zeval g a1) = (Zeval g a2)
|Blt a1 a2 => (Zeval g a1) > (Zeval g a2)
|Band a1 a2 => (Zeval g a1) < (Zeval g a2)
end.

Inductive Hoare: assert-> instr -> assert -> Prop :=
H_skip: forall p:assert, Hoare p Skip p 
| H_seq: forall p1 p2 p3:assert, forall i1 i2:instr, 
Hoare p1 i1 p2 -> Hoare p2 i2 p3 -> Hoare p1 (Seq i1 i2) p3.


  


