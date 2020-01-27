open Str
open Printing
open List_utils
open Lexing
open Grammar_lexer
open Grammar_parser_yacc
open Qualifiers

let print_list oc l =
  List.iter (fun s -> Printf.fprintf oc "%s " s) l


let print_position outx lexbuf =
  let pos = lexbuf.lex_curr_p in
  Printf.fprintf outx "%s:%d:%d" pos.pos_fname
    pos.pos_lnum (pos.pos_cnum - pos.pos_bol + 1)

let parse_with_error lexbuf =
  try main token lexbuf with
  | Error ->
    Printf.fprintf stderr "%a: syntax error\n" print_position lexbuf;
    exit (-1)

let parse_grammar file : string list * (string * string) list * (string * (string * qualt option) list * string option) list =
  let ic = open_in file in
  let lexbuf = Lexing.from_channel ic in
  lexbuf.Lexing.lex_curr_p <- {lexbuf.Lexing.lex_curr_p with pos_fname = file};
  let (toks,nts,rules) = parse_with_error lexbuf in
  let h = Hashtbl.create (List.length nts) in
  let (undefined_strings, used_strings) =
    List.fold_left (fun (undef, used) (nt, prods, act) ->
        List.fold_left
          (fun (undef, used) (prod, _) ->
             let undef =
               if not (List.mem prod toks || List.mem prod (List.map snd nts) || List.mem prod undef)
               then prod::undef
               else undef in
             let used =
               if List.mem prod used then used else prod::used in
             (undef, used)
          )
          (undef, used) prods
      ) ([],[]) rules in
  if undefined_strings <> []
  then (Printf.printf "Undefined tokens or non-terminals: %a\n" print_list undefined_strings;
        failwith "Undefined tokens or non-terminals");

  Printf.printf "Unused non-terminals: %a\n" print_list (List.filter (fun nt -> not (List.mem nt used_strings)) (List.map snd nts));
  Printf.printf "Unused tokens: %a\n" print_list (List.filter (fun nt -> not (List.mem nt used_strings)) toks);
  List.iter ( fun (nt,p,a) ->
      match Hashtbl.find_opt h nt with
      | None -> Hashtbl.add h nt [(nt,p,a)]
      | Some lp -> Hashtbl.replace h nt (lp@[(nt,p,a)]) ) rules;
  let rules = List.concat (List.map (fun n -> hashget_def h n []) (List.map snd nts)) in
  (toks,nts,rules)



let dump_grammar oc (toks, nts, rules) =
  Printf.fprintf oc "tokens";
  List.iter (fun n -> Printf.fprintf oc " %s" n) toks;
  Printf.fprintf oc "\nnon-terminals ";
  List.iter (fun n -> Printf.fprintf oc " %s" n) nts;
  Printf.fprintf oc "\nrules\n";
  List.iter (fun (n,lt,a) -> Printf.fprintf oc "%s ->%s\n" n (print_seq (fun x -> x) lt)) rules
