open Grammar_parser
open Ll_parser
open Printing


let grammar_file = ref None
let table_file = ref None
let parser_c_file = ref None
let parser_h_file = ref None

let int_of_lltype = function
  | First i
  | Follow i -> i

let nth_rule (toks,nts,rules) a =
  (List.nth rules (int_of_lltype a - 1))

let rec filter_map f l =
  match l with
    [] -> []
  | a::r ->
    match f a with
    | Some fa -> fa :: filter_map f r
    | _ -> filter_map f r

let filter_mapi f l =
  filter_map (fun (i,e) -> f i e)
    (List.mapi (fun i e -> (i,e)) l)

let hats pl =
  filter_map (fun (s, oq) ->
      match oq with
      | Some Qualifiers.Root -> Some s
      | _ -> None
    ) pl


let get_list_heads pl =
  filter_mapi (fun i (s, oq) ->
      match oq with
      | Some Qualifiers.ListHead -> Some i
      | _ -> None
    ) pl

let get_list_tails pl =
  filter_mapi (fun i (s, oq) ->
      match oq with
      | Some Qualifiers.ListTail -> Some i
      | _ -> None
    ) pl

let not_bangs pl =
  filter_map (fun (i,(s, oq)) ->
      if oq = None then Some i else None)
    (List.mapi (fun i e -> (i,e)) pl)

let rec make_c_list l =
  match l with
    [] -> "NULL"
  | i::r -> Printf.sprintf "cons($%d,%s)" (i+1) (make_c_list r)

(* If no element has a hat, return this list of elements without a bang. If one
   elemt has a hat, it serves as the root for this node, and the not-banged
   tokens are its children. *)
let default_action (pl: (string * Qualifiers.qualt option) list) nt : string =
  let h = hats pl in
  let nb = not_bangs pl in
  let lhs = get_list_heads pl in
  let lts = get_list_tails pl in
  if List.length h > 0 && (List.length lhs > 0 || List.length lts > 0)
  then
    failwith (Printf.sprintf "Node roots and list specifications are incompatible (see rule for non-terminal %s)\n" nt)
  else
  if List.length lhs > 0 || List.length lts > 0
  then
    match lhs, lts with
      [hd],[tl] -> Printf.sprintf "return cons($%d, $%d);" (hd+1) (tl+1)
    | _, _ ->
      failwith (Printf.sprintf "List specifications require exactly one head and \
                                one tail (see rule for non-terminal %s)\n" nt)
  else
    match h with
    | [h] -> Printf.sprintf "return make_node(%s, %s);" h (make_c_list nb)
    | [] ->  Printf.sprintf "return %s;" (make_c_list nb)
    | _ -> failwith
             (Printf.sprintf "Expected only one root in rule for non-terminal %s\n" nt)


let print_nt_types (l : (string*string) list) =
  List.iter (fun (nt, t) -> Printf.printf "%s -> %s\n" nt t) l

let find_type nts_typed nt =
  try List.assoc nt nts_typed
  with _ ->
    print_nt_types nts_typed;
    failwith (Printf.sprintf "Unable to find type for nt = %s\n" nt)

