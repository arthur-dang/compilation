
(* The type of tokens. *)

type token = 
  | TYP of (string)
  | TOK
  | RULES
  | NT
  | LISTTAIL
  | LISTHEAD
  | IDENTIFIER of (string)
  | HAT
  | EXCLAMATION
  | EOL
  | EOF
  | CODE of (string)
  | ARROW

(* This exception is raised by the monolithic API functions. *)

exception Error

(* The monolithic API. *)

val main: (Lexing.lexbuf -> token) -> Lexing.lexbuf -> (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
