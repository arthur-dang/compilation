
module MenhirBasics = struct
  
  exception Error
  
  type token = 
    | TYP of (
# 9 "grammar_parser_yacc.mly"
      (string)
# 11 "grammar_parser_yacc.ml"
  )
    | TOK
    | RULES
    | NT
    | LISTTAIL
    | LISTHEAD
    | IDENTIFIER of (
# 8 "grammar_parser_yacc.mly"
      (string)
# 21 "grammar_parser_yacc.ml"
  )
    | HAT
    | EXCLAMATION
    | EOL
    | EOF
    | CODE of (
# 9 "grammar_parser_yacc.mly"
      (string)
# 30 "grammar_parser_yacc.ml"
  )
    | ARROW
  
end

include MenhirBasics

let _eRR =
  MenhirBasics.Error

type _menhir_env = {
  _menhir_lexer: Lexing.lexbuf -> token;
  _menhir_lexbuf: Lexing.lexbuf;
  _menhir_token: token;
  mutable _menhir_error: bool
}

and _menhir_state = 
  | MenhirState31
  | MenhirState29
  | MenhirState24
  | MenhirState22
  | MenhirState15
  | MenhirState9
  | MenhirState7
  | MenhirState5
  | MenhirState2
  | MenhirState1
  | MenhirState0

# 1 "grammar_parser_yacc.mly"
  

    open Qualifiers

    
# 67 "grammar_parser_yacc.ml"

let rec _menhir_goto_option_EOL_ : _menhir_env -> 'ttv_tail -> (unit option) -> 'ttv_return =
  fun _menhir_env _menhir_stack _v ->
    let _menhir_stack = (_menhir_stack, _v) in
    let _menhir_stack = Obj.magic _menhir_stack in
    assert (not _menhir_env._menhir_error);
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | CODE _v ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = (_menhir_stack, _v) in
        let _menhir_env = _menhir_discard _menhir_env in
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOL ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _menhir_env = _menhir_discard _menhir_env in
            let _menhir_stack = Obj.magic _menhir_stack in
            let ((((_menhir_stack, _menhir_s, (_1 : (
# 8 "grammar_parser_yacc.mly"
      (string)
# 89 "grammar_parser_yacc.ml"
            ))), _, (_3 : ((string * Qualifiers.qualt option) list))), (_4 : (unit option))), (_5 : (
# 9 "grammar_parser_yacc.mly"
      (string)
# 93 "grammar_parser_yacc.ml"
            ))) = _menhir_stack in
            let _6 = () in
            let _2 = () in
            let _v : (string * (string * Qualifiers.qualt option) list * string option) = 
# 50 "grammar_parser_yacc.mly"
                                                         ( (_1,_3, Some (_5)) )
# 100 "grammar_parser_yacc.ml"
             in
            _menhir_goto_rule _menhir_env _menhir_stack _menhir_s _v
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            let _menhir_stack = Obj.magic _menhir_stack in
            let (((_menhir_stack, _menhir_s, _), _), _) = _menhir_stack in
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s, _), _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s

and _menhir_goto_rule : _menhir_env -> 'ttv_tail -> _menhir_state -> (string * (string * Qualifiers.qualt option) list * string option) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    let _menhir_stack = Obj.magic _menhir_stack in
    assert (not _menhir_env._menhir_error);
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | EOL ->
        _menhir_run22 _menhir_env (Obj.magic _menhir_stack) MenhirState24
    | IDENTIFIER _v ->
        _menhir_run8 _menhir_env (Obj.magic _menhir_stack) MenhirState24 _v
    | EOF ->
        _menhir_reduce19 _menhir_env (Obj.magic _menhir_stack) MenhirState24
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState24

and _menhir_goto_main : _menhir_env -> 'ttv_tail -> _menhir_state -> (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 137 "grammar_parser_yacc.ml"
) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    match _menhir_s with
    | MenhirState31 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_5 : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 147 "grammar_parser_yacc.ml"
        )) = _v in
        let (((_menhir_stack, _menhir_s), (_2 : (
# 9 "grammar_parser_yacc.mly"
      (string)
# 152 "grammar_parser_yacc.ml"
        ))), _, (_3 : (string list))) = _menhir_stack in
        let _4 = () in
        let _1 = () in
        let _v : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 159 "grammar_parser_yacc.ml"
        ) = 
# 18 "grammar_parser_yacc.mly"
                                   ( let (toks,nts,rules) = _5 in (toks, nts @ List.map (fun l -> (_2,l)) _3, rules) )
