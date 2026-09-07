#!/usr/bin/env perl
use v5.36;
use List::Util qw(max);

# Generate an SVG drawing of a simple fractal tree (front view).
#
# Run: perl tree.pl [branches] [output.svg]
#      perl tree.pl 4 tree.svg

use constant {
    VIEWBOX_W      => 400,
    VIEWBOX_H      => 400,
    STEM_HEIGHT    => 20,
    INITIAL_RADIUS => 250,
    INITIAL_ANGLE  => 90,
    LINE_COLOR     => '#2B1100',
    LINE_WIDTH     => 5,
    LEAF_COLOR     => '#00A000',
};

sub main {
    my $branches = shift(@ARGV) // 4;
    my $output   = shift(@ARGV) // 'tree.svg';

    die "branches must be a positive integer\n"
      unless $branches =~ /^\d+$/ && $branches > 0;

    open( my $fh, '>', $output ) or die "Cannot open $output: $!\n";
    print $fh build_svg($branches);
    close $fh;

    say "Check $output";
    say "Finished.";
}

sub move_point ( $x, $y, $radius, $angle_deg ) {
    my $rad = $angle_deg * 3.14159265358979 / 180;
    return ( $x + $radius * cos($rad), $y + $radius * sin($rad) );
}

sub flip_y ($y) { return VIEWBOX_H - $y }

sub svg_line (%p) {    # id, x0, y0, x1, y1
    my $y0 = flip_y( $p{y0} );
    my $y1 = flip_y( $p{y1} );
    return
      sprintf( qq{\t<path\n\t\tstyle="fill:none;stroke:%s;stroke-width:%d;}
          . qq{stroke-linecap:round;stroke-linejoin:round"\n}
          . qq{\t\td="M %.2f,%.2f %.2f,%.2f"\n\t\tid="%s" />\n},
        LINE_COLOR, LINE_WIDTH, $p{x0}, $y0, $p{x1}, $y1, $p{id} );
}

sub svg_ellipse (%p) {    # id, cx, cy, rx, ry
    my $cy = flip_y( $p{cy} );
    return
      sprintf( qq{\t<ellipse\n\t\tstyle="opacity:0.6;fill:%s;stroke:none"\n}
          . qq{\t\trx="%.2f" ry="%.2f" cx="%.2f" cy="%.2f"\n\t\tid="leaf_%s" />\n},
        LEAF_COLOR, $p{rx}, $p{ry}, $p{cx}, $cy, $p{id} );
}

sub ramify ( $id, $x0, $y0, $radius, $angle, $branches, $svg_ref ) {
    my ( $x1, $y1 ) = move_point( $x0, $y0, $radius, $angle );

    $$svg_ref .=
      svg_line( id => $id, x0 => $x0, y0 => $y0, x1 => $x1, y1 => $y1 );
    $$svg_ref .= svg_ellipse(
        id => $id,
        cx => $x1,
        cy => $y1,
        rx => $branches * 20,
        ry => $branches * 10
    );

    return if $branches <= 1;

    my $step_x       = ( $x1 - $x0 ) / $branches;
    my $step_y       = ( $y1 - $y0 ) / $branches;
    my $child_radius = $radius / $branches;

    for my $i ( 1 .. $branches - 1 ) {
        my $x        = $x0 + $i * $step_x;
        my $y        = $y0 + $i * $step_y;
        my $child_id = "${id}_$i";
        ramify(
            $child_id, $x, $y, $child_radius,
            $angle + 45,
            $branches - 1, $svg_ref
        );
        ramify(
            $child_id, $x, $y, $child_radius,
            $angle - 45,
            $branches - 1, $svg_ref
        );
    }
}

sub build_svg ($branches) {
    my $svg =
      sprintf( qq{<?xml version="1.0" standalone="no"?>\n}
          . qq{<svg width="%d" height="%d" viewBox="0 0 %d %d"\n}
          . qq{\txmlns="http://www.w3.org/2000/svg" version="1.1">\n}
          . qq{\t<title>Tree SVG - Perl</title>\n},
        VIEWBOX_W, VIEWBOX_H, VIEWBOX_W, VIEWBOX_H );

    my $mid_x = VIEWBOX_W / 2;

    $svg .=
      svg_line( id => 'ground', x0 => 0, y0 => 0, x1 => VIEWBOX_W, y1 => 0 );
    $svg .= svg_line(
        id => 'trunk',
        x0 => $mid_x,
        y0 => 0,
        x1 => $mid_x,
        y1 => STEM_HEIGHT
    );

    my $branch_svg = '';
    ramify( 'branch', $mid_x, STEM_HEIGHT, INITIAL_RADIUS, INITIAL_ANGLE,
        $branches, \$branch_svg );
    $svg .= $branch_svg;

    $svg .= "</svg>\n";
    return $svg;
}

# Entry point
main() unless caller;
