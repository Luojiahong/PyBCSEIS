#!/usr/bin/perl -w
#Script:	getStaList.pl
#Purpose:	To produce a list of SAC files belong to a specific station with a certain component
#Author:	Wen-xuan(Wayne) Du
#Date:		January 2, 2003
#Last Modified:	January 10, 2003

$script	= "getStaList.pl";
$argc	= $#ARGV+1;
if ($argc != 4 && $argc != 5) {
	print "Usage:\t\t$script <flag> <dirFile> <sta> <comp> [dataDir]\n";
	print "<flag>:\t\t1 - Raw data; 2 - Filtered data\n";
	print "<dirFile>:\tdirectory file\n";
	print "<sta>:\t\tstation code\n";
	print "<comp>:\t\tcomponent name <N,E,Z>\n";
	print "[dataDir]:\tpath to data directory (relative)\n\n";
	print "Output file '<sta>.list' is a list of SAC files belonging to station <sta> with component <comp>\n";
	exit (-1);
}

# first get the command line arguments
$flag		= shift;	# flag
if ($flag != 1 && $flag != 2) {
	print "Incorrect input for 'flag': should be either 0 or 1!\n";
	exit (-1);
}
$dirFile	= shift;	# directory file
$sta		= shift;	# station name
$comp		= shift;	# component name
if ($comp ne 'N' && $comp ne 'E'&& $comp ne 'Z') {
	print "Incorrect input for 'comp': should be one of 'N', 'E' and 'Z'!\n";
	exit (-1);
}
$base	= '';			# base directory path
if ($argc == 5) {
	$dataDir = shift;
	$base    = "$dataDir/";
}
$listFile = "${sta}.list";	# list file name

open(DIR, "<$dirFile") or die "Can not open directory file '$dirFile' for reading: $!\n";
open(LIST, ">$listFile") or die "Can not open directory file '$listFile' for reading: $!\n";
print ">>> Producing the list of SAC files belonging to station $sta with $comp component ...\n"; 
$count	= 0;	# number of directories to search for SAC file
$good	= 0;	# number of directories containing the SAC file wanted
while ($dir=<DIR>) {
	chomp $dir;
	$sacFile = `ls ${base}$dir/${sta}.*${comp}.SAC` if ($flag == 1);
	$sacFile = `ls ${base}$dir/filt.${sta}.*${comp}.SAC` if ($flag == 2);
	chomp $sacFile;
	print LIST "$sacFile\n" if ($sacFile);
	$good++ if ($sacFile);
	$count++;
}
close(DIR);
close(LIST);
print ">>> $good out of $count directories contain SAC file belonging to station $sta with $comp component.\n";
print ">>> '$listFile' is the list file produced. Done!\n";
