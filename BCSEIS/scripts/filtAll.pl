#!/usr/bin/perl -w

$lf = 1;
$hf = 15;
$poleNum = 3;
$passNum = 2;
$stationFile = "station.list";

open(STATION,"$stationFile") or die "Can not open station file '$stationFile' for reading: $!\n";
while ($sta=<STATION>) {
	chomp $sta;
	$sta=substr$sta,0,3;
	$listFile = "${sta}.list";
	print $listFile;
	system "cp StaStat/$listFile .";
	print ">>> filtSacList2.pl $listFile $lf $hf $poleNum $passNum\n";
	system "filtSacList2.pl $listFile $lf $hf $poleNum $passNum";
	system "rm filt.${listFile} $listFile";
}
close(STATION);
