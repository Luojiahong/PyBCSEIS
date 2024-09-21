#!/usr/bin/perl -w
#Script:	filtSacList2.pl
#Purpose:	To bandpass filter a list of SAC files and output a list of filtered SAC files
#Author:	Wen-xuan(Wayne) Du
#Date:		January 7, 2003
#Last Modified:	February 14, 2003
#Major changes:	This version allows the user to control the number of poles and passes

$script	= "filtSacList2.pl";
$cycle	= 20;
if ($#ARGV != 4) {
	print "Usage:\t\t$script <listFile> <lf> <hf> <poleNum> <passNum>\n";
 	print "<listFile>:\tfile containing a list of SAC files to be filtered\n";
	print "<lf>:\t\tlow frequency for filtering\n";
	print "<hf>:\t\thigh frequency for filtering\n";
	print "<poleNum>:\tnumber of poles;\n";
	print "<passNum>:\tnumber of passes;\n";
	print "\nThis program bandpass filters a list of SAC files and output a list of filtered SAC files.\n";
	exit (-1);
}

## processing the command line arguments
$listFile 	= shift;
$lf		= shift;
$hf		= shift;
$poleNum	= shift;
$passNum	= shift;
$filtFile	= "filt.$listFile";	# output file containing a list of filtered SAC files
print ">>> SAC files in '$listFile' will be bandpass filtered between $lf and $hf Hz with $poleNum poles and $passNum passes...\n";

open(LIST, "<$listFile") or die "Can not open list file '$listFile' for reading: $!\n";
open(FILT, ">$filtFile") or die "Can not open filtered file '$filtFile' for writing: $!\n";
@sacList	= ();	# list of SAC files to be filtered
@filtList	= ();	# list of filtered SAC files
$i		= 0;
while ($sacFile=<LIST>) {
	chomp $sacFile;
	$str1 = substr($sacFile, 0, 1);
	if ($str1 eq '#') {
		next;	# event is commented out
	} else {
		$sacList[$i]	= $sacFile;
		@array		= split('/', $sacFile);	# separate the directory and file name
		$arrayLen	= $#array + 1;
		$name		= $array[$#array];
		$dir		= '';
		if ($arrayLen > 1) {	# directory name exist 
			my $pathLen	= $arrayLen - 1;
			$dir	= "$array[0]";
			for ($j = 1; $j < $pathLen; $j++) {
				$dir	= "$dir/$array[$j]";
			}
			$dir	= "$dir/";
		}
		## take care of the scenario nyquist frequency is smaller than $hf
		#$delta		= `getsachdr $sacFile delta`;
		#$nyquistFreq	= 0.5/$delta;
		#if ($nyquistFreq < $hf) {
		#	my $hf0	= $hf;
		#	$hf	= ($nyquistFreq - 0.5);
		#	print ">>> $sacFile: Nyquist($nyquistFreq) < hf($hf0); hf is reset to $hf Hz.\n";
		#}
		$filtFile	= "${dir}filt.$name";
		print FILT "$filtFile\n";
		$filtList[$i]	= $filtFile;
		$i++;
	}
}
close(LIST);
close(FILT);

$sacNum		= $#sacList + 1;	## number of total SAC files to be filtered
$cycleNum	= int($sacNum/$cycle);	## number of cycles for filtering
$cycleNum++ if (($sacNum - $cycleNum*$cycle) > 0);
print ">>> A total of $sacNum SAC files will be filtered in $cycleNum cycles.\n";
	
## filtering section
for ($i = 0; $i < $cycleNum; $i++) {
	$num	= $i*$cycle;
	$sacSeq	= "$sacList[$num]";
	$filtSeq = "$filtList[$num]";
	while (($num+1) < ($i+1)*$cycle && ($num+1) < $sacNum) {
		$num++;
		$sacSeq	= "$sacSeq $sacList[$num]";
		$filtSeq = "$filtSeq $filtList[$num]";
	}

	system "sac<<EOF

echo processed
production on
readerr badfile fatal nofiles fatal

read $sacSeq
taper type hanning width 0.1
bandpass npoles $poleNum corner $lf $hf passes $passNum
write $filtSeq

quit

EOF";
}
print ">>> Done!\n";
