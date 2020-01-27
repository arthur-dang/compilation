%{

    open Qualifiers

    %}

%token EOF EOL TOK NT RULES ARROW HAT EXCLAMATION LISTHEAD LISTTAIL
%token<string> IDENTIFIER
%token<string> CODE TYP

%start main
%type <string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list> main

%%

  main:
    | TOK list_ident EOL main { let (toks,nts,rules) = $4 in (toks @ $2, nts, rules) }
      | NT TYP list_ident EOL main { let (toks,nts,rules) = $5 in (toks, nts @ List.map (fun l -> ($2,l)) $3, rules) }
      | RULES EOL rules EOF { ([],[],$3) }
    
    ;
 
      list_ident:
        | IDENTIFIER list_ident { $1 :: $2}
        | { [] }
    ;

      list_qual_ident:
        | IDENTIFIER qualifier list_qual_ident { ($1,$2) :: $3}
        | { [] }
    ;


      qualifier:
        | HAT { Some (Root) }
        | LISTHEAD { Some (ListHead) }
        | LISTTAIL { Some (ListTail) }
        | EXCLAMATION { Some (Ignore) }
        | { None }
    ;

      rules:
        | rule rules { $1 :: $2 }
        | EOL rules {$2}
        | {[]}
    ;

      rule:
        | IDENTIFIER ARROW list_qual_ident EOL { ($1,$3, None) }
        | IDENTIFIER ARROW list_qual_ident EOL? CODE EOL { ($1,$3, Some ($5)) }
;
%%
