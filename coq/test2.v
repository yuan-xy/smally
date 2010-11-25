Section Minimal_propositional_logic.
Variables P Q R T : Prop.
Theorem syllogism : (P->Q) -> (Q->R) -> P -> R.
Proof.
intros H0 H1 p.
apply H1.
apply H0.
assumption.
Qed.

Theorem imp_trans : (P->Q) -> (Q->R) -> P -> R.
Proof.
auto.
Qed.

Theorem apply_example : (Q->R->T)->(P->Q)->P->R->T.
Proof.
intros H HO p.
apply H.
apply HO.
assumption.

Save.




End  Minimal_propositional_logic.