# 163 "grammar_parser_yacc.ml"
         in
        _menhir_goto_main _menhir_env _menhir_stack _menhir_s _v
    | MenhirState5 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_4 : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 172 "grammar_parser_yacc.ml"
        )) = _v in
        let ((_menhir_stack, _menhir_s), _, (_2 : (string list))) = _menhir_stack in
        let _3 = () in
        let _1 = () in
        let _v : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 180 "grammar_parser_yacc.ml"
        ) = 
# 17 "grammar_parser_yacc.mly"
                              ( let (toks,nts,rules) = _4 in (toks @ _2, nts, rules) )
# 184 "grammar_parser_yacc.ml"
         in
        _menhir_goto_main _menhir_env _menhir_stack _menhir_s _v
    | MenhirState0 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_1 : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 193 "grammar_parser_yacc.ml"
        )) = _v in
        Obj.magic _1
    | _ ->
        _menhir_fail ()

and _menhir_goto_list_qual_ident : _menhir_env -> 'ttv_tail -> _menhir_state -> ((string * Qualifiers.qualt option) list) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    match _menhir_s with
    | MenhirState15 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let (((_menhir_stack, _menhir_s, (_1 : (
# 8 "grammar_parser_yacc.mly"
      (string)
# 209 "grammar_parser_yacc.ml"
        ))), (_2 : (Qualifiers.qualt option))), _, (_3 : ((string * Qualifiers.qualt option) list))) = _menhir_stack in
        let _v : ((string * Qualifiers.qualt option) list) = 
# 29 "grammar_parser_yacc.mly"
                                               ( (_1,_2) :: _3)
# 214 "grammar_parser_yacc.ml"
         in
        _menhir_goto_list_qual_ident _menhir_env _menhir_stack _menhir_s _v
    | MenhirState9 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        assert (not _menhir_env._menhir_error);
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOL ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _menhir_env = _menhir_discard _menhir_env in
            let _tok = _menhir_env._menhir_token in
            (match _tok with
            | EOF | EOL | IDENTIFIER _ ->
                let _menhir_stack = Obj.magic _menhir_stack in
                let ((_menhir_stack, _menhir_s, (_1 : (
# 8 "grammar_parser_yacc.mly"
      (string)
# 232 "grammar_parser_yacc.ml"
                ))), _, (_3 : ((string * Qualifiers.qualt option) list))) = _menhir_stack in
                let _4 = () in
                let _2 = () in
                let _v : (string * (string * Qualifiers.qualt option) list * string option) = 
# 49 "grammar_parser_yacc.mly"
                                               ( (_1,_3, None) )
# 239 "grammar_parser_yacc.ml"
                 in
                _menhir_goto_rule _menhir_env _menhir_stack _menhir_s _v
            | CODE _ ->
                let _menhir_stack = Obj.magic _menhir_stack in
                let x = () in
                let _v : (unit option) = 
# 116 "<standard.mly>"
    ( Some x )
# 248 "grammar_parser_yacc.ml"
                 in
                _menhir_goto_option_EOL_ _menhir_env _menhir_stack _v
            | _ ->
                assert (not _menhir_env._menhir_error);
                _menhir_env._menhir_error <- true;
                let _menhir_stack = Obj.magic _menhir_stack in
                let (_menhir_stack, _menhir_s, _) = _menhir_stack in
                _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
        | CODE _ ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _v : (unit option) = 
# 114 "<standard.mly>"
    ( None )
# 262 "grammar_parser_yacc.ml"
             in
            _menhir_goto_option_EOL_ _menhir_env _menhir_stack _v
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            let _menhir_stack = Obj.magic _menhir_stack in
            let (_menhir_stack, _menhir_s, _) = _menhir_stack in
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
    | _ ->
        _menhir_fail ()

and _menhir_goto_qualifier : _menhir_env -> 'ttv_tail -> (Qualifiers.qualt option) -> 'ttv_return =
  fun _menhir_env _menhir_stack _v ->
    let _menhir_stack = (_menhir_stack, _v) in
    let _menhir_stack = Obj.magic _menhir_stack in
    assert (not _menhir_env._menhir_error);
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | IDENTIFIER _v ->
        _menhir_run10 _menhir_env (Obj.magic _menhir_stack) MenhirState15 _v
    | CODE _ | EOL ->
        _menhir_reduce4 _menhir_env (Obj.magic _menhir_stack) MenhirState15
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState15

and _menhir_fail : unit -> 'a =
  fun () ->
    Printf.fprintf stderr "Internal failure -- please contact the parser generator's developers.\n%!";
    assert false

and _menhir_goto_rules : _menhir_env -> 'ttv_tail -> _menhir_state -> ((string * (string * Qualifiers.qualt option) list * string option) list) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    match _menhir_s with
    | MenhirState22 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s), _, (_2 : ((string * (string * Qualifiers.qualt option) list * string option) list))) = _menhir_stack in
        let _1 = () in
        let _v : ((string * (string * Qualifiers.qualt option) list * string option) list) = 
