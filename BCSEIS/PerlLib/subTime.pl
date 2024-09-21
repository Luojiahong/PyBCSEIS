#!/usr/bin/perl -w

#################################################################################
#	Library:	subTime							#
#										#
#	Purpose:	Define several subroutines for conversions between	#
#			several difference formats.				#
#	Author:		Wen-xuan (Wayne) Du					#
#	Last updated:	Nov. 17, 2000						#
#################################################################################

#################################################################################
#	Subroutine:	isLeapYear($year)					#
#										#
#	Purpose:	Testing whether $year is a leap year			#
#	Argument:	$year							#
#	Return value:	1 if $year is a leap year; 0 if not.			#
#################################################################################
sub isLeapYear {
    my ($year) = @_;
    my $leap_flag = 0;
    $leap_flag = 1 if (((($year % 4) == 0) && (($year % 100) != 0)) || (($year % 400) == 0));
    return $leap_flag;
}                                                                                                   

#################################################################################
#	Subroutine:	getYearDay($year, $month, $monthday)			#
#										#
#	Purpose:	calculate the yearday from the input time info.		#
#	Return value:	a scalar: $yearday					#
#################################################################################
sub getYearDay {
    my ($year, $month, $monthday) = @_;
    my @norm_year = (0,31,59,90,120,151,181,212,243,273,304,334);
    my @leap_year = (0,31,60,91,121,152,182,213,244,274,305,335);

    ## test whether is a leap year
    my $leap_flag = isLeapYear($year);

    my @year	= @norm_year;
    @year	= @leap_year if ($leap_flag);
    my $yearday = $year[$month-1] + $monthday;

    return $yearday;
}

#################################################################################
#	Subroutine:	revYearDay($year, $yearday)				#
#										#
#	Purpose:	calculate the month and monthday from the inputs:	#
#			year and yearday					#
#	Return value:	an array ($month, $monthday)				#
#################################################################################
sub revYearDay {
    my ($year, $yearday) = @_;
    my @norm_year = (0,31,59,90,120,151,181,212,243,273,304,334);
    my @leap_year = (0,31,60,91,121,152,182,213,244,274,305,335);

    ## test whether is leap year
    my $leap_flag = isLeapYear($year);

    my @year = @norm_year;
    @year = @leap_year if ($leap_flag);

    my $month = 0;
    my $monthday = 0;

    for (my $i = 0; $i < $#year; $i++) {
	if ($year[$i] < $yearday && $yearday <= $year[$i+1]) {
           $monthday = $yearday - $year[$i];
           $month  = $i + 1;
	   last;
        }
    }

    return ($month, $monthday);
}

1;
