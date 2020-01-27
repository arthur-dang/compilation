open List

(* Teste si toutes les valeurs de [l1] sont dans [l2].
   (Relation "sous-ensemble") *)
let incl l1 l2 =
  for_all (fun x -> mem x l2) l1

(* Relation "même ensemble". *)
let same_list l1 l2 = incl l1 l2 && incl l2 l1

(* L'ensemble des éléments de [l1] qui ne sont pas dans [l2]. *)
let diff l1 l2 =
  filter (fun x -> not (mem x l2)) l1

(* Enlève les doublons dans une liste. *)
let rec cleardup l =
  match l with
    [] -> []
  | a::r -> if mem a r then cleardup r else a::cleardup r

let inter l1 l2 =
  filter (fun x -> mem x l2) l1

let disjoint l1 l2 =
  inter l1 l2 = []
