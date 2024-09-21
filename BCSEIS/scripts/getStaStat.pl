#!/usr/bin/perl -w
#Script:	getStaStat.pl
#Purpose:	To get the statistical information regarding to the stations in a list
#Author:	Wen-xuan(Wayne) Du
#Date:		January 23, 2003
#Last Modified:	January 23, 2003

$script	= "getStaStat.pl";
$argc	= $#ARGV+1;
if ($argc != 2) {
	print "Usage:\t\t$script <staFile> <dirList>\n";
	print "<staFile>:\tlist of station names to check on\n";
	print "<dirList>:\tlist of data directories to check on\n";
	exit (-1);
}

## deal with command line arguments first
$staFile	= shift;
$dirList	= shift;
$statFile	= "staStat.w";	# statistical file about the waveform availability for stations in file $staFile
$statFile0	= "staStat.p";	# statistical file about the P arrival availability for stations in file $staFile
$statFile1	= "staStat.s";	# statistical file about the S arrival availability for stations
$statFile2	= "staStat.pas"; # statistical file about the P&S arrival availability for stations 
$comp = "Z";

open(STA,"<$staFile") or die "Can not open station file $staFile for reading: $!\n";
open(STAT,">$statFile") or die "Can not open statistical file $statFile for writing: $!\n";
open(STAT0,">$statFile0") or die "Can not open statistical file $statFile0 for writing: $!\n";
open(STAT1,">$statFile1") or die "Can not open statistical file $statFile1 for writing: $!\n";
open(STAT2,">$statFile2") or die "Can not open statistical file $statFile2 for writing: $!\n";

@staList = ();
while (<STA>) {
	chomp;
	push @staList, $_;
}
close(STA);

$statDir = "StaStat";
if (!-d $statDir) {
	print ">>> mkdir $statDir\n";
	system "mkdir $statDir";
}

%staStat = ();
%staStat0 = ();
%staStat1 = ();
%staStat2 = ();
for ($i = 0; $i <= $#staList; $i++) {
	$sta = $staList[$i];
	chomp ($sta);
#	$snum=length($staList[$i]);
	$sta = substr$staList[$i],0,3;
	print ">>> getStaList.pl 1 $dirList $sta $comp\n";
	system "getStaList.pl 1 $dirList $sta $comp";

	$list  = ${sta}.".list";
	$list0 = "${sta}.plist";
	$list1 = "${sta}.slist";
	$list2 = "${sta}.pslist";
	print ">>> chkSacListPha2.pl $list $list0 $list1 $list2\n";
	system "chkSacListPha2.pl $list $list0 $list1 $list2";

	$stat  = `wc -l $list`;
	$stat0 = `wc -l $list0`;
	$stat1 = `wc -l $list1`;
	$stat2 = `wc -l $list2`;
	chomp ($stat,$stat0,$stat1,$stat2);

	my ($blank,$num,$name) = split('\s+',$stat);
	$staStat{$sta} = $num;
	($blank,$num,$name) = split('\s+',$stat0);
	$staStat0{$sta} = $num;
	($blank,$num,$name) = split('\s+',$stat1);
	$staStat1{$sta} = $num;
	($blank,$num,$name) = split('\s+',$stat2);
	$staStat2{$sta} = $num;

	system "mv -f $list $statDir/.";
	system "mv -f $list0 $statDir/.";
	system "mv -f $list1 $statDir/.";
	system "mv -f $list2 $statDir/."; 
}

foreach $sta (sort { $staStat{$b} <=> $staStat{$a} } keys %staStat) {
	print STAT "$sta\t$staStat{$sta}\n";
}
foreach $sta (sort { $staStat0{$b} <=> $staStat0{$a} } keys %staStat0) {
        print STAT0 "$sta\t$staStat0{$sta}\n";
}
foreach $sta (sort { $staStat1{$b} <=> $staStat1{$a} } keys %staStat1) {
        print STAT1 "$sta\t$staStat1{$sta}\n";
}
foreach $sta (sort { $staStat2{$b} <=> $staStat2{$a} } keys %staStat2) {
        print STAT2 "$sta\t$staStat2{$sta}\n";
}
close(STAT);
close(STAT0);
close(STAT1);
close(STAT2);
system "mv -f $statFile $statDir/.";
system "mv -f $statFile0 $statDir/.";
system "mv -f $statFile1 $statDir/.";
system "mv -f $statFile2 $statDir/.";

print ">>> Station statistics are saved in files '$statDir/$statFile', '$statDir/$statFile0', '$statDir/$statFile1' and '$statDir/$statFile2'. Done!\n";
