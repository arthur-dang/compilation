#!/bin/bash

DIR=$1
TP=$2
say_ok(){
    echo -ne "\t\x1b[32mOK\x1b[0m"
}

say_ko(){
    echo -ne "\t\x1b[31mKO\x1b[0m"
}

say_ok_or_ko(){
    R=$1
	  if [[ "0" -eq $R ]]; then
        say_ok
	  else
		    say_ko
    fi
}

test_comp(){
    F=$1
    P1=$2
    P2=$3
    LANG=$4
	  OUT=`$DIR/supecomp $F -$LANG-run $P1 $P2 | diff "$F.expect_${P1}_${P2}" ->/dev/null`
    say_ok_or_ko "$?"

}
test_lexer(){
    F=$1
    OUT=`$DIR/supecomp $F | diff "$F.lexer_expect" ->/dev/null`
    say_ok_or_ko "$?"
}
test_parser(){
    f=$1
    out=`expr/supecomp $f -ast -`
    res="$?"
    echo $out | dot -Tpng -o tests/pngs/$(basename $f).png 2>/dev/null
    say_ok_or_ko "$res"
}
test_asm(){
    F=$1
    P1=$2
    P2=$3
    TARGET=$4
    OPTS=$5
    EXE=`mktemp`
    chmod u+x $EXE
    $DIR/supecomp $F -target $TARGET $OPTS -libdir $DIR/runtime_$TARGET/ -o $EXE 1>/dev/null 2>&1
    ($EXE $P1 $P2 | diff "$F.expect_${P1}_${P2}" -)  1>/dev/null 2>&1
    say_ok_or_ko "$?"
    rm $EXE
}

test_x86_64_asm(){
    test_asm $1 $2 $3 x86-64 ""
}
test_x86_64_asm2(){
    test_asm $1 $2 $3 x86-64 "-clever-regalloc 9"
}
test_x86_32_asm(){
    test_asm $1 $2 $3 x86-32 ""
}
test_riscv_asm(){
    test_asm $1 $2 $3 riscv ""
}

test_riscv_asm2(){
    test_asm $1 $2 $3 riscv "-clever-regalloc 15"
}

printf "%-30s: " "File"
printf "\tlexer"
if [ "$TP" -ge 2 ]; then
    printf "\tparser"
    if [ "$TP" -ge 3 ]; then
        printf "\telang\telang"
        if [ "$TP" -ge 4 ]; then
            printf "\tcfg\tcfg"
            if [ "$TP" -ge 5 ]; then
                printf "\trtl\trtl"
                if [ "$TP" -ge 6 ]; then
                    printf "\tx86-32\tx86-32"
                    printf "\tx86-64\tx86-64"
                    printf "\tx86-64\tx86-64"
                    printf "\trv\trv"
                    printf "\trv\trv"
                fi
            fi
        fi
    fi
fi
echo ""


rm -rf tests/pngs
mkdir tests/pngs

for f in tests/*.e; do
    printf "%-30s: " $f
    test_lexer "$f"
    if [ "$TP" -ge 2 ]; then
        test_parser "$f"
        if [ "$TP" -ge 3 ]; then
	          test_comp "$f" 1 5 "elang"
            test_comp "$f" 12 14 "elang"
            if [ "$TP" -ge 4 ]; then
                test_comp "$f" 1 5 "cfg"
                test_comp "$f" 12 14 "cfg"
                if [ "$TP" -ge 5 ]; then
                    test_comp "$f" 1 5 "rtl"
                    test_comp "$f" 12 14 "rtl"
                    if [ "$TP" -ge 6 ]; then
                        test_x86_32_asm "$f" 12 14
                        test_x86_32_asm "$f" 1 5
                        test_x86_64_asm "$f" 12 14
                        test_x86_64_asm "$f" 1 5
                        test_x86_64_asm2 "$f" 12 14
                        test_x86_64_asm2 "$f" 1 5
                        test_riscv_asm "$f" 12 14
                        test_riscv_asm "$f" 1 5
                        test_riscv_asm2 "$f" 12 14
                        test_riscv_asm2 "$f" 1 5
                    fi
                fi
            fi
        fi
    fi
    echo ""
done



