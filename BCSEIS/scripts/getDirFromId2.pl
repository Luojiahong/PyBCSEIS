#!/usr/bin/perl -w
#Script:	getDirFromId2.pl
#Purpose:	Get an event directory name from an event id
#Author:	Wen-xuan(Wayne) Du
#Date:		January 27, 2003
#Last Modified:	January 27, 2003

$script	= "getDirFromId2.pl";
if ($#ARGV != 1) {
	print "Usage:\t\t$script <idDirFile> <id>\n";
	print "<idDirFile>:\tinput id-directory file\n";
	print "<id>:\t\tinput event id\n";
	print "\nThis program will output the event directory name associated with the event id.\n";
	exit (-1);
}

## deal with command line arguments
$idDirFile = shift;
$id = shift;

$info = `grep '^$id' $idDirFile`;
chomp $info;
if ($info ne '') {
	my ($id0,$dir) = split('\s+',$info);
	print "$dir\n";
}
