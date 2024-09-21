#!/usr/bin/perl -w
#Script:	getDtFromCC.pl
#Author:	Wen-xuan(Wayne) Du
#Date:		February 5, 2003
#Last Modified:	September 16, 2003

# take care of the path info to the library file 'subArray.pl'
use lib "$ENV{HOME}/PerlLib";
require "subArray.pl";

use FileHandle;

$script = "getDtFromCC.pl";
$argc = $#ARGV+1;
if ($argc != 8) {
	print "Usage:\t\t$script <option> <ccDirList> <lCCLim> <cCCLim> <uCCLim> <diffLim> <evIdList> <dtFile>\n";
	print "<option>:\t0 - no limit control over CC-BS lag difference;\n";
	print "         \t1 - CC-BS lag difference 1 || 2 <= diffLim;\n";
	print "         \t2 - CC-BS lag difference 1 <= diffLim;\n";
	print "         \t3 - CC-BS lag difference 1 && 2 <= diffLim;\n";
	print "<ccDirList>:\tlist of directory names where CC results are kept;\n";
	print "<lCCLim>:\tlower CC limit\n";
	print "<cCCLim>:\tcenter CC limit;\n";
	print "<uCCLim>:\tupper CC limit;\n";
	print "<diffLim>:\tlimit in CC-BS lag difference;\n";
	print "<evIdList>:\tevent id list;\n";
	print "<dtFile>:\toutput dt.cc file;\n";
	exit (-1);
}

## deal with command line inputs
$option = shift;
if ($option != 0 && $option != 1 && $option != 2 && $option != 3) {
	print ">>> Error: incorrect 'option' in command line: should be 0, 1, 2 or 3!\n";
	exit (-1);
}
$ccDirList = shift;
$lCCLim = shift;
$cCCLim = shift;
$uCCLim = shift;
$diffLim = shift;
$evIdList = shift;
$dtFile = shift;

$DTCC = new FileHandle ">$dtFile";
die "Can not open file '$dtFile' for writing: $!\n" if (! defined $DTCC);

## deal with the ccDirList
open(CCDIR,"<$ccDirList") or die "Can not open file '$ccDirList' for reading: $!\n";
@ccDirs = ();
while($ccDir = <CCDIR>) {
	chomp $ccDir;
	push @ccDirs, $ccDir;
}
close(CCDIR);

## deal with the event id list
open(ID,"<$evIdList") or die "Can not open file '$evIdList' for reading: $!\n";
@idArr = ();
while($evId = <ID>) {
        chomp $evId;
        push @idArr, $evId;
}
close(ID);
$idNum = $#idArr+1;

## deal with CC files
for (my $i = 0; $i < $idNum; $i++) {
	my $evId = $idArr[$i];
	$ii = $i+1;
	print ">>> processing event # $ii: $evId\n";
	for (my $j = 0; $j <= $#ccDirs; $j++) {
		my $ccDir = $ccDirs[$j];
		my $ccFile = "$ccDir/${evId}.CC";
		next if (!-e $ccFile);
                open(CC,"<$ccFile") or die "Can not open '$ccFile' for reading: $!\n";
		my %ccInfo = ();
		my $maxCC = 0.0;
		my $evId2 = '';
		my $num = 0;
		while ($ccLine = <CC>) {
			chomp $ccLine;
			my ($tmp1, $tmp2, $tmp3) = split('\s+', $ccLine);
			if ($tmp1 eq '#') {
				if ($evId2 ne '') {
					$ccInfo{$evId2}->[0][0] = $maxCC;
					$ccInfo{$evId2}->[0][1] = $num;
				}
				$evId2 = $tmp3;
				$maxCC = 0.0;
				$num = 0;
			} else {
				$num++;
				my ($sta,$ccdt,$corv,$phaName,$ccLag1,$bsLag1,$bsLag2,$sublag,$sigma) = split('\s+',$ccLine);
				$maxCC = $corv if ($corv > $maxCC);
				$ccInfo{$evId2}->[$num][0] = $sta;
				$ccInfo{$evId2}->[$num][1] = $ccdt;
				$ccInfo{$evId2}->[$num][2] = $corv;
				$ccInfo{$evId2}->[$num][3] = $phaName;
				$ccInfo{$evId2}->[$num][4] = $ccLag1;
				$ccInfo{$evId2}->[$num][5] = $bsLag1;
				$ccInfo{$evId2}->[$num][6] = $bsLag2;
			}
		}
		close(CC);
		$ccInfo{$evId2}->[0][0] = $maxCC;
		$ccInfo{$evId2}->[0][1] = $num;

		foreach $evId2 (sort keys %ccInfo) {
			my $ff = &isInArray($evId2, @idArr);
			next if ($ff == 0);
			my $bsFlag = 0;	## cCCLim <= maxCC < uCCLim; 1 if maxCC >= uCCLim
			my $maxCC = $ccInfo{$evId2}->[0][0];
			my $num = $ccInfo{$evId2}->[0][1];
			next if ($maxCC < $cCCLim);
			$bsFlag = 1 if ($maxCC >= $uCCLim);
			my @goodCCInfo = ();
			for (my $j = 1; $j <= $num; $j++) {
				my $sta = $ccInfo{$evId2}->[$j][0];
				my $ccdt = $ccInfo{$evId2}->[$j][1];
				my $corv = $ccInfo{$evId2}->[$j][2];
				my $phaName = $ccInfo{$evId2}->[$j][3];
				my $ccLag1 = $ccInfo{$evId2}->[$j][4];
				my $bsLag1 = $ccInfo{$evId2}->[$j][5];
				my $bsLag2 = $ccInfo{$evId2}->[$j][6];
				my $lagDiff1 = abs($ccLag1 - $bsLag1);  # would be huge if $bsLag1 == -999 (error)
				my $lagDiff2 = abs($ccLag1 - $bsLag2);  # would be huge if $bsLag2 == -999 (error)
				my $outLine = "$sta\t$ccdt\t$corv\t$phaName";
				if ($option == 0) {
					push @goodCCInfo, $outLine if ($corv >= $cCCLim);
				} else {
					if ($bsFlag == 1) {
						next if ($corv < $lCCLim);
					} else {
						next if ($corv < $cCCLim);
					}
					if ($option == 1) {
						push @goodCCInfo, $outLine if ($lagDiff1 <= $diffLim || $lagDiff2 <= $diffLim);
					} elsif ($option == 2) {
						push @goodCCInfo, $outLine if ($lagDiff1 <= $diffLim);
					} elsif ($option == 3) {
						push @goodCCInfo, $outLine if ($lagDiff1 <= $diffLim && $lagDiff2 <= $diffLim);
					}
				}
			}
			if (@goodCCInfo > 0) {
				print $DTCC "# $evId $evId2 0.0\n";
				for (my $i = 0; $i <= $#goodCCInfo; $i++) {
					print $DTCC "$goodCCInfo[$i]\n";
				}
			}	
		}
	}
}
$DTCC->close;
