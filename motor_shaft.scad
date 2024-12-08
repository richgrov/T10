function motor_shaft_cut_length() = 15;
function motor_shaft_length() = 22;
function motor_box_height() = 48;
function motor_box_length() = 42;
function motor_cyl_height() = 2;
function motor_total_height() = motor_box_height() + motor_cyl_height() + motor_shaft_length();

module motor_shaft(tolerance = 0) {
	shaft_length = motor_shaft_length();
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

module motor_body() {
	cube([motor_box_length(), motor_box_length(), motor_box_height()]);
}

module nema_17() {
	motor_shaft();

	translate([0, 0, motor_shaft_length()]) {
		cylinder(h = motor_cyl_height(), r = 11);
		translate([-motor_box_length()/2, -motor_box_length()/2, 2]) {
			motor_body();
		}
	}
}

nema_17();
