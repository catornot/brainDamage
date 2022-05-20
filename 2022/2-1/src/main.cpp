#include "main.h"
#include "okapi/api.hpp"
// this must be moved to main.h or not idk


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "The Fog is Coming");
	pros::lcd::set_text(4, "       _.-;;-._");
 	pros::lcd::set_text(6, "'-..-'|   ||   |");
 	pros::lcd::set_text(7, "'-..-'|_.-;;-._|");
	pros::lcd::set_text(8, "'-..-'|   ||   | ");
	pros::lcd::set_text(9, "'-..-'|_.-''-._|");
}

void disabled() {}


void competition_initialize() {}

using namespace okapi;
void autonomous()
{
		// pros::Motor left_mtr2( 16 ); // huh
		// pros::Motor right_mtr1( 7 );
		// pros::Motor left_mtr1( 18 );
		// pros::Motor right_mtr2( 12 );
		pros::Motor right_suck( 11, true );
		pros::Motor left_suck( 20 );

		ChassisScales scales( { 4_in, 15.5_in }, imev5GreenTPR ); // 11.1, 17.1, 15 so this wheel, wheel tract which should be 17.1 thought 17.1 is not the right value and should be remesured https://okapilib.github.io/OkapiLib/classokapi_1_1ChassisScales.html

		std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
				.withMotors( { 16, 18 }, { 12, 7 } ) // this returns the new chassis back so it should work
				.withDimensions( AbstractMotor::gearset::green, scales ) // if moving scales doesn't work revert to passed methode ( moved to remove the clutter )
				.build();

		// pros::delay( 50 );
		// while( !chassis -> isSettled() )
		// {
		// 	pros::delay( 2 );
		// }
		// if waitUntilSettled doesn't work try this ^
		// chassis -> turnAngle( 0_deg );
		// chassis -> waitUntilSettled();

		chassis -> setMaxVelocity( 50 ); // this should exist
		chassis -> moveDistance( 10_in );
		chassis -> waitUntilSettled(); // same for this maybe I could use IsSettled(); with a while loop like smth like this

		for( int x = 0; x < 3; x++ )
		{
				chassis -> turnAngleAsync( 90_deg );
				chassis -> waitUntilSettled();

				chassis -> moveDistance( 10_in );
				chassis -> waitUntilSettled();
		}
		// chassis -> turnAngle( 90_deg );
		// chassis -> waitUntilSettled();
		//
		// chassis -> moveDistance( 10_in );
		// chassis -> waitUntilSettled();
}

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr2( 16 );
	pros::Motor right_mtr1( 7 ); // TODO : check if the problem is the reverse
	pros::Motor left_mtr1( 18 );
	pros::Motor right_mtr2( 12 );
	pros::Motor right_suck( 11, true );
	pros::Motor left_suck( 20 );
	// pros::ADIAnalogOut speaker()

	master.set_text(0, 0, "Better Win Nerd");
	master.rumble("  -.-."); // huh this doesn't do anything, do controllers even have rumble motors?

	while (true) {

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int power = master.get_analog(ANALOG_LEFT_X);
		int turn = master.get_analog(ANALOG_LEFT_Y);
		int suck_power = master.get_analog(ANALOG_RIGHT_Y);

		int left = power + turn;
		int right = power - turn;

		pros::lcd::print(2, "power :%d, turn : %d", power, turn );
		pros::lcd::print(3, "left :%d, right : %d", left, right );
		pros::lcd::print(4, "move_velocity l2 :%lf", left_mtr2.get_actual_velocity() );
		pros::lcd::print(5, "move_velocity l1 :%lf", left_mtr1.get_actual_velocity() );
		pros::lcd::print(8, "move_velocity r1 :%lf", right_mtr1.get_actual_velocity() );
		pros::lcd::print(9, "move_velocity r2 :%lf", right_mtr2.get_actual_velocity() );

		left_mtr1.move_velocity( left );
		right_mtr1.move_velocity( right );
		left_mtr2.move_velocity( left );
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
