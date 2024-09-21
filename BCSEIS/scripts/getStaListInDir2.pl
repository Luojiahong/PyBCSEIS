#!/usr/bin/perl -w
#Script:	getStaListInDir2.pl
#Purpose:	Get the list of station names in a data directory
#Version:	2.0
#Major Changes:	Also check for event origin time when checking for phase arrivals in the SAC header
#Author:	Wen-xuan(Wayne) Du
#Date:		January 15, 2003
#Last Modified:	July 1st, 2003

# take care of the path info to the library file 'subArray.pl'
use lib "$ENV{HOME}/PerlLib";
require "subArray.pl";

$script	= "getStaListInDir2.pl";
$argc	= $#ARGV+1;
if ($argc != 1 && $argc != 2) {
	print "Usage:\t\t$script <dirName> [phaCode]\n";
	print "<dirName>:\tdata directory name to work on\n";
	print "[phaCode]:\t0 - P; 1 - S; 2 - P&S\n";
	print "Version:\t2.0; This version checks for event origin time also\n";
	print "\nThis program produces a list of station names in a data directory with specified phase code.\n";
	exit (-1);
}

## deal with command line arguments
$dirName = shift;
$phaCode = shift if ($argc == 2);

@sacFiles = <$dirName/*Z.SAC>;
@staList = ();
for ($i = 0; $i <= $#sacFiles; $i++) {
	$sacFile = $sacFiles[$i];
	@array = split('/', $sacFile);	# separate the directory and file name
	$sacName = $array[$#array];
	my ($sta,$comp,$sac)	= split('\.',$sacName);
	if ($sta ne 'filt') {	# not a filtered SAC file
		my $good = 1;
		if ($argc == 2) {
			my $flag1 = 0;
			my $flag2 = 0;
			if ($phaCode == 0) {
				$parr = `getsachdr $sacFile a`;
				chomp $parr;
				$flag1 = 1 if ($parr != -12345.0);
				$good = 0 if ($flag1 == 0);
			} elsif ($phaCode == 1) {
				$sarr = `getsachdr $sacFile t0`;
				chomp $sarr;
				$flag2 = 1 if ($sarr != -12345.0);
				$good = 0 if ($flag2 == 0);
			} else {
				my $arrLine = `getsachdr $sacFile a t0`;
				chomp $arrLine;
				($parr,$sarr) = split('\s+',$arrLine);
				$flag1 = 1 if ($parr != -12345.0);
				$flag2 = 1 if ($sarr != -12345.0);
				$good = 0 if ($flag1 == 0 || $flag2 == 0);
			}
			my $oarr = `getsachdr $sacFile o`;
			chomp $oarr;
			$good = 0 if ($oarr == -12345.0);
		}
		if ($good == 1) {
			my $ff	= &isInArray($sta, @staList);
			push @staList, $sta if ($ff == 0);
		}
	}
}
@staList = sort @staList;
print "@staList\n";
