#!/usr/bin/perl -w

#################################################################################
#       Subroutine:     isInArray($member, @array)                              #
#       Purpose:        return 0 if $member is not in @array                    #
#                       return 1 if $member is in @array                        #
#################################################################################
sub isInArray {
        my ($member, @array) = @_;
        my $flag = 0;
        foreach $arr (@array) {
                if ($member eq $arr) {
                        $flag = 1;
                        last;
                }
        }
        return $flag;
}

1;