let make_nt nts_typed (table: string*string -> lltype list) (toks,nts,rules) oc n () =
  let typ = find_type nts_typed n in
  Printf.fprintf oc "%s parse_%s(lexer_state* lex){\n" typ n;
  List.iter
    (fun t ->
       let rules = List.map (fun a -> nth_rule (toks,nts,rules) a) (table (n,t)) in
       match rules with
         [] -> ()
       | (_,pl,act)::_ ->
         let act = match act with
           | Some act -> act
           | _ -> default_action pl n in
         let vars = vars_of_act act in
         Printf.fprintf oc "  if(lex->symbol.tag ==  %s){\n" t;
         List.iteri
           (fun i (t,_) ->
              if List.mem t toks
              then
                if List.mem (i+1) vars
                then Printf.fprintf oc "    char* p%d = eat(lex, %s);\n" (i + 1) t
                else Printf.fprintf oc "    eat(lex, %s);\n" t
              else
              if List.mem (i+1) vars
              then Printf.fprintf oc "    %s p%d = parse_%s(lex);\n" (find_type nts_typed t) (i+1) t
              else Printf.fprintf oc "    parse_%s(lex);\n" t)
           pl;
         Printf.fprintf oc "\n" ;
         Printf.fprintf oc "    %s\n" (resolve_vars act);
         Printf.fprintf oc "}\n";
    )
    toks;
  let expected = List.filter (fun t -> List.length (table (n,t)) > 0) toks in
  Printf.fprintf oc "  syntax_error_message(lex, \"error while parsing %s\");\n" n;
  Printf.fprintf oc "  printf(\"Expected one of \");\n";
  begin
    match expected with
      [] -> Printf.fprintf oc "  printf(\"{}\");\n"
    | a::r -> Printf.fprintf oc "  printf(\"{ %%s\", string_of_token(%s));\n" a;
      List.iter (fun t ->
          Printf.fprintf oc "  printf(\", %%s\", string_of_token(%s));\n" t;
        ) r;
      Printf.fprintf oc "  printf(\"}\");\n"
  end;
  Printf.fprintf oc "  printf(\" but got '%%s' instead.\\n\", string_of_symbol(lex->symbol));\n";
  Printf.fprintf oc "  exit(1);\n";
  (* Printf.fprintf oc "  return NULL;\n"; *)
  Printf.fprintf oc "}\n"


let make_parser phf (nts_typed: (string*string) list) (table: string*string -> lltype list) (toks,nts,rules) oc () =
  Printf.fprintf oc "#include \"%s\"\n" (Filename.basename phf);
  Printf.fprintf oc "#include \"parser.h\"\n";
  Printf.fprintf oc "#include \"lexer.h\"\n";
  Printf.fprintf oc "#include \"datatypes.h\"\n";
  Printf.fprintf oc "#include \"ast.h\"\n";
  Printf.fprintf oc "#include <stdio.h>\n";
  Printf.fprintf oc "#include <stdlib.h>\n";
  Printf.fprintf oc "#include <string.h>\n";
  List.iter (fun n -> make_nt nts_typed table (toks,nts,rules) oc n ()) nts

let make_header nts_typed (toks,nts,rules) oc () =
  List.iter (fun n ->
      let t = find_type nts_typed n in
      Printf.fprintf oc "%s parse_%s();\n" t n) nts

let erase_qualifiers_and_actions (rules: (string * (string*Qualifiers.qualt option) list * string option) list): (string * string list* string) list =
List.map (fun (nt, l, a) -> (nt, List.map fst l, (match a with Some a -> a | _ -> ""))) rules

let _ =
  Arg.parse
    [("-g", Arg.String ( fun s -> grammar_file := Some s), "Input grammar file (.g)");
     ("-t", Arg.String ( fun s -> table_file := Some s), "Where to output tables (.html)");
     ("-pc", Arg.String ( fun s -> parser_c_file := Some s), "Where to output the parser code (.c)");
     ("-ph", Arg.String ( fun s -> parser_h_file := Some s), "Where to output the parser header (.h)");
    ] print_endline "Usage: ";
  match !grammar_file with
  | None ->
    (Printf.eprintf "No grammar file specified. Exiting\n"; exit 1)
  | Some gf ->
    begin
    let (toks, nts, erules) = parse_grammar gf in
    let rules = erase_qualifiers_and_actions erules in
    let nts_typed = List.map (fun (a,b) -> (b,a)) nts in
    let nts = List.map snd nts in
    iter_nullnt (toks, nts, rules) ();
    iter_first (toks, nts, rules) ();
    iter_follownt (toks, nts, rules) ();
    fill_lltable (toks, nts, rules) ();
    begin match !table_file with
      | Some tfile -> let oc = open_out tfile in
        print_html (toks, nts, rules) oc ();
        close_out oc;
      | None -> Printf.eprintf "Not generating HTML report. Use -t option if desired.\n"
    end;
    begin match !parser_c_file, !parser_h_file with
      | Some cf, Some hf ->
        let oc = open_out cf in
        make_parser hf nts_typed
          (fun (n,t) -> hashget_def lltable (n,t) []) (toks, nts, erules) oc ();
        close_out oc;
        let oc = open_out hf in
        make_header nts_typed (toks, nts, rules) oc ();
        close_out oc;
      | _, _ ->
        Printf.eprintf "Not generating C parser. Use -pc and -ph options if desired.\n"
    end

  end
