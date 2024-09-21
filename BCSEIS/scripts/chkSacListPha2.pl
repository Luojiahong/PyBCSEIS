#!/usr/bin/perl -w
#Script:	chkSacListPha.pl
#Purpose:	To check the availability of phase arrival from a list of SAC files
#Author:	Wen-xuan(Wayne) Du
#Date:		January 8, 2003
#Last Modified:	March 19, 2003

$script	= "chkSacListPha2.pl";
$argc = $#ARGV + 1;
if ($argc != 4) {
	print "Usage: $script <sacList> <goodList0> <goodList1> <goodList2>\n";
	print "<sacList>:\ta list of SAC files to be checked\n";
	print "<goodList0>:\ta list of SAC files that have the P pick.\n";
	print "<goodList1>:\ta list of SAC files that have the S pick.\n";
	print "<goodList2>:\ta list of SAC files that have both the P&S picks.\n";
	print "\nThis script checks the availability of phase arrivals from a list of SAC files.\n";
	exit (-1);
}

# first get the command line arguments
$sacList = shift;
$goodList0 = shift;
$goodList1 = shift;
$goodList2 = shift;

print ">>> SAC files in '$sacList' will be checked for the P, S and P&S arrivals.\n"; 

# deal with list files
open (LIST, "<$sacList") or die "Can not open file '$sacList' for reading!: $!\n";
open (GOOD0, ">$goodList0") or die "Can not open file '$goodList0' for writing!: $!\n";
open (GOOD1, ">$goodList1") or die "Can not open file '$goodList1' for writing!: $!\n";
open (GOOD2, ">$goodList2") or die "Can not open file '$goodList2' for writing!: $!\n";

$count = 0;
$good0 = 0;
$good1 = 0;
$good2 = 0;
while ($sacFile=<LIST>) {
	$flag1	= 0;
	$flag2	= 0;
	$count++;
	chomp $sacFile;
	$str1 = substr($sacFile, 0, 1);
	if ($str1 eq '#') {
		next;	# event is commented out
	} else {
		# check the availability of the phase arrival 
		my $hdrVals = `getsachdr $sacFile a t0`;
		chomp $hdrVals;
		my ($parr, $sarr) = split('\s+',$hdrVals);
		$flag1	= 1 if ($parr != -12345.0);
		$flag2	= 1 if ($sarr != -12345.0);

		if ($flag1 == 1) {
			print GOOD0 "$sacFile\n";
			$good0++;
		}

		if ($flag2 == 1) {
			print GOOD1 "$sacFile\n";
			$good1++;
		}

		if ($flag1 == 1 && $flag2 == 1) {
			print GOOD2 "$sacFile\n";
			$good2++;
		}
	}	
}
close(LIST);
close(GOOD0);
close(GOOD1);
close(GOOD2);

print ">>> $good0 out of $count events have P arrivals and the list is saved in file '$goodList0'.\n";
print ">>> $good1 out of $count events have S arrivals and the list is saved in file '$goodList1'.\n";
print ">>> $good2 out of $count events have P&S arrivals and the list is saved in file '$goodList2'.\n";

