#!/usr/bin/perl
$i=0;
@a=NULL;
while(<>) {
    s/FOLD.*//g;
    s/Corretti.*//g;
    s/Totali.*//g;
    s/Accuratezza: //g;
    s/^\n$//g;
    s/\s+//g;
    if(/.*[0-9]+/) {
	@a[$i]=0+$_;
	$i++;
	print "@a[$i-1]\n";
    }
}
print "\n\n";
$i/=2;
$acc=0;
$max=0;
for($k=0;$k<$i;$k++) {
    if(@a[$k]>@a[$k+$i]) {
	$max=@a[$k];
    } else {
	$max=@a[$k+$i];
    }
    $acc+=$max;
    $max=~s/(0\.)([0-9])([0-9]?)([0-9]?)([0-9]?).*/($2*10+$3)+$4\/10+($5<5?0:0.1)/ge;
    $max=~s/^([0-9]+)$/\1.0/g;
    print "fold$k: $max%\n";
}
$acc/=$i;
$acc=~s/(0\.)([0-9])([0-9]?)([0-9]?)([0-9]?).*/($2*10+$3)+$4\/10+($5<5?0:0.1)/ge;
$acc=~s/^([0-9]+)$/\1.0/g;
print "\n:: $acc%\n";
