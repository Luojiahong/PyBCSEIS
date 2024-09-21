#!/usr/bin/perl -w
#Script:	calcCCEvInfo3.pl
#Version:	3.0
#Author:	Wen-xuan(Wayne) Du
#Date:		Febuary 10, 2003
#Last Modified:	September 11, 2003
#Major Changes:	use 'getStaListInDir2.pl' to get the station list

# take care of the path info to the library file 'subArray.pl'
use lib "$ENV{HOME}/PerlLib";
require "subArray.pl";

$script	= "calcCCEvInfo3.pl";
$argc	= $#ARGV+1;
$warnFile = "ccEvInfo.warn";
$ccEvInfoFile = "ccEvInfo.dat";
if ($argc != 2 && $argc != 3) {
	print "Usage:\t\t$script <projCode> <evIdList> [staList]\n";
	print "<projCode>:\tproject code;\n";
	print "<evIdList>:\tevent id list;\n";
	print "[staList]:\tstation list to check the pick availability;\n";
	print "\nThis program calculates the event info for CC purpose.\n";
	print "'$ccEvInfoFile' stores the output\n";
	print "'$warnFile' contains the warning messages.\n\n";
	print "Version:\t3.0 [This version adds in the 'staList' option to confine the checking range]\n";
	exit (-1);
}

## deal with command line arguement inputs
$projCode = shift;
$evIdList = shift;
$staList = '';
$staList = shift if ($argc == 3);
## check the 'projCode'
my $chkResult = `chkProjCode.pl $projCode`;
chomp $chkResult;
($projnum,$base) = split('\s+',$chkResult);
if ($projnum == -1) {
	print ">>> Error: incorrect 'projCode' in command line argument list!\n";
	exit (-1);
}
$evInfoFile = "$base/Events/EventInfo/events.catalog";
$idDirFile = "$base/Events/IdDirFiles/iddir.dat";

## list of stations to check
@stas = ();
if ($argc == 3) {
	open(STA,"<$staList") or die "Can not open file '$staList' for reading: $!\n";
	while($sta=<STA>) {
		chomp $sta;
		push @stas, $sta;
	}
	close(STA);
}

open(EVLIST,"<$evIdList") or die "Can not open file '$evIdList' for reading: $!\n";
open(WARN,">$warnFile") or die "Can not open file '$warnFile' for writing: $!\n";
open(EVINFO,">$ccEvInfoFile") or die "Can not open file '$ccEvInfoFile' for writing: $!\n";

$evNum = 1;
while ($evId=<EVLIST>) {
	print ">>> Evaluating event #$evNum ";
	chomp $evId;
	## get the event data directory
	$dir = `getDirFromId2.pl $idDirFile $evId`;
	chomp $dir;
	$dataDir = "$base/Events/Data/$dir";
	$evLine = `grep '^$evId' $evInfoFile`;
	chomp $evLine;
	if ($evLine eq '') {
		print WARN ">>> '$evId': No event info available.\n";
		print "-- deleted (no event info available)";
		next;
	}
	## get the catalog event location info
	my ($id0,$oyear,$omon,$oday,$ohr,$omin,$osec,$elat,$elon,$edep,$mag) = split('\s+',$evLine);
	## get the station list
	my $pStaList = `getStaListInDir2.pl $dataDir 0`;
	my $sStaList = `getStaListInDir2.pl $dataDir 1`;
	chomp ($pStaList, $sStaList);
	@goodList1 = ();
	if ($pStaList eq '') {
		print WARN ">>> '$evId': No station available with P pick\n";
		print "-- deleted (no station available with P pick) ";
	} else {
		my @pList = split('\s+',$pStaList);
		if ($argc == 3) {
			foreach $sta (@pList) {
				my $ff = &isInArray($sta,@stas);
				push @goodList1, $sta if ($ff == 1);
			}
		} else {
			@goodList1 = @pList;
		}
	}
	$goodNum1 = $#goodList1 + 1;
	@goodList2 = ();
	if ($sStaList eq '') {
		print WARN ">>> '$evId': No station available with S pick\n";
		print "-- deleted (no station available with S pick)";
	} else {
		my @sList = split('\s+',$sStaList);
		if ($argc == 3) {
			foreach $sta (@sList) {
				my $ff = &isInArray($sta,@stas);
				push @goodList2, $sta if ($ff == 1);
			}
		} else {
			@goodList2 = @sList;
		}
	}
	$goodNum2 = $#goodList2 + 1;
	printf EVINFO "# $evId %8.4f %9.4f %7.3f $dir %d %d\n",$elat,$elon,$edep,$goodNum1,$goodNum2;
	if ($goodNum1 > 0) {
		foreach $sta (@goodList1) {
			print EVINFO "$sta\n";
		}
	}
	if ($goodNum2 > 0) {
		foreach $sta (@goodList2) {
			print EVINFO "$sta\n";
		}
	}
	print "\n";
	$evNum++;
}
close(EVLIST);
close(EVINFO);
print ">>> Done! results are saved in '$ccEvInfoFile' and warning messages in '$warnFile'.\n";