# 44 "grammar_parser_yacc.mly"
                    (_2)
# 307 "grammar_parser_yacc.ml"
         in
        _menhir_goto_rules _menhir_env _menhir_stack _menhir_s _v
    | MenhirState24 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s, (_1 : (string * (string * Qualifiers.qualt option) list * string option))), _, (_2 : ((string * (string * Qualifiers.qualt option) list * string option) list))) = _menhir_stack in
        let _v : ((string * (string * Qualifiers.qualt option) list * string option) list) = 
# 43 "grammar_parser_yacc.mly"
                     ( _1 :: _2 )
# 317 "grammar_parser_yacc.ml"
         in
        _menhir_goto_rules _menhir_env _menhir_stack _menhir_s _v
    | MenhirState7 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        assert (not _menhir_env._menhir_error);
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOF ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _menhir_stack = Obj.magic _menhir_stack in
            let ((_menhir_stack, _menhir_s), _, (_3 : ((string * (string * Qualifiers.qualt option) list * string option) list))) = _menhir_stack in
            let _4 = () in
            let _2 = () in
            let _1 = () in
            let _v : (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 335 "grammar_parser_yacc.ml"
            ) = 
# 19 "grammar_parser_yacc.mly"
                            ( ([],[],_3) )
# 339 "grammar_parser_yacc.ml"
             in
            _menhir_goto_main _menhir_env _menhir_stack _menhir_s _v
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            let _menhir_stack = Obj.magic _menhir_stack in
            let (_menhir_stack, _menhir_s, _) = _menhir_stack in
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
    | _ ->
        _menhir_fail ()

and _menhir_reduce4 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _v : ((string * Qualifiers.qualt option) list) = 
# 30 "grammar_parser_yacc.mly"
          ( [] )
# 356 "grammar_parser_yacc.ml"
     in
    _menhir_goto_list_qual_ident _menhir_env _menhir_stack _menhir_s _v

and _menhir_run10 : _menhir_env -> 'ttv_tail -> _menhir_state -> (
# 8 "grammar_parser_yacc.mly"
      (string)
# 363 "grammar_parser_yacc.ml"
) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | EXCLAMATION ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _menhir_stack = Obj.magic _menhir_stack in
        let _1 = () in
        let _v : (Qualifiers.qualt option) = 
# 38 "grammar_parser_yacc.mly"
                      ( Some (Ignore) )
# 378 "grammar_parser_yacc.ml"
         in
        _menhir_goto_qualifier _menhir_env _menhir_stack _v
    | HAT ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _menhir_stack = Obj.magic _menhir_stack in
        let _1 = () in
        let _v : (Qualifiers.qualt option) = 
# 35 "grammar_parser_yacc.mly"
              ( Some (Root) )
# 389 "grammar_parser_yacc.ml"
         in
        _menhir_goto_qualifier _menhir_env _menhir_stack _v
    | LISTHEAD ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _menhir_stack = Obj.magic _menhir_stack in
        let _1 = () in
        let _v : (Qualifiers.qualt option) = 
# 36 "grammar_parser_yacc.mly"
                   ( Some (ListHead) )
# 400 "grammar_parser_yacc.ml"
         in
        _menhir_goto_qualifier _menhir_env _menhir_stack _v
    | LISTTAIL ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _menhir_stack = Obj.magic _menhir_stack in
        let _1 = () in
        let _v : (Qualifiers.qualt option) = 
# 37 "grammar_parser_yacc.mly"
                   ( Some (ListTail) )
# 411 "grammar_parser_yacc.ml"
         in
        _menhir_goto_qualifier _menhir_env _menhir_stack _v
    | CODE _ | EOL | IDENTIFIER _ ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _v : (Qualifiers.qualt option) = 
# 39 "grammar_parser_yacc.mly"
          ( None )
