#!/usr/bin/perl -w
#Script:	chkProjCode.pl
#Purpose:	To provide a centralized check on 'projCode' input
#Author:	Wen-xuan(Wayne) Du
#Date:		January 30, 2003
#Last Modified:	January 30, 2003

$script	= "chkProjCode.pl";
$argc	= $#ARGV+1;
if ($argc != 1) {
	print "Usage:\t\t$script <projCode>\n";
	print "<projCode>:\tproject code (currently supported are 'ctbt', 'nz' and 'pf'; case insensitive)\n";
	print "Output:\t\t-1 for incorrect 'projCode'; (projnum, basedir) if correct\n\n";
	print "This program check the correctness of 'projCode'.\n";
	exit (-1);
}
$projCode = shift;

## get the name of the operating system
if ($projCode eq 'WY' || $projCode eq 'wy') {
	print "1 /mnt/e/Python/REAL-master/REAL-master/demo_real/BCSEIS\n";
} elsif ($projCode eq 'TS' || $projCode eq 'ts') {
	print "2 /mnt/e/XMLY\n";
} elsif ($projCode eq 'nz2') {
        print "2 /u91/dxw/NewZealand\n";
} elsif ($projCode eq 'PF' || $projCode eq 'pf') {
	print "3 /home/dxw/Parkfield\n";
} elsif ($projCode eq 'UH' || $projCode eq 'uh') {
	print "4 /u12/dxw/WorkshopData/Martin\n";
} elsif ($projCode eq 'TVZ' || $projCode eq 'tvz') {
        print "5 /home/dxw/TVZ\n";
} else {
	print "-1\n";
}
