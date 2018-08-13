#!/usr/bin/env perl
use strict;
use warnings;

##
# todos.pl
# Parse a concrete directory's files for "TODO" commentaries
# and print them to STDOUT.
#
# Those TODO must have the next structure in order to be correctly
# parsed:
#
# TODO [NAME PRIORITY] BRIEF SUBJECT
#
# Where NAME is the name of the writer of the comment.
# Where PRIORITY is one of the following:
#	lp - low priority
#	mp - middle priority
#	hp - high priority
# Where BRIEF SUBJECT is a small introduction to the TODO reason.
#
# NOTE: In this actual version 'todos.pl' only search in *.c / *.h
# files. If this is not the future desired behaviour, change it
# in the &wanted function.
#
# @author: criw <contact@thecriw.com>
# @maintainer: criw <contact@thecriw.com>


use FindBin;
use lib "$FindBin::RealBin/";
use Colors qw(GREEN RED YELLOW BLUE);

use File::Find;

sub print_todo($$$$);

# User must provide a unique argument which is the directory
# where the parsing will be executed.
#
# Furthemore, said directory must exist.
die "Usage: $0 [SEARCH_DIR]" if @ARGV != 1;

my $src_path = shift;
chomp $src_path;
die "'$src_path' is not a directory." if !-d $src_path;

my @files;
find(\&wanted, $src_path);

for my $file (@files) {
	my @todos;

	# Each of the files must exist and be readable.
	die "'$file' doesn't exist or is not readable." if !-f $file;

	open(my $f, "<", $file) or die "Couldn't open file '$file'. $!";

	while(<$f>) {
		push(@todos, $_) if m/TODO/;
	}

	for (@todos) {
		# As said before, valid TODO's must be formated as:
		#	TODO [NAME PRIORITY] BRIEF SUBJECT
		# Refer to the top of this document for a explanation of each
		# part of it.
		my ($by, $priority, $brief) = ($_ =~ /\[ ?(\w+) (\w+) ?\] ?(.*)/);
		print_todo($file, $by, $priority, $brief);
	}
}

print "SEARCH DIR: '$src_path'\n";

##
# wanted
# Specifies how to handle a file detected by File::Find find() function.
sub wanted
{
	push(@files, $File::Find::name) if m/.\.(c|h)$/;
	return;
}

##
# print_todo $file $by $priority $brief
sub print_todo($$$$)
{
	my $file = shift;
	my $by = shift;
	my $priority = shift;
	my $brief  = shift;

	$priority = Colors::Green("Low Priority") if $priority eq "lp";
	$priority = Colors::Yellow("Medium Priority") if $priority eq "mp";
	$priority = Colors::Red("High Priority") if $priority eq "hp";

	$by = Colors::Blue(uc $by);
	$priority = Colors::Green($priority);

	$file = Colors::Blue $file;

	print "AUTHOR: $by\tPRIORITY: $priority\n"; 
	print "$file: $brief\n\n";
}