# 419 "grammar_parser_yacc.ml"
         in
        _menhir_goto_qualifier _menhir_env _menhir_stack _v
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s

and _menhir_goto_list_ident : _menhir_env -> 'ttv_tail -> _menhir_state -> (string list) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    match _menhir_s with
    | MenhirState2 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s, (_1 : (
# 8 "grammar_parser_yacc.mly"
      (string)
# 439 "grammar_parser_yacc.ml"
        ))), _, (_2 : (string list))) = _menhir_stack in
        let _v : (string list) = 
# 24 "grammar_parser_yacc.mly"
                                ( _1 :: _2)
# 444 "grammar_parser_yacc.ml"
         in
        _menhir_goto_list_ident _menhir_env _menhir_stack _menhir_s _v
    | MenhirState1 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        assert (not _menhir_env._menhir_error);
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOL ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _menhir_env = _menhir_discard _menhir_env in
            let _tok = _menhir_env._menhir_token in
            (match _tok with
            | NT ->
                _menhir_run28 _menhir_env (Obj.magic _menhir_stack) MenhirState5
            | RULES ->
                _menhir_run6 _menhir_env (Obj.magic _menhir_stack) MenhirState5
            | TOK ->
                _menhir_run1 _menhir_env (Obj.magic _menhir_stack) MenhirState5
            | _ ->
                assert (not _menhir_env._menhir_error);
                _menhir_env._menhir_error <- true;
                _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState5)
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            let _menhir_stack = Obj.magic _menhir_stack in
            let (_menhir_stack, _menhir_s, _) = _menhir_stack in
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
    | MenhirState29 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        assert (not _menhir_env._menhir_error);
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOL ->
            let _menhir_stack = Obj.magic _menhir_stack in
            let _menhir_env = _menhir_discard _menhir_env in
            let _tok = _menhir_env._menhir_token in
            (match _tok with
            | NT ->
                _menhir_run28 _menhir_env (Obj.magic _menhir_stack) MenhirState31
            | RULES ->
                _menhir_run6 _menhir_env (Obj.magic _menhir_stack) MenhirState31
            | TOK ->
                _menhir_run1 _menhir_env (Obj.magic _menhir_stack) MenhirState31
            | _ ->
                assert (not _menhir_env._menhir_error);
                _menhir_env._menhir_error <- true;
                _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState31)
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            let _menhir_stack = Obj.magic _menhir_stack in
            let (_menhir_stack, _menhir_s, _) = _menhir_stack in
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s)
    | _ ->
        _menhir_fail ()

and _menhir_reduce19 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _v : ((string * (string * Qualifiers.qualt option) list * string option) list) = 
# 45 "grammar_parser_yacc.mly"
          ([])
# 507 "grammar_parser_yacc.ml"
     in
    _menhir_goto_rules _menhir_env _menhir_stack _menhir_s _v

and _menhir_run8 : _menhir_env -> 'ttv_tail -> _menhir_state -> (
# 8 "grammar_parser_yacc.mly"
      (string)
# 514 "grammar_parser_yacc.ml"
) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | ARROW ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | IDENTIFIER _v ->
            _menhir_run10 _menhir_env (Obj.magic _menhir_stack) MenhirState9 _v
        | CODE _ | EOL ->
            _menhir_reduce4 _menhir_env (Obj.magic _menhir_stack) MenhirState9
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState9)
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s

and _menhir_run22 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _menhir_stack = (_menhir_stack, _menhir_s) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | EOL ->
        _menhir_run22 _menhir_env (Obj.magic _menhir_stack) MenhirState22
    | IDENTIFIER _v ->
        _menhir_run8 _menhir_env (Obj.magic _menhir_stack) MenhirState22 _v
    | EOF ->
        _menhir_reduce19 _menhir_env (Obj.magic _menhir_stack) MenhirState22
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState22

and _menhir_reduce2 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _v : (string list) = 
# 25 "grammar_parser_yacc.mly"
          ( [] )
# 563 "grammar_parser_yacc.ml"
     in
    _menhir_goto_list_ident _menhir_env _menhir_stack _menhir_s _v

and _menhir_run2 : _menhir_env -> 'ttv_tail -> _menhir_state -> (
# 8 "grammar_parser_yacc.mly"
      (string)
# 570 "grammar_parser_yacc.ml"
) -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s _v ->
    let _menhir_stack = (_menhir_stack, _menhir_s, _v) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | IDENTIFIER _v ->
        _menhir_run2 _menhir_env (Obj.magic _menhir_stack) MenhirState2 _v
    | EOL ->
        _menhir_reduce2 _menhir_env (Obj.magic _menhir_stack) MenhirState2
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState2

