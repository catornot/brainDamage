#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
// void on_center_button() {
// 	static bool pressed = false;
// 	pressed = !pressed;
// 	if (pressed) {
// 		pros::lcd::set_text(2, "I was pressed!");
// 	} else {
// 		pros::lcd::clear_line(2);
// 	}
// }

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello World");
	pros::lcd::set_text(4, "| |\\  _  /| |");
	pros::lcd::set_text(5, "|_| \\/\\/ |_|");
	pros::lcd::set_text(6, "");

	// pros::lcd::register_btn1_cb(on_center_button);
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
	pros::Motor left_mtr2( 16 );
	pros::Motor right_mtr2( 14 ); // TODO : check if the problem is the reverse
	pros::Motor left_mtr1( 18 );
	pros::Motor right_mtr1( 12 );
	pros::Motor right_suck( 11, true );
	pros::Motor left_suck( 20 );

	while (true) {

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int power = master.get_analog(ANALOG_LEFT_X);
		int turn = master.get_analog(ANALOG_LEFT_Y);
		int suck_power = master.get_analog(ANALOG_RIGHT_Y);
		
		pros::lcd::print(2, "power :%d, turn : %d", power, turn );
		pros::lcd::print(3, "left :%d, right : %d", left, right );

		int left = power + turn;
		int right = power - turn;

		left_mtr1.move_velocity( left );
		left_mtr2.move_velocity( left );
		right_mtr1.move_velocity( right );
		right_mtr2.move_velocity( right );
		
		if ( suck_power != 0 )
		{
			right_suck.move_velocity( suck_power );
			left_suck.move_velocity( suck_power );
		}
		else
		{
			right_suck.brake();
			left_suck.brake();
		}

		pros::delay(2);
	}
}
