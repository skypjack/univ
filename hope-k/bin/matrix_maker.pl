#!/usr/bin/perl

use POSIX;

($ARGV[0] && $ARGV[1] && $ARGV[2]) || die "parametri: file_kernel file_target percentuale_test [numero_ripetizioni]\n";
$cnt = 0;

print "caricamento matrice kernel, ";
open FILE, "<", $ARGV[0] || die "ATTENZIONE: impossibile aprire $ARGV[0]\n";
$line = <FILE>; $dim = 0;
$line =~ s/.*tr([0-9]+)/$dim=$1/e;
print "dimensione matrice ipotizzata: $dim x $dim\n";
$cnt = floor($dim * 10 / 100);
@kernel = ();
print "caricamento ";
while($line = <FILE>) {
    if(!$cnt--) { print "."; $cnt = floor($dim * 10 / 100); }
    $line =~ s/tr[0-9]+\s*//;
    @line = split " ", $line;
    push @kernel, @line;
}
print ".\n";
($dim == (@kernel / $dim)) || die "matrice non quadrata: $dim x ", $dim = (@kernel / $dim), "\n";

print "caricamento target:\n";
open FILE, "<", $ARGV[1] || die "ATTENZIONE: impossibile aprire $ARGV[1]\n";
$cnt = floor($dim * 10 / 100);
@target = ();
print "caricamento ";
while($line = <FILE>) {
    if(!$cnt--) { print "."; $cnt = floor($dim * 10 / 100); }
    if($line =~ /[+-]1/) { push @target, 0+$line; }
    else { print "\ndrop: $line\n"; }
}
print ".\n";
($dim == @target) || die "numero target errato: attesi ", $dim, ", rilevati ", $dim = @target, "\n";

$traindim = floor($dim * $ARGV[2] / 100);
$testdim = $dim - $traindim;
$rep = 1;
if($ARGV[3]) {$rep = $ARGV[3];}

print "preparazione matrici:\n";
while($rep--) {
    print "\tstep $rep:\n";
    print "\t\tmescolamento\n";
    $corg = 0;
    $cdst = 0;
    $cnt = $ARGV[2];
    while($cnt--) {
	$corg = floor(rand($dim));
	$cdst = floor(rand($dim));
	while ($cdst == $corg) {
	    $cdst = floor(rand($dim));
	}
	for($iter = 0; $iter < $dim; $iter++) {
	    $swap = $kernel[$dim*$iter+$corg];
	    $kernel[$dim*$iter+$corg] = $kernel[$dim*$iter+$cdst];
	    $kernel[$dim*$iter+$cdst] = $swap;
	}
	for($iter = 0; $iter < $dim; $iter++) {
	    $swap = $kernel[$dim*$corg+$iter];
	    $kernel[$dim*$corg+$iter] = $kernel[$dim*$cdst+$iter];
	    $kernel[$dim*$cdst+$iter] = $swap;
	}
	$swap = $target[$corg];
	$target[$corg] = $target[$cdst];
	$target[$cdst] = $swap;
    }
    print "\t\tgenerazione indici di partizione\n";
    @mark = ();
    $testcnt = $testdim;
    $traincnt = $traindim;
    while($traincnt && $testcnt) {
	if(rand() > rand()) {
	    push @mark, 1;
	    $traincnt--;
	} else {
	    push @mark, -1;
	    $testcnt--;
	}
    }
    while($traincnt) {
	push @mark, 1;
	$traincnt--;
    }
    while($testcnt) {
	push @mark, -1;
	$testcnt--;
    }
    print "\t\tscrittura file\n";
    $fold = "fold$rep";
    mkdir "$fold" || die "ATTENZIONE: impossibile creare la cartella $fold\n";
    open TRAIN, ">", "$fold/train$rep" || die "ATTENZIONE: impossibile aprire $fold/train$rep\n";
    open TEST, ">", "$fold/test$rep" || die "ATTENZIONE: impossibile aprire $fold/test$rep\n";
    open TRAIN_TARGET, ">", "$fold/target_train$rep" || die "ATTENZIONE: impossibile aprire $fold/train$rep_target\n";
    open TEST_TARGET, ">", "$fold/target_test$rep" || die "ATTENZIONE: impossibile aprire $fold/test$rep_target\n";
    print TRAIN "K<x,y>";
    print TEST "K<x,y>";
    print TRAIN_TARGET "label\tclass\n";
    print TEST_TARGET "label\tclass\n";
    for($iter = 0; $iter < $traindim; $iter++) {
	print TRAIN "\ttr", $iter+1;
	print TEST "\ttr", $iter+1;
    }
    print TRAIN "\n";
    print TEST "\n";
    $testcnt = $testdim;
    $traincnt = $traindim;
    for($iter = 0; $iter < $dim; $iter++) {
	if($mark[$iter] == 1) {
	    print TRAIN_TARGET "tr", $traindim-$traincnt+1, "\t";
	    if($target[$iter] < 0) { print TRAIN_TARGET $target[$iter], "\n"; }
	    else { print TRAIN_TARGET "+", $target[$iter], "\n"; }
	    print TRAIN "tr", $traindim-$traincnt+1;
	    $traincnt--;
	    for($step = 0; $step < $dim; $step++) {
		if($mark[$step] == 1) {
		    print TRAIN "\t", $kernel[$dim*$iter+$step];
		}		
	    }
	    print TRAIN "\n";
	} else {
	    print TEST_TARGET "tr", $testdim-$testcnt+1, "\t";
	    if($target[$iter] < 0) { print TEST_TARGET $target[$iter], "\n"; }
	    else { print TEST_TARGET "+", $target[$iter], "\n"; }
	    print TEST "tr", $testdim-$testcnt+1;
	    $testcnt--;
	    for($step = 0; $step < $dim; $step++) {
		if($mark[$step] == 1) {
		    print TEST "\t", $kernel[$dim*$iter+$step];
		}		
	    }
	    print TEST "\n";
	}
    }
}
