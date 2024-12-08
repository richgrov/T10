use <motor_shaft.scad>;

arm_padding = 8;
arm_thickness = motor_box_length() + arm_padding * 2;
arm_length = 150;

module arm() {
	difference() {
		cube([arm_length + motor_box_length() + arm_padding, motor_box_length(), arm_thickness]);
		translate([motor_box_length()/2 + arm_padding, motor_total_height(), motor_box_length()/2 + arm_padding]) {
			rotate([90, 0, 0]) {
				scale([1.1, 1.1, 1]) {
					nema_17();
				}
			}
		}
	}
}

difference() {
	arm();
	translate([(arm_length + motor_box_length() + arm_padding)/2, motor_shaft_cut_length(), arm_thickness/2]) {
		rotate([90, 0, 0]) {
			motor_shaft(tolerance = 0.5);
		}
	}
}

