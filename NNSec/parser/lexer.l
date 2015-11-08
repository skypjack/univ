/**
 * Lexer class. Objects of this type tokenizes an information streams.
 */

package nnsec.parser;
import java_cup.runtime.*;

%%

%class Lexer
%public
%unicode
%cupsym Sym
%cup

nl		=	[\r|\n|\r\n]*
ws		=	[ \t\f]*
wtok		=	{nl} | {ws}
equal		=	{ws}"="{ws}
sfloat		=	["+"|"-"]?[0-9]+"."[0-9]*
integer		=	[0-9]+
string		=	[a-zA-Z0-9 ]+
lbracket	=	{wtok}"{"{wtok}
rbracket	=	{wtok}"}"{wtok}
endterm		=	{wtok}";"{wtok}

%state		NET_SEC
%state		I_NODE_SEC
%state		H_NODE_SEC
%state		O_NODE_SEC
%state		LINK_SEC

%%

<YYINITIAL> {
	"net"{lbracket}				{
							yybegin(NET_SEC);
							return new java_cup.runtime.Symbol(Sym.NET_SEC);
						}
}

<NET_SEC> {
	"name"{equal}				{ return new java_cup.runtime.Symbol(Sym.NAME); }
	"description"{equal}			{ return new java_cup.runtime.Symbol(Sym.DESC); }
	"actv"{equal}				{ return new java_cup.runtime.Symbol(Sym.ACTVF); }
	"out"{equal}				{ return new java_cup.runtime.Symbol(Sym.OUTF); }
	"onode"{lbracket}			{
							yybegin(O_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.O_NODE_SEC);
						}
	"inode"{lbracket}			{
							yybegin(I_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.I_NODE_SEC);
						}
	"hnode"{lbracket}			{
							yybegin(H_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.H_NODE_SEC);
						}
	"output"{lbracket}			{
							yybegin(O_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.O_NODE_SEC);
						}
	"input"{lbracket}			{
							yybegin(I_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.I_NODE_SEC);
						}
	"hidden"{lbracket}			{
							yybegin(H_NODE_SEC);
							return new java_cup.runtime.Symbol(Sym.H_NODE_SEC);
						}
	"link"{lbracket}			{
							yybegin(LINK_SEC);
							return new java_cup.runtime.Symbol(Sym.LINK_SEC);
						}
	{string}				{ return new java_cup.runtime.Symbol(Sym.STRING, new String(yytext())); }
	{endterm}				{ return new java_cup.runtime.Symbol(Sym.END_TERM); }
	{rbracket}				{
							yybegin(YYINITIAL);
							return new java_cup.runtime.Symbol(Sym.NET_SEC_END);
						}
}

<I_NODE_SEC> {
	"id"{equal}				{ return new java_cup.runtime.Symbol(Sym.NODE_ID); }
	{integer}				{ return new java_cup.runtime.Symbol(Sym.ID_VALUE, new Integer(yytext())); }
	{endterm}				{ return new java_cup.runtime.Symbol(Sym.END_TERM); }
	{rbracket}				{
							yybegin(NET_SEC);
							return new java_cup.runtime.Symbol(Sym.NODE_SEC_END);
						}
}

<H_NODE_SEC> {
	"id"{equal}				{ return new java_cup.runtime.Symbol(Sym.NODE_ID); }
	"actv"{equal}				{ return new java_cup.runtime.Symbol(Sym.ACTVF); }
	"out"{equal}				{ return new java_cup.runtime.Symbol(Sym.OUTF); }
	"group"{equal}				{ return new java_cup.runtime.Symbol(Sym.GROUP_ID); }
	"layer"{equal}				{ return new java_cup.runtime.Symbol(Sym.GROUP_ID); }
	"threshold"{equal}			{ return new java_cup.runtime.Symbol(Sym.THRESHOLD); }
	{sfloat}				{ return new java_cup.runtime.Symbol(Sym.THRESHOLD_VALUE, new Double(yytext())); }
	{integer}				{ return new java_cup.runtime.Symbol(Sym.ID_VALUE, new Integer(yytext())); }
	{string}				{ return new java_cup.runtime.Symbol(Sym.STRING, new String(yytext())); }
	{endterm}				{ return new java_cup.runtime.Symbol(Sym.END_TERM); }
	{rbracket}				{
							yybegin(NET_SEC);
							return new java_cup.runtime.Symbol(Sym.NODE_SEC_END);
						}
}

<O_NODE_SEC> {
	"id"{equal}				{ return new java_cup.runtime.Symbol(Sym.NODE_ID); }
	"actv"{equal}				{ return new java_cup.runtime.Symbol(Sym.ACTVF); }
	"out"{equal}				{ return new java_cup.runtime.Symbol(Sym.OUTF); }
	"threshold"{equal}			{ return new java_cup.runtime.Symbol(Sym.THRESHOLD); }
	{sfloat}				{ return new java_cup.runtime.Symbol(Sym.THRESHOLD_VALUE, new Double(yytext())); }
	{integer}				{ return new java_cup.runtime.Symbol(Sym.ID_VALUE, new Integer(yytext())); }
	{string}				{ return new java_cup.runtime.Symbol(Sym.STRING, new String(yytext())); }
	{endterm}				{ return new java_cup.runtime.Symbol(Sym.END_TERM); }
	{rbracket}				{
							yybegin(NET_SEC);
							return new java_cup.runtime.Symbol(Sym.NODE_SEC_END);
						}
}

<LINK_SEC> {
	"hnode"{equal}				{ return new java_cup.runtime.Symbol(Sym.LINK_INPUT_NODE_ID); }
	"tnode"{equal}				{ return new java_cup.runtime.Symbol(Sym.LINK_OUTPUT_NODE_ID); }
	"to"{equal}				{ return new java_cup.runtime.Symbol(Sym.LINK_INPUT_NODE_ID); }
	"from"{equal}				{ return new java_cup.runtime.Symbol(Sym.LINK_OUTPUT_NODE_ID); }
	"weight"{equal}				{ return new java_cup.runtime.Symbol(Sym.LINK_WEIGHT); }
	{sfloat}				{ return new java_cup.runtime.Symbol(Sym.WEIGHT_VALUE, new Double(yytext())); }
	{integer}				{ return new java_cup.runtime.Symbol(Sym.NODE_ID_VALUE, new Integer(yytext())); }
	{endterm}				{ return new java_cup.runtime.Symbol(Sym.END_TERM); }
	{rbracket}				{
							yybegin(NET_SEC);
							return new java_cup.runtime.Symbol(Sym.LINK_SEC_END);
						}
}

.						{}

