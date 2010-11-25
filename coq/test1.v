Require Import Arith.
Require Import ZArith.
Require Import Bool.

Open Scope Z_scope.


Locate "_ * _".

Print Scope Z_scope.

Check 33%nat.

Check 33.

Check O.

Check false.

Check plus.

Check negb.

Check (negb true).

Check S.

Check (S (S O)).

Check 4.

Check (5*(4-2)+7).

Check fun n:nat =>(n*n*n)%nat.

Check (fun n _:nat => n).

Check (fun n (z:Z) f => (n+(Zabs_nat (f z)))%nat).

(* Check (fun f x => Zabs_nat(f x)). *)

Parameter max_int : Z.  (* golbal parameter *)

Definition min_int := 1 - max_int.

Print  min_int.

Definition cube := fun z:Z => z*z*z.
Print cube.

Section binomial_def.
Variables a b:Z.          (* local Variable *)
Definition binomial z:Z := a*z+b.
End binomial_def.

Print binomial.

Section h_def.
Variables a b:Z.
Let s:Z := a+b.
Let d:Z := a-b.
Definition hh : Z := s*s + d*d.
End h_def.

Print hh.

Definition Zsqr (z:Z) : Z := z*z.
Definition my_fun (f:Z->Z)(z:Z) : Z := f (f z).

Eval cbv [my_fun] in (my_fun Zsqr).
(* delta expand the lable *)
Eval cbv delta [my_fun] in (my_fun Zsqr).
Eval cbv delta [my_fun Zsqr] in (my_fun Zsqr).

Eval cbv beta delta [my_fun Zsqr] in (my_fun Zsqr).

Definition h := fun s d:Z => s*s + d*d.

Eval cbv [h] in (h 2 3).
Eval cbv delta [h] in (h 2 3).
Eval cbv beta delta [h] in (h 2 3).
Eval cbv beta zeta delta in (h 2 3).
Eval compute in (h 2 3).
Eval cbv iota beta zeta delta in (h 2 3).

