#!/bin/sh

SRCDIR="nnsec/parser"
PARSERDIR="parser"
LEXER="$PARSERDIR/lexer.l"
PARSER="$PARSERDIR/parser.cup"

echo -n "Building parser "
echo -n "."
java JFlex.Main $LEXER
echo -n "."
java java_cup.Main -package nnsec -parser Parser -symbols Sym $PARSER
echo -n "."
mv $PARSERDIR/*.java $SRCDIR
echo -n "."
mv ./*.java $SRCDIR
echo "."
