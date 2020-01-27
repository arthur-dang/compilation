open Printf

let rec print_seq p = function
  | [] -> ""
  | a::r -> " " ^ p a ^ print_seq p r
let hashget_def t k d =
  match Hashtbl.find_opt t k with
  | None -> d
  | Some b -> b
