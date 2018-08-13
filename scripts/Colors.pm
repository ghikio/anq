#!/usr/bin/env perl
use strict;
use warnings;

##
# Colors.pm
# Provide functions that, with the use of ANSI escape sequences,
# change the display color of a string.
#
# NOTE: This should work in most *nix terminals, but may fail
# in Windows or specific systems (idk to be honest).
#
# @author: criw <contact@thecriw.com>
# @maintainer: criw <contact@thecriw.com>
package Colors;

use Exporter;
our @EXPORT_OK = qw(Red Green Yellow Blue);

use constant {
	NORMAL	=> "\x1B[0m" ,
	RED	=> "\x1B[31m",
	GREEN	=> "\x1B[32m",
	YELLOW	=> "\x1B[33m",
	BLUE	=> "\x1B[34m",
};

##
# Red $str
sub Red($)
{
	my $str = shift;
	$str = RED . $str . NORMAL;
	$str;
}

##
# Green $str
sub Green($)
{
	my $str = shift;
	$str = GREEN . $str . NORMAL;
	$str;
}

##
# Yellow $str
sub Yellow($)
{
	my $str = shift;
	$str = YELLOW . $str . NORMAL;
	$str;
}

##
# Blue $str
sub Blue($)
{
	my $str = shift;
	$str = BLUE . $str . NORMAL;
	$str;
}
