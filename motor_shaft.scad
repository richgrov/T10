function motor_shaft_cut_length() = 15;

module motor_shaft(tolerance = 0) {
	shaft_length = 22;
	shaft_radius = 2.5 + tolerance;
	cut_length = motor_shaft_cut_length();
	cut_thickness = 0.5;
	$fs = 0.01;

	difference() {
		cylinder(h=shaft_length, r=shaft_radius);
		translate([-5, shaft_radius-cut_thickness, 0]) {
			cube([10, cut_thickness, cut_length]);
		}
	}
}

motor_shaft();
