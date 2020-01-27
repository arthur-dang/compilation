open List_utils
open Printing
open List


let iter_until_fixpoint (f : unit -> bool) name =
  let rec aux i =
    if f ()
    then aux (i+1)
    else Printf.printf "Finished %s after %d steps.\n" name i in
  aux 1

let id x = x

let apply_on_all f l = List.exists id (List.map f l)

let nullt : (string, bool) Hashtbl.t = Hashtbl.create 20
let firstt : (string, string list) Hashtbl.t = Hashtbl.create 20
let followt : (string, string list) Hashtbl.t = Hashtbl.create 20

let nullable k = hashget_def nullt k false
let first n = hashget_def firstt n []
let follow n = hashget_def followt n []

let null_nt (toks,nts,rules) (n: string)  : bool =
  if hashget_def nullt n false then false
  else
    begin
      let r = List.filter (fun (x,_,_) -> x = n) rules in
      let r = List.map (fun (_,x,_) -> x) r in
      let can_be_null = List.exists (List.for_all nullable) r in
      Hashtbl.replace nullt n can_be_null; can_be_null
    end

let null_all_nt (toks,nts,rules) () : bool = apply_on_all (null_nt (toks,nts,rules)) nts

let iter_nullnt (toks,nts,rules) () = iter_until_fixpoint (null_all_nt (toks,nts,rules)) "nullnt"

let rec first_prod (toks,nts,rules) (p: string list) : string list =
  match p with
  | [] -> []
  | s::r ->
    if List.mem s toks then [s]
    else begin hashget_def firstt s [] @ if nullable s then first_prod (toks,nts,rules) r else [] end

let first_nt (toks,nts,rules) (n: string) : bool =
  let old = hashget_def firstt n [] in
  let r = List.filter (fun (x,_,_) -> x = n) rules in
  let r = List.map (fun (_,x,_) -> x) r in
  let f = cleardup (List.concat (List.map (first_prod (toks,nts,rules)) r)) in
  Hashtbl.replace firstt n f; not (same_list old f)

let iter_first (toks,nts,rules) () =
  iter_until_fixpoint (fun () -> apply_on_all (first_nt (toks,nts,rules)) nts) "firstt"

let rec cut_prod (x: string) (l: string list) : string list =
  match l with
    [] -> []
  | a::r -> if a = x then r else cut_prod x r

let null_prodlist (toks,nts,rules)(pl: string list) =
  List.for_all (fun p ->
      if List.mem p toks then false
      else nullable p) pl

let follow_nt (toks,nts,rules) (n: string) : bool =
  let old = follow n in
  let l = List.filter (fun (_,x,_) -> List.mem n x) rules in
  let l = List.map (fun (fx,sx,_) -> (fx, cut_prod n sx)) l in
  let l = List.map (fun x -> (if null_prodlist (toks,nts,rules) (snd x)
                              then follow (fst x)
                              else []) @ (first_prod (toks,nts,rules) (snd x))) l in
  let l = cleardup (List.concat l) in
  Hashtbl.add followt n l; old <> l

let follow_all_nt (toks,nts,rules) () = apply_on_all (follow_nt (toks,nts,rules)) nts

let iter_follownt (toks,nts,rules) () =
  iter_until_fixpoint (follow_all_nt (toks,nts,rules)) "follownt"

type lltype = First of int | Follow of int

let lltable : (string * string, lltype list) Hashtbl.t = Hashtbl.create 20

let add_into_table x t p =
  Hashtbl.add lltable (x,t) (cleardup (hashget_def lltable (x,t) [] @ [p]))


let fill_lltable (toks,nts,rules) () =
  List.iteri (fun i (x,gamma,_) ->
      List.iter (fun t -> add_into_table x t (First (i+1))) (first_prod (toks,nts,rules) gamma);
      if null_prodlist (toks,nts,rules) gamma
      then
        List.iter (fun t -> add_into_table x t (Follow (i+1))) (follow x)
    ) rules

let string_of_lltype = function
  | First i -> Printf.sprintf "<a style='color:blue;' href=\"#rule-%d\">%d</a>" i i
  | Follow i -> Printf.sprintf "<a style='color:red;' href=\"#rule-%d\">%d</a>" i i


let print_table (toks,nts,rules) oc () =
  Printf.fprintf oc "<!DOCTYPE html>" ;
  Printf.fprintf oc "<html><head><link rel='stylesheet' type='text/css' href='style.css'/></head>\n";
  Printf.fprintf oc "<body><table><tr><th></th>" ;
  List.iter (fun t ->
      Printf.fprintf oc " <th class='bigth'><div class='verticalText'>%s</div></th> " t
    ) toks;
  Printf.fprintf oc "</tr>\n";
  List.iteri
    (fun i x ->
       Printf.fprintf oc "<tr><td style='text-align:center;'>%s</td> " x;
       List.iter (fun t ->
           let rs = (hashget_def lltable (x,t) []) in
           Printf.fprintf oc " <td style='text-align:center;%s'>%s</td> "
             (if List.length rs > 1 then "background: rgba(255, 99, 71, 0.5);" else "")
             (print_seq string_of_lltype rs)
         ) toks;
    )
    nts;
  Printf.fprintf oc "</table></body></html>\n"

let grammar_hash (toks,nts,rules) =
  let h = Hashtbl.create (List.length nts) in
  List.iter ( fun (nt,p,a) ->
      match Hashtbl.find_opt h nt with
      | None -> Hashtbl.add h nt [(p,a)]
      | Some lp -> Hashtbl.replace h nt (lp@[(p,a)]) ) rules; h