and _menhir_errorcase : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    match _menhir_s with
    | MenhirState31 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState29 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s), _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState24 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState22 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState15 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let ((_menhir_stack, _menhir_s, _), _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState9 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState7 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState5 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState2 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s, _) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState1 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s
    | MenhirState0 ->
        let _menhir_stack = Obj.magic _menhir_stack in
        raise _eRR

and _menhir_run1 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _menhir_stack = (_menhir_stack, _menhir_s) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | IDENTIFIER _v ->
        _menhir_run2 _menhir_env (Obj.magic _menhir_stack) MenhirState1 _v
    | EOL ->
        _menhir_reduce2 _menhir_env (Obj.magic _menhir_stack) MenhirState1
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState1

and _menhir_run6 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _menhir_stack = (_menhir_stack, _menhir_s) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | EOL ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_env = _menhir_discard _menhir_env in
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | EOL ->
            _menhir_run22 _menhir_env (Obj.magic _menhir_stack) MenhirState7
        | IDENTIFIER _v ->
            _menhir_run8 _menhir_env (Obj.magic _menhir_stack) MenhirState7 _v
        | EOF ->
            _menhir_reduce19 _menhir_env (Obj.magic _menhir_stack) MenhirState7
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState7)
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s

and _menhir_run28 : _menhir_env -> 'ttv_tail -> _menhir_state -> 'ttv_return =
  fun _menhir_env _menhir_stack _menhir_s ->
    let _menhir_stack = (_menhir_stack, _menhir_s) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | TYP _v ->
        let _menhir_stack = Obj.magic _menhir_stack in
        let _menhir_stack = (_menhir_stack, _v) in
        let _menhir_env = _menhir_discard _menhir_env in
        let _tok = _menhir_env._menhir_token in
        (match _tok with
        | IDENTIFIER _v ->
            _menhir_run2 _menhir_env (Obj.magic _menhir_stack) MenhirState29 _v
        | EOL ->
            _menhir_reduce2 _menhir_env (Obj.magic _menhir_stack) MenhirState29
        | _ ->
            assert (not _menhir_env._menhir_error);
            _menhir_env._menhir_error <- true;
            _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState29)
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        let _menhir_stack = Obj.magic _menhir_stack in
        let (_menhir_stack, _menhir_s) = _menhir_stack in
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) _menhir_s

and _menhir_discard : _menhir_env -> _menhir_env =
  fun _menhir_env ->
    let lexer = _menhir_env._menhir_lexer in
    let lexbuf = _menhir_env._menhir_lexbuf in
    let _tok = lexer lexbuf in
    {
      _menhir_lexer = lexer;
      _menhir_lexbuf = lexbuf;
      _menhir_token = _tok;
      _menhir_error = false;
    }

and main : (Lexing.lexbuf -> token) -> Lexing.lexbuf -> (
# 12 "grammar_parser_yacc.mly"
      (string list * (string * string) list * (string * (string * Qualifiers.qualt option) list * string option) list)
# 718 "grammar_parser_yacc.ml"
) =
  fun lexer lexbuf ->
    let _menhir_env = let _tok = Obj.magic () in
    {
      _menhir_lexer = lexer;
      _menhir_lexbuf = lexbuf;
      _menhir_token = _tok;
      _menhir_error = false;
    } in
    Obj.magic (let _menhir_stack = ((), _menhir_env._menhir_lexbuf.Lexing.lex_curr_p) in
    let _menhir_env = _menhir_discard _menhir_env in
    let _tok = _menhir_env._menhir_token in
    match _tok with
    | NT ->
        _menhir_run28 _menhir_env (Obj.magic _menhir_stack) MenhirState0
    | RULES ->
        _menhir_run6 _menhir_env (Obj.magic _menhir_stack) MenhirState0
    | TOK ->
        _menhir_run1 _menhir_env (Obj.magic _menhir_stack) MenhirState0
    | _ ->
        assert (not _menhir_env._menhir_error);
        _menhir_env._menhir_error <- true;
        _menhir_errorcase _menhir_env (Obj.magic _menhir_stack) MenhirState0)

# 52 "grammar_parser_yacc.mly"
  

# 746 "grammar_parser_yacc.ml"

# 269 "<standard.mly>"
  

# 751 "grammar_parser_yacc.ml"
