use <motor_shaft.scad>;

finger_height = 30;
y_size = 30;

module end_effector_half() {
	spacing_for_knob = 12.5;
	finger_thickness = 13.75;

	union() {
		translate([0, 0, finger_height]) {
			cube([
				spacing_for_knob / 2 + finger_thickness,
				y_size,
				motor_shaft_cut_length(),
			]);
		};
		translate([spacing_for_knob / 2, 0, 0]) {
			cube([finger_thickness, y_size, finger_height]);
		}
	}
}

module end_effector() {
	union() {
		end_effector_half();
		mirror([1, 0, 0]) {
			end_effector_half();
		}
	}
}

difference() {
	translate([0, -y_size / 2, 0]) {
		end_effector();
	}
	translate([0, 0, finger_height]) {
		motor_shaft(tolerance = 0.5);
	}
}