let str_replace s s1 s2 =
  Str.global_replace (Str.regexp (Str.quote s1)) s2 s

let resolve_vars s =
  Str.global_replace (Str.regexp "\\$\\([0-9]+\\)") "p\\1" s

let vars_of_act s =
  let rec aux pos =
    let new_var =
      try Some (Str.search_forward (Str.regexp "\\$\\([0-9]+\\)") s pos)
      with _ -> None
    in
    match new_var with
      None -> []
    | Some x ->
      let str = Str.matched_group 1 s in
      int_of_string str::aux (x + String.length str)
  in
  aux 0

let print_grammar (toks,nts,rules) oc () =
  let counter = ref 1 in
  Printf.fprintf oc "<table>" ;
  List.iter (fun nt  ->
      let rules = try Hashtbl.find (grammar_hash (toks,nts,rules)) nt with _ -> failwith (Printf.sprintf "Not found nt = %s" nt) in
      let first_rule = ref true in
      List.iter ( fun (rule,act) ->
          Printf.fprintf oc " <tr id='rule-%d' style='%s;'><td class='left'>(%d)</td> <td class='left'> %s </td class='left'> <td class='left'> -> </td> <td class='left'> %s</td><td class='left'><pre>%s</pre></td></tr>\n"
        !counter
        (if !first_rule then "border-top: solid 1px" else "border: none")
        !counter
        (if !first_rule then Printf.sprintf "<span id='nt-%s'>%s</span>" nt nt else "")
        (if rule = [] then "&epsilon;" else print_seq (fun x -> if List.mem x toks then x else Printf.sprintf "<a href='#nt-%s' style='color: black;'>%s</a>" x x) rule)
        act ;
        first_rule := false;
        counter:=!counter+1
      ) rules ;
      Printf.fprintf oc "\n"
  ) nts;
  Printf.fprintf oc "</table>\n"


let print_null (toks,nts,rules) oc () =
  Printf.fprintf oc "<br><table><tr><th>Non-terminal</th><th>Nullable</th></tr>\n" ;
  List.iteri (fun i x  ->
      Printf.fprintf oc "<tr><td> %s</td><td>  %s</td></tr> \n"
        x
        (if nullable x then "true" else "false")
    ) nts;
  Printf.fprintf oc "</table>\n<br>"


let print_first (toks,nts,rules) oc () =
  Printf.fprintf oc "<br><table><tr><th>Non-terminal</th><th>First</th></tr>\n" ;
  List.iteri (fun i x  ->
      Printf.fprintf oc " <tr><td>%s</td><td>%s</td></tr> \n"
        x
        (print_seq (fun y -> y) (first x))
    ) nts;
  Printf.fprintf oc "</table>\n"


let print_follow (toks,nts,rules) oc () =
  Printf.fprintf oc "<table><tr><th> Non-terminal</th><th> Follow</th></tr>\n" ;
  List.iteri (fun i x  ->
      Printf.fprintf oc "<tr><td> %s </td><td> %s</td></tr> \n"
        x
        (print_seq (fun y -> y) (follow x))
    ) nts;
  Printf.fprintf oc "</table>\n"




let print_html (toks,nts,rules) oc () =
  Printf.fprintf oc "<!DOCTYPE html>" ;
  Printf.fprintf oc "<html><head>\n";
  (* <link rel='stylesheet' type='text/css' href='style.css'/> *)
  Printf.fprintf oc "<style type=\"text/css\">.bigth\n\
 {\n\
     text-align: center;\n\
     vertical-align: bottom;\n\
     height: 250px;\n\
     padding-bottom: 3px;\n\
     padding-left: 5px;\n\
     padding-right: 5px;\n\
 }\n\
 table {\n\
     display: inline-table;\n\
     margin: 1em;\n\
     border: solid 1px;\n\
     border-width: 1px;\n\
     border-collapse: collapse;\n\
 }\n\
 table th, table td {\n\
     border: solid 1px;\n\
     border-collapse: collapse;\n\
 }\n\
 .verticalText\n\
 {\n\
     text-align: center;\n\
     vertical-align: middle;\n\
     width: 20px;\n\
     margin: 0px;\n\
     padding: 0px;\n\
     padding-left: 3px;\n\
     padding-right: 3px;\n\
     padding-top: 10px;\n\
     white-space: nowrap;\n\
     -webkit-transform: rotate(-90deg);\n\
     -moz-transform: rotate(-90deg);\n\
 }\n\
 .left{\n\
     text-align: left;\n\
     border:none;\n\
 }</style>\n";
  Printf.fprintf oc "</head>\n";
  Printf.fprintf oc "<body>\n" ;
  Printf.fprintf oc "<h1>Grammaire</h1>\n";
  print_grammar (toks,nts,rules) oc ();
  Printf.fprintf oc "<h1>Table Null</h1>\n";
  print_null (toks,nts,rules) oc ();
  Printf.fprintf oc "<h1>Table First</h1>\n";
  print_first (toks,nts,rules) oc ();
  Printf.fprintf oc "<h1>Table Follow</h1>\n";
  print_follow (toks,nts,rules) oc ();
  Printf.fprintf oc "<h1>Table LL</h1>\n";
  print_table (toks, nts, rules) oc ();
  Printf.fprintf oc "</body>\n</html>\n"


