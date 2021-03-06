#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_top(11); //10 - tank //14 11 13 15 /1 - x
	pros::Motor left_bot(16); //8 tank	/ 6 x
	pros::Motor right_top(14); //4 tank / 11 x
	pros::Motor right_bot(20); //3 tank / 16 x

	right_top.set_reversed(true);
	right_bot.set_reversed(true);

	double scales[] = {.5, .5, .5, .5};

	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left_y = master.get_analog(ANALOG_LEFT_Y);
		int left_x = master.get_analog(ANALOG_LEFT_X);
		int right_y = master.get_analog(ANALOG_RIGHT_Y);
		pros::lcd::print(1, "%d", left_top.get_encoder_units());
		//double theta = atan ( left_y / left_x);
		//double vector = pow((pow(left_y, 2) + pow(left_x, 2)) , .5);

		// left_top.move(((left_y+left_x) / 2) * scales[0]); //average of joystick position
		// left_bot.move(((left_y+left_x) / 2) * scales[1]);
		// right_bot.move(((-left_y-left_x) / 2) * scales[3]);
		// right_top.move(((-left_y-left_x) / 2) * scales[2]);


		left_top.move(scales[0] * (left_y + left_x + right_y));
		right_top.move(scales[2] * (left_y - left_x - right_y));
	  left_bot.move(scales[1] * (left_y + left_x - right_y));
		right_bot.move(scales[3] * (left_y - left_x + right_y));



		pros::delay(51);
	}

	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_mtr = left;
		right_mtr = right;
		pros::delay(20);
	}
}
