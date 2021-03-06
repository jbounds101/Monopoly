// Monopoly.cpp : Defines the entry point for the console application.
//

// Fix the issue that lets people keep houses even after trading away a combo

#include "stdafx.h"
#define NOMINMAX
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <limits>
#include <vector>
using namespace std;

const string red("\033[1;31m");
const string red_back("\033[41m");
const string green("\033[1;32m");
const string green_back("\033[42m");
const string yellow("\033[1;33m");
const string dark_yellow("\033[0;33m");
const string blue("\033[1;34m");
const string blue_back("\033[44m");
const string magenta("\033[1;35m");
const string magenta_back("\033[1;45m");
const string cyan("\033[1;36m");
const string dark_cyan("\033[0;36m");
// White isn't actually a color - resets color to default (which is white)
const string white("\033[0m");

string null_text;
const int one_second = 1000;

void show_board();
void show_input_prompt();
int calculate_rent();
void show_stats();

class number_calculation_functions
{
public:

	int random_range(int max, int min)
	{
		return rand() % (max + 1 - min) + min;
	}


};

class output_text_functions
{
	const int text_speed = 20;
	const double fast_text_speed = 2.3;
public:

	void color(string color, string text)
	{
		output_with_delay(color, text);
		cout << endl;
	}

	void color_fast(string color, string text)
	{
		int input_length = text.length();
		for (int i = 0; i < input_length; i++)
		{
			cout << color << text[i] << white;
			Sleep(fast_text_speed);
		}
		cout << endl;
	}

	void color_pause(string color, string text)
	{
		output_with_delay(color, text);
		cout << endl;
		code_pause(false);
	}

	void code_pause(bool clear)
	{
		system("pause");
		if (clear == true)
		{
			system("CLS");
		}
	}

	void output_with_delay(string color, string input_string)
	{
		int input_length = input_string.length();
		for (int i = 0; i < input_length; i++)
		{
			cout << color << input_string[i] << white;
			Sleep(text_speed);
		}
	}

	void clear()
	{
		system("CLS");
	}

	void space(int spaces)
	{
		for (int i = 0; i < spaces; i++)
		{
			cout << " ";
		}
	}
};

struct property
{
	string name;
	int purchase_price;
	int house_price;
	int houses_owned;
	int rent[6];
	string color;
	bool combo;
};

const int number_properties = 40;
property property_list[number_properties] =
{
	{ "Go / Collect $200",    0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Mediterranean Avenue", 100, 50,  0,{ 2,  10,  30,  90,   160,  250 }, dark_cyan, false },
	{ "Community Chest",      0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Baltic Avenue",        100, 50,  0,{ 4,  20,  60,  180,  320,  450 }, dark_cyan, false },
	{ "Income Tax",           0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Reading Railroad",     200, 25,  0,{ 25,  50,   100,   200,    0,    0 }, white, false },
	{ "Oriental Avenue",     100, 50,  0,{ 6,  30,  90,  270,  400,  550 }, cyan, false },
	{ "Chance",               0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Vermont Avenue",      100, 50,  0,{ 6,  30,  90,  270,  400,  550 }, cyan, false },
	{ "Connecticut Avenue",   120, 50,  0,{ 8,  40,  100, 300,  450,  600 }, cyan, false },
	{ "Just Visiting / Jail", 0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "St. Charles Place",   140, 100, 0,{ 10, 50,  150, 450,  625,  750 }, magenta, false },
	{ "Electric Company",     150, 0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "States Avenue",       140, 100, 0,{ 10, 50,  150, 450,  625,  750 }, magenta, false },
	{ "Virginia Avenue",      160, 100, 0,{ 12, 60,  180, 500,  700,  900 }, magenta, false },
	{ "Pennsylvania Railroad",200, 25,  0,{ 25,  50,  100,   200,    0,    0 }, white, false },
	{ "St. James Place",     180, 100, 0,{ 14, 70,  200, 550,  750,  950 }, dark_yellow, false },
	{ "Community Chest",      0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Tennessee Avenue",    180, 100, 0,{ 14, 70,  200, 550,  750,  950 }, dark_yellow, false },
	{ "New York Avenue",      200, 100, 0,{ 16, 80,  220, 600,  800,  1000 }, dark_yellow, false },
	{ "Free Parking",         0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Kentucky Avenue",     220, 150, 0,{ 18, 90,  250, 700,  875,  1050 }, red, false },
	{ "Chance",               0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Indiana Avenue",      220, 150, 0,{ 18, 90,  250, 700,  875,  1050 }, red, false },
	{ "Illinois Avenue",      240, 150, 0,{ 20, 100, 300, 750,  925,  1100 }, red, false },
	{ "B. & O. Railroad",     200, 25,  0,{ 25,  50,   100,   200,    0,    0 }, white, false },
	{ "Atlanic Avenue",       260, 150, 0,{ 22, 110, 330, 800,  975,  1150 }, yellow, false },
	{ "Ventnor Avenue",       260, 150, 0,{ 22, 110, 330, 800,  975,  1150 }, yellow, false },
	{ "Water Works",          150, 0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Marvin Gardens",       280, 150, 0,{ 24, 120, 360, 850,  1025, 1200 }, yellow, false },
	{ "Go to Jail",           0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Pacific Avenue",       300, 200, 0,{ 26, 130, 390, 900,  1100, 1275 }, green, false },
	{ "North Carolina Avenue",300, 200, 0,{ 26, 130, 390, 900,  1100, 1275 }, green, false },
	{ "Community Chest",      0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Pennsylvania Avenue",  320, 200, 0,{ 28, 150, 450, 1000, 1200, 1400 }, green, false },
	{ "Short Line Railroad",  200, 25,  0,{ 25,  50,   100,   200,    0,    0 }, white, false },
	{ "Chance",               0,   0,   0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Park Place",           350, 200, 0,{ 35, 175, 500, 1100, 1300, 1500 }, blue, false },
	{ "Luxury Tax",           0,   100, 0,{ 0,  0,   0,   0,    0,    0 }, white, false },
	{ "Boardwalk",            400, 200, 0,{ 50, 200, 600, 1400, 1700, 2000 }, blue, false }
};

struct player
{
	string icon;
	int position;
	int money;
	bool in_jail;
	string color;
	vector<bool> properties_owned;
	int railroads_owned;
	int utilities_owned;
	int time_in_jail;
	bool bankrupt;
	bool combo;
};

const int number_players = 4;
// Maybe change later; for now, keep at 3 constant players
// For now it seems that 3 players is enough
player player_list[number_players] =
{
	// Sets player values
	{ "P1", 0, 1500, false, red_back,{}, 0, 0, 0, false, false },
	{ "P2", 0, 1500, false, green_back,{}, 0, 0, 0, false, false },
	{ "P3", 0, 1500, false, blue_back,{}, 0, 0, 0, false, false },
	{ "P4", 0, 1500, false, magenta_back,{}, 0, 0, 0, false, false }

};

int current_player = 0;
int players_remaining = number_players;
int player_choice = 0;
int player_choice_confirm = 0;
int player_choice_quantity = 0;
int player_choice_buy_property = 0;
int random_chance = 0;



// Use this var for debugging (allows changing of the dice total output)
int debug_dice = 0;



int main()
{
	// Needed for main to function properly
	output_text_functions fString;
	number_calculation_functions fNumber;
	srand(time(NULL));
	cout << red << "DEBUG" << white;
	fString.clear();

	// Sets all properties (vector) owned for players to false
	for (int k = 0; k < number_players; k++)
	{
		for (int i = 0; i < 40; i++)
		{
			player_list[k].properties_owned.push_back(false);
		}
	}





	while (players_remaining > 1)
	{
		if (player_list[current_player].bankrupt == false)
		{
			show_input_prompt();
		}
		current_player += 1;
		if (current_player == number_players)
		{
			current_player = 0;
		}
	}
	cout << player_list[current_player].color << player_list[current_player].icon << white << " has won!";
	// Only one player remains




	return 0;
}

void show_board()
{
	output_text_functions fString;
	string output_color;

	cout << white;
	for (int i = 0; i < 40; i++)
	{

		int prefix_space = 3 * number_players;
		int suffix_space = 30;




		for (int j = 0; j < number_players; j++)
		{
			if (player_list[j].position == i)
			{
				cout << player_list[j].color << player_list[j].icon << white << " ";
				prefix_space -= 3;
			}
		}

		for (int k = 0; k < prefix_space; k++)
		{
			cout << " ";
		}
		cout << property_list[i].color << property_list[i].name;



		int string_length = (property_list[i].name).length();
		for (int k = string_length; k < suffix_space; k++)
		{
			cout << " ";
		}
		if (property_list[i].purchase_price == 0)
		{
			cout << red << "//////////////////";
		}
		else
		{
			bool display_purchase = true;
			for (int j = 0; j < number_players; j++)
			{
				if (player_list[j].properties_owned[i] == true)
				{
					cout << white << player_list[j].color << "  PROPERTY OWNED  ";
					cout << white;
					display_purchase = false;
				}
			}
			if (display_purchase == true)
			{
				cout << white << "Purchase cost: " << property_list[i].purchase_price;
			}
		}




		fString.space(10);


		fString.space(10);


		bool not_owned = true;
		if (property_list[i].rent[0] == 0)
		{
			cout << red << "///////////////////";
		}
		else
		{
			if ((i == 5) || (i == 15) || (i == 25) || (i == 35))
			{
				// Needs to know what player the railroad is owned by
				for (int h = 0; h < number_players; h++)
				{
					if (player_list[h].properties_owned[i] == true)
					{
						cout << white << "Current Rent: $" << property_list[i].rent[player_list[h].railroads_owned - 1];
						not_owned = false;
					}
				}

			}
			else
			{
				for (int h = 0; h < number_players; h++)
				{
					if (player_list[h].properties_owned[i] == true)
					{
						cout << white << "Current Rent: $" << property_list[i].rent[property_list[i].houses_owned];;
						not_owned = false;
					}

				}
			}
			if (not_owned == true)
			{
				cout << red << "///////////////////";
			}
		}

		cout << white << endl;


	}
	cout << white << endl;
}

void show_input_prompt()
{
	output_text_functions fString;
	number_calculation_functions fNumber;



	bool dice_rolled = false;
	bool double_roll = false;
	int dice_one = 0;
	int dice_two = 0;
	int dice_total = 0;
	int jail_counter = 0;
	int property_counter = 0;
	bool just_out_of_jail = false;
	bool money_check = false;
	bool trading_money_output = false;
	bool trading_money_input = false;
	int money_amount_output = 0;
	int money_amount_input = 0;
	int trade_player = 0;
	bool exit_trade = false;
	int amount_of_money = 0;
	vector<int> trade_away;
	vector<int> trade_in;
	int suffix_space = 30;
	int larger_number = 0;
	int house_sell_money_output = 0;
	int house_sell_money_input = 0;


	cout << endl;
	while ((dice_rolled == false) || (double_roll == true))
	{
		trade_away.clear();
		trade_in.clear();
		exit_trade = false;
		money_check = false;
		money_amount_output = 0;
		money_amount_input = 0;
		amount_of_money = 0;
		larger_number = 0;
		trade_player = 0;
		trading_money_input = false;
		trading_money_output = false;
		house_sell_money_output = 0;
		house_sell_money_input = 0;

		fString.clear();
		show_board();
		cout << "It is " << player_list[current_player].color << player_list[current_player].icon << "'s" << white << " turn!" << endl;
		cout << yellow << "Money: $ " << player_list[current_player].money << endl;

		if (double_roll == true)
		{
			if (jail_counter >= 3)
			{
				player_list[current_player].in_jail = true;
				player_list[current_player].position = 10;
				player_list[current_player].time_in_jail = 0;

				fString.color_pause(white, "You were caught speeding and put into jail!");
				fString.clear();
				break;

			}
			else
			{
				if (player_list[current_player].in_jail == false)
				{
					fString.color(white, "Roll again! Select an option:");
					fString.color_fast(yellow, "1: Roll dice");
					fString.color_fast(yellow, "2: Buy houses");
					fString.color_fast(yellow, "3: Trade");
				}
				else
				{
					// This only runs when the player is in jail and has rolled doubles
					player_list[current_player].in_jail = false;
					double_roll = false;
					cout << "You have been freed from jail!" << white << endl;
					fString.code_pause(false);

					break;
				}
			}
		}

		if ((player_list[current_player].in_jail == false) && (double_roll == false))
		{
			fString.color(white, "Select an option:");
		}
		else
		{
			player_list[current_player].time_in_jail += 1;

			if (player_list[current_player].time_in_jail == 3)
			{
				// If player does not get out of jail within 3 turns, fine them $50 and release them
				player_list[current_player].in_jail = false;
				double_roll = false;
				cout << "You have been freed from jail!" << white << endl;
				fString.code_pause(false);
				fString.color(white, "Select an option:");
			}

			if ((player_list[current_player].time_in_jail != 3) && (player_list[current_player].in_jail == true))
			{
				fString.color_fast(white, "You are in jail! (Roll doubles to get out OR be released in three turns! Select an option:");
				cout << "Turns in jail: " << yellow << player_list[current_player].time_in_jail << white << " / " << yellow << "3" << white << endl;
				fString.color_fast(yellow, "1: Roll dice");
				fString.color_fast(yellow, "2: Buy houses");
				fString.color_fast(yellow, "3: Trade");
			}



		}
		if ((double_roll == false) && (player_list[current_player].in_jail == false))
		{
			fString.color_fast(yellow, "1: Roll dice");
			fString.color_fast(yellow, "2: Buy houses");
			fString.color_fast(yellow, "3: Trade");
		}



		cin >> player_choice;

		switch (player_choice)
		{
		case 1:
			dice_rolled = true;

			dice_one = fNumber.random_range(6, 1);
			dice_two = fNumber.random_range(6, 1);
			dice_total = dice_one + dice_two;
			if (debug_dice > 0)
			{
				dice_total = debug_dice;
			}

			if (dice_one == dice_two)
			{

				double_roll = true;

				if (player_list[current_player].in_jail == false)
				{
					jail_counter += 1;
				}
			}
			else
			{
				double_roll = false;
			}




			if (player_list[current_player].in_jail == false)
			{
				player_list[current_player].position += dice_total;


				if (player_list[current_player].position >= 40)
				{
					money_check = true;
					player_list[current_player].money += 200;
					player_list[current_player].position -= 40;
					//	fString.code_pause(false);
				}
				fString.clear();
				show_board();
				cout << "It is " << player_list[current_player].color << player_list[current_player].icon << "'s" << white << " turn!" << endl;
				cout << yellow << "Money: $ " << player_list[current_player].money << endl;
				if (money_check == true)
				{
					cout << "You passed GO and collected $ " << yellow << "200" << white << endl;
				}
			}





			cout << "You rolled a " << yellow << dice_one << white << " and a " << yellow << dice_two << white << "!" << yellow << " (" << dice_total << ")" << white << endl;
			if (double_roll == true)
			{
				cout << dark_yellow << "You rolled doubles!" << white << endl;
			}


			switch (player_list[current_player].position)
			{
			case 2:
			case 17:
			case 33:
				// Community Chest
				random_chance = fNumber.random_range(10, 1);
				cout << "You landed on community chest!" << endl;
				switch (random_chance)
				{
				case 1:
					cout << yellow << "-Advance to GO (Collect $ 200)-" << white << endl;
					player_list[current_player].position = 0;
					player_list[current_player].money += 200;
					show_stats();
					break;
				case 2:
					cout << yellow << "-Bank error in your favor (Collect $ 200)-" << white << endl;
					player_list[current_player].money += 200;
					break;
				case 3:
					cout << yellow << "-Doctor's fees (Pay $ 50)-" << white << endl;
					player_list[current_player].money -= 50;
					break;
				case 4:
					cout << yellow << "-From sale of stock you get $ 50-" << white << endl;
					player_list[current_player].money += 50;
					break;
				case 5:
					player_list[current_player].in_jail = true;
					player_list[current_player].position = 10;
					player_list[current_player].time_in_jail = 0;
					double_roll = false;
					cout << yellow << "-Go to jail, do not collect $ 200-" << white << endl;
					break;
				case 6:
					cout << yellow << "-Grand Opera Night (Collect $ 50 for opening night seats)-" << white << endl;
					player_list[current_player].money += 50;
					break;
				case 7:
					cout << yellow << "-Income tax refund (Collect $ 20)-" << white << endl;
					player_list[current_player].money += 20;
					break;
				case 8:
					cout << yellow << "-Hospital fees (Pay $ 50)-" << white << endl;
					player_list[current_player].money -= 50;
					break;
				case 9:
					cout << yellow << "-School fees (Pay $ 50)-" << white << endl;
					player_list[current_player].money -= 50;
					break;
				case 10:
					cout << yellow << "-You inherit $ 100-" << white << endl;
					player_list[current_player].money += 100;
					break;
				}
				break;

			case 7:
			case 22:
			case 36:
				// Chance
				random_chance = fNumber.random_range(10, 1);
				cout << "You landed on chance!" << endl;
				switch (random_chance)
				{
				case 1:
					cout << yellow << "-Advance to GO (Collect $ 200)-" << white << endl;
					player_list[current_player].position = 0;
					player_list[current_player].money += 200;
					show_stats();
					break;
				case 2:
					cout << yellow << "-Advance to Illinois Avenue, if you pass GO, collect $ 200-" << white << endl;
					if (player_list[current_player].position > 24)
					{
						player_list[current_player].money += 200;
					}
					player_list[current_player].position = 24;
					show_stats();
					break;
				case 3:
					cout << yellow << "-Advance to St. Charles Place, if you pass GO, collect $ 200-" << white << endl;
					if (player_list[current_player].position > 11)
					{
						player_list[current_player].money += 200;
					}
					player_list[current_player].position = 11;
					show_stats();
					break;
				case 4:
					cout << yellow << "-Advance to the nearest railroad (foward), if you pass GO, collect $ 200-" << white << endl;
					if (player_list[current_player].position < 5)
					{
						player_list[current_player].position = 5;
					}
					else if (player_list[current_player].position < 15)
					{
						player_list[current_player].position = 15;
					}
					else if (player_list[current_player].position < 25)
					{
						player_list[current_player].position = 25;
					}
					else if (player_list[current_player].position < 35)
					{
						player_list[current_player].position = 35;
					}
					else if (player_list[current_player].position < 40)
					{
						player_list[current_player].position = 5;
						player_list[current_player].money += 200;
					}
					show_stats();
					break;
				case 5:
					cout << yellow << "-Bank pays you dividend of $ 50-" << white << endl;
					player_list[current_player].money += 50;
					break;
				case 6:
					cout << yellow << "-Go back three spaces-" << white << endl;
					player_list[current_player].position -= 3;
					show_stats();
					break;
				case 7:
					player_list[current_player].in_jail = true;
					player_list[current_player].position = 10;
					player_list[current_player].time_in_jail = 0;
					double_roll = false;
					cout << yellow << "-Go to jail, do not collect $ 200-" << white << endl;
					break;
				case 8:
					cout << yellow << "-Pay poor tax of $ 15-" << white << endl;
					player_list[current_player].money -= 15;
					break;
				case 9:
					cout << yellow << "-Take a trip to Reading Railroad-" << white << endl;
					if (player_list[current_player].position > 5)
					{
						player_list[current_player].money += 200;
					}
					player_list[current_player].position = 5;
					show_stats();
					break;
				case 10:
					cout << yellow << "-Take a walk on Boardwalk-" << white << endl;
					player_list[current_player].position = 39;
					show_stats();
					break;
				}
				break;
			case 4:
				cout << yellow << "You landed on income tax! (Pay $ 200)" << endl;
				player_list[current_player].money -= 200;
				break;
			case 20:
				cout << yellow << "You landed on free parking! (Collect $ 300)" << endl;
				player_list[current_player].money += 300;
				break;
			case 30:
				player_list[current_player].in_jail = true;
				player_list[current_player].position = 10;
				player_list[current_player].time_in_jail = 0;
				double_roll = false;
				cout << yellow << "You landed on the Go to Jail space. (Do not collect $ 200)" << endl;
				break;
			case 38:
				cout << yellow << "You landed on luxury tax! (Pay $ 75)" << endl;
				player_list[current_player].money -= 75;
			}
			if (player_list[current_player].in_jail == true)
			{
				break;
			}


			for (int i = 0; i < number_players; i++)
			{
				if (player_list[i].properties_owned[player_list[current_player].position] == true)
				{
					if (player_list[current_player].properties_owned[player_list[current_player].position])
					{
						// Landed on a property owned and is also the current players property
						cout << "You landed on your own property!";
					}
					else 
					{
						// Landed on another person's property
						int money_paid = calculate_rent();
						if ((player_list[current_player].position == 5) || (player_list[current_player].position == 15) || (player_list[current_player].position == 25) || (player_list[current_player].position == 35))
						{
							switch (player_list[i].railroads_owned)
							{
							case 1:
								money_paid = 25;
								break;
							case 2:
								money_paid = 50;
								break;
							case 3:
								money_paid = 100;
								break;
							case 4:
								money_paid = 200;
								break;
							}
						}

						if ((player_list[current_player].position == 12) || (player_list[current_player].position == 28))
						{
							int random_number_one = fNumber.random_range(6, 1);
							int random_number_two = fNumber.random_range(6, 1);
							int random_number_total = random_number_one + random_number_two;
							switch (player_list[i].utilities_owned)
							{
							case 1:
								money_paid = 4 * random_number_total;
								cout << "You rolled a " << yellow << random_number_one << white << " and a " << yellow << random_number_two << white << " for utility costs! Rent is 4 times the total roll of the dice!" << endl;
								break;
							case 2:
								money_paid = 10 * random_number_total;
								cout << "You rolled a " << yellow << random_number_one << white << " and a " << yellow << random_number_two << white << " for utility costs! Rent is 10 times the total roll of the dice!" << endl;
								break;
							}
						}



						player_list[current_player].money -= money_paid;
						player_list[i].money += money_paid;
						
						cout << white << "You landed on " << player_list[i].color << player_list[i].icon << "'s" << white << " property (" << property_list[player_list[current_player].position].name << "), and paid $ " << yellow << money_paid << white << " for rent! (Money after rent: $ " << yellow << player_list[current_player].money << white << ")" << endl;
						cout << player_list[i].color << player_list[i].icon << white << " Money: $ " << yellow << player_list[i].money << white;
					}
				}
			}
			property_counter = 0;
			for (int u = 0; u < number_players; u++)
			{
				if (player_list[u].properties_owned[player_list[current_player].position] == false)
				{
					property_counter += 1;
				}
			}
			if ((property_counter == number_players) && (property_list[player_list[current_player].position].purchase_price > 0) && (player_list[current_player].money > property_list[player_list[current_player].position].purchase_price))
			{
				// This will run when 1. The property landed on is not owned by anyone / 2. The property has a purchase price / 3. The player has enough money to make this purchase
				cout << "You landed on " << yellow << property_list[player_list[current_player].position].name << white << "! Do you want to purchase this property? (Cost: $ " << yellow << property_list[player_list[current_player].position].purchase_price << white << ")" << endl;
				cout << yellow << "Curent money: $ " << player_list[current_player].money << endl;
				fString.color_fast(yellow, "1: Yes");
				fString.color_fast(yellow, "2: No");
				cin >> player_choice_buy_property;
				switch (player_choice_buy_property)
				{
				case 1:

					if ((player_list[current_player].position == 5) || (player_list[current_player].position == 15) || (player_list[current_player].position == 25) || (player_list[current_player].position == 35))
					{
						player_list[current_player].railroads_owned += 1;
					}
					if ((player_list[current_player].position == 12) || (player_list[current_player].position == 28))
					{
						player_list[current_player].utilities_owned += 1;
					}

					player_list[current_player].money -= property_list[player_list[current_player].position].purchase_price;
					player_list[current_player].properties_owned[player_list[current_player].position] = true;
					cout << "You bought " << yellow << property_list[player_list[current_player].position].name << white << "! (Money after purchase: $ " << yellow << player_list[current_player].money << white << ")";
					break;
				case 2:

					break;
				default:
					exit;
				}
			}

			if (player_list[current_player].in_jail == false)
			{
				player_list[current_player].time_in_jail = 0;
			}

			// Bankrupt check
			for (int u = 0; u < number_players; u++)
			{
				if (player_list[u].money < 0)
				{
					cout << endl;
					if (player_list[u].bankrupt == false)
					{
						players_remaining -= 1;
						cout << player_list[u].color << player_list[u].icon << white << red << " has gone bankrupt! (All properties they owned are now able to be bought!)" << white << endl;
						player_list[u].bankrupt = true;
						fString.code_pause(false);
						fString.clear();
						for (int j = 0; j < 40; j++)
						{
							if (player_list[u].properties_owned[j] == true)
							{
								property_list[j].combo = false;
								property_list[j].houses_owned = 0;
								player_list[u].properties_owned[j] = false;
								player_list[u].in_jail = false;
								player_list[u].time_in_jail = 0;
								player_list[u].railroads_owned = 0;
							}

						}
						if (current_player == u)
						{
							return;
						}
					}
				}
			}


			break;
		case 2:


			// Stops the jail counter from counting up when buying houses
			if (player_list[current_player].time_in_jail > 0)
			{
				player_list[current_player].time_in_jail -= 1;
			}


			if ((player_list[current_player].properties_owned[1]) && (player_list[current_player].properties_owned[3]))
			{
				cout << "You can buy houses for the" << dark_cyan << " dark blue properties" << white << endl;
				property_list[1].combo = true;
				property_list[3].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[6]) && (player_list[current_player].properties_owned[8]) && (player_list[current_player].properties_owned[9]))
			{
				cout << "You can buy houses for the" << cyan << " cyan properties" << white << endl;
				property_list[6].combo = true;
				property_list[8].combo = true;
				property_list[9].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[11]) && (player_list[current_player].properties_owned[13]) && (player_list[current_player].properties_owned[14]))
			{
				cout << "You can buy houses for the" << magenta << " magenta properties" << white << endl;
				property_list[11].combo = true;
				property_list[13].combo = true;
				property_list[14].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[16]) && (player_list[current_player].properties_owned[18]) && (player_list[current_player].properties_owned[19]))
			{
				cout << "You can buy houses for the" << dark_yellow << " orange properties" << white << endl;
				property_list[16].combo = true;
				property_list[18].combo = true;
				property_list[19].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[21]) && (player_list[current_player].properties_owned[23]) && (player_list[current_player].properties_owned[24]))
			{
				cout << "You can buy houses for the" << red << " red properties" << white << endl;
				property_list[21].combo = true;
				property_list[23].combo = true;
				property_list[24].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[26]) && (player_list[current_player].properties_owned[27]) && (player_list[current_player].properties_owned[29]))
			{
				cout << "You can buy houses for the" << yellow << " yellow properties" << white << endl;

				property_list[26].combo = true;
				property_list[27].combo = true;
				property_list[29].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[31]) && (player_list[current_player].properties_owned[32]) && (player_list[current_player].properties_owned[34]))
			{
				cout << "You can buy houses for the" << green << " green properties" << white << endl;
				property_list[31].combo = true;
				property_list[32].combo = true;
				property_list[34].combo = true;
				player_list[current_player].combo = true;
			}
			if ((player_list[current_player].properties_owned[37]) && (player_list[current_player].properties_owned[39]))
			{
				cout << "You can buy houses for the" << blue << " blue properties" << white << endl;
				property_list[37].combo = true;
				property_list[39].combo = true;
				player_list[current_player].combo = true;
			}

			if (player_list[current_player].combo == true)
			{
				// If player has a combo

				suffix_space = 30;
				for (int i = 0; i < 40; i++)
				{
					if ((player_list[current_player].properties_owned[i] == true) && (i != 12) && (i != 28) && (i != 5) && (i != 15) && (i != 25) && (i != 35))
					{
						cout << yellow << i << ": " << property_list[i].color << property_list[i].name << white;

						int string_length = (property_list[i].name).length();
						string_length += to_string(i).length();
						for (int k = string_length; k < suffix_space; k++)
						{
							cout << " ";
						}

						cout << "(Houses owned: " << property_list[i].houses_owned << "/5)  " << "(House cost: $ " << property_list[i].house_price << ")" << endl;
					}
				}
				fString.color_fast(yellow, "Select a property to buy houses on: ");
				cout << endl;
				cin >> player_choice;

				if (player_list[current_player].properties_owned[player_choice] == true)
				{
					if (property_list[player_choice].combo == true)
					{
						if (player_list[current_player].money >= property_list[player_choice].house_price)
						{
							// Player has enough money for a house
							cout << "-" << property_list[player_choice].color << property_list[player_choice].name << white << "-" << endl;
							cout << " (House cost: $ " << property_list[player_choice].house_price << ")" << endl;
							cout << " (Houses owned " << property_list[player_choice].houses_owned << "/5)" << endl;
							for (int k = 1; k < 6; k++)
							{
								if (k > 1)
								{
									cout << "Rent with " << k << " houses: $ " << yellow << property_list[player_choice].rent[k] << white << endl;
								}
								else
								{
									cout << "Rent with " << k << " house: $ " << yellow << property_list[player_choice].rent[k] << white << endl;
								}

							}
							fString.color(white, "How many houses would you like to buy?");

							cin >> player_choice_quantity;
							if ((property_list[player_choice].houses_owned + player_choice_quantity) <= 5)
							{
								// Quantity of houses does not exceed 5
								if (player_list[current_player].money >= player_choice_quantity*property_list[player_choice].house_price)
								{
									// Player has enough money for x houses
									cout << "Purchase cost $ " << yellow << property_list[player_choice].house_price * player_choice_quantity << white << " (Money after purchase $ " << yellow << player_list[current_player].money - (property_list[player_choice].house_price * player_choice_quantity) << white << ")" << endl;
									cout << white << "Are you sure you want to buy " << yellow << player_choice_quantity << white << " houses?" << endl;
									cout << yellow << "1. Yes" << white << endl;
									cout << yellow << "2. No" << white << endl;
									cin >> player_choice_confirm;
									switch (player_choice_confirm)
									{
									case 1:
										player_list[current_player].money -= (property_list[player_choice].house_price * player_choice_quantity);
										property_list[player_choice].houses_owned += player_choice_quantity;
										cout << white << "You added " << yellow << player_choice_quantity << white << " houses to " << property_list[player_choice].color << property_list[player_choice].name << white << "!" << white << endl;
										cout << white << "Current amount of houses on this property: " << yellow << property_list[player_choice].houses_owned << "/5" << white << endl;
										break;

									case 2:
										// Player said no to confirmation
										fString.color_fast(red, "You refused the purchase!");
										break;

									default:
										fString.color_fast(red, "Invalid input!");
										break;
									}
								}
								else
								{
									// Player doesn't have enough money for x houses
									fString.color_fast(red, "You don't have enough money!");
								}
							}
							else
							{
								// Quantity of houses is over 5
								fString.color_fast(red, "You can't buy that many houses! (Maximum 5 houses per property)");
								cout << red << "Amount of houses available on this property: " << 5 - property_list[player_choice].houses_owned << white << endl;
							}
						}
						else
						{
							// Player doesn't have enough money for a house
							fString.color_fast(red, "You don't have enough money for a house!");
						}
					}
					else
					{
						// Player cannot buy a house because they don't have the color combo
						fString.color_fast(red, "You don't have a color combo for this property!");
					}
				}
				else
				{
					// Player doesn't own the property
					fString.color_fast(red, "You don't own this property!");
				}
			}
			else
			{
				// No combos
				fString.color_fast(red, "You can't buy any houses with your current properties!");
			}
			break;




		case 3:
			// Trade properties
			if (player_list[current_player].time_in_jail > 0)
			{
				player_list[current_player].time_in_jail -= 1;
			}
			cout << "<------------------------------------->" << endl;
			fString.color_fast(yellow, "Select a player you would like to trade with (Enter their number): ");
			cin >> player_choice;
			player_choice -= 1;
			if (player_choice >= number_players)
			{
				fString.color_fast(red, "Invalid player number!");
				break;
			}
			if (player_choice == current_player)
			{
				fString.color_fast(red, "You can't trade with yourself!");
				break;
			}
			if (player_list[player_choice].bankrupt == true)
			{
				fString.color_fast(red, "That player is bankrupt!");
				break;
			}

			trade_player = player_choice;
			cout << player_list[current_player].color << "Your money: $ " << yellow << player_list[current_player].money << white << endl;
			cout << player_list[current_player].color << "Your available trades:" << white << endl;
			suffix_space = 30;
			for (int i = 0; i < 40; i++)
			{
				if (player_list[current_player].properties_owned[i] == true)
				{
					// Player owns the house
					cout << yellow << i << ": " << property_list[i].color << property_list[i].name << white;
					if (property_list[i].houses_owned > 0)
					{
						// Player owns a house on the property
						int string_length = (property_list[i].name).length();
						string_length += to_string(i).length();
						for (int k = string_length; k < suffix_space; k++)
						{
							cout << " ";
						}
						cout << "(Houses: " << property_list[i].houses_owned << "/5)";
						

					}
					cout << endl;

				}
			}
			cout << yellow << "41:" << white << " Trade money" << endl;


			cout << endl;
			cout << player_list[player_choice].color << player_list[player_choice].icon << " money: $ " << yellow << player_list[player_choice].money << white << endl;
			cout << player_list[player_choice].color << player_list[player_choice].icon << "'s available trades:" << white << endl;
			for (int i = 0; i < 40; i++)
			{
				if (player_list[player_choice].properties_owned[i] == true)
				{
					cout << yellow << i << ": " << property_list[i].color << property_list[i].name << white << endl;

				}
			}
			cout << yellow << "41:" << white << " Trade money" << endl;

			cout << endl;
			fString.color_fast(red, "WARNING: If selling properties with houses- the houses will be sold for half the purchase cost after the trade!");
			fString.color_fast(yellow, "Which properties (or amount of money) would you like to trade away?");
			fString.color_fast(yellow, "OR input 42 to stop selecting your output trades: ");
			while (exit_trade == false)
			{
				cin >> player_choice;
				if ((player_choice <= 40) && (player_choice > 0))
				{
					// Player selected a property
					if (player_list[current_player].properties_owned[player_choice] == true)
					{
						// Player selected a property they own
						cout << "You added " << property_list[player_choice].color << property_list[player_choice].name << white << " to the trade output!" << endl;
						trade_away.push_back(player_choice);
					}
					else
					{
						// Player selected a property they do not own
						fString.color_fast(red, "You don't own this property!");
					}

				}
				else if (player_choice == 41)
				{
					// Player selected trade money option
					fString.color_fast(yellow, "How much money would you like to trade?");
					cin >> player_choice_quantity;
					if (player_list[current_player].money >= player_choice_quantity)
					{
						// Player has enough money for this trade
						cout << "You added $ " << yellow << player_choice_quantity << white << " to the trade output!" << endl;
						trading_money_output = true;
						money_amount_output = player_choice_quantity;
					}
					else
					{
						// Player doesn't have enough money for this trade
						fString.color_fast(red, "You don't have enough money for this trade!");
					}
				}
				else if (player_choice == 42)
				{
					// Player selected finish selecting
					exit_trade = true;
				}
				else if ((player_choice < 0) || (player_choice > 42))
				{
					// Player selects an invalid option
					fString.color_fast(red, "Invalid input!");
				}
				
			}

			// Player has stopped adding properties to give away and has started to select their input trades
			//
			//
			//
			//
			exit_trade = false;
			fString.color_fast(yellow, "Which properties (or amount of money) would you like to recieve?");
			fString.color_fast(yellow, "OR input 42 to stop selecting your input trades: ");
			while (exit_trade == false)
			{
				cin >> player_choice;
				if ((player_choice <= 40) && (player_choice > 0))
				{
					// Player selected a property
					if (player_list[trade_player].properties_owned[player_choice] == true)
					{
						// Player selected a property they own
						cout << "You added " << property_list[player_choice].color << property_list[player_choice].name << white << " to the trade input!" << endl;
						trade_in.push_back(player_choice);
					}
					else
					{
						// Player selected a property they do not own
						fString.color_fast(red, "They don't own this property!");
					}

				}
				else if (player_choice == 41)
				{
					// Player selected trade money option
					fString.color_fast(yellow, "How much money would you like the other player to trade?");
					cin >> player_choice_quantity;
					if (player_list[trade_player].money >= player_choice_quantity)
					{
						// Player has enough money for this trade
						cout << "You added $ " << yellow << player_choice_quantity << white << " to the trade input!" << endl;
						trading_money_input = true;
						money_amount_input = player_choice_quantity;
					}
					else
					{
						// Player doesn't have enough money for this trade
						fString.color_fast(red, "They don't have enough money for this trade!");
					}
				}
				else if (player_choice == 42)
				{
					// Player selected finish selecting
					exit_trade = true;
				}
				else if ((player_choice < 0) || (player_choice > 42))
				{
					// Player selects an invalid option
					fString.color_fast(red, "Invalid input!");
				}

			}

			// Player has selected the properties and money amounts for both players
			suffix_space = 50;
			cout << "<-------------------------------------------------------------------------->" << endl;
			cout << "Trading away: ";
			cout << "                                    ";
			cout << "Trading for: " << endl;

			if (trade_away.size() > trade_in.size())
			{
				larger_number = trade_away.size();
			}
			else
			{
				larger_number = trade_in.size();
			}

			for (int i = 0; i < larger_number; i++)
			{
				int string_length = 0;
				if (i < trade_away.size() && (trade_away.empty() == false))
				{
					cout << property_list[trade_away[i]].color << property_list[trade_away[i]].name << white;
					string_length = property_list[trade_away[i]].name.length();
				}
				if (i < trade_in.size() && (trade_in.empty() == false))
				{
					for (int k = string_length; k < suffix_space; k++)
					{
						cout << " ";
					}
					cout << property_list[trade_in[i]].color << property_list[trade_in[i]].name << white;
				}
				cout << endl;		
			}
			if (trading_money_output == true)
			{
				cout << "$ " << money_amount_output;
			}
			
			if (trading_money_input == true)
			{
				int string_length = to_string(money_amount_output).length() + 2;
				for (int k = string_length; k < suffix_space; k++)
				{
					cout << " ";
				}
				if (trading_money_output == false)
				{
					cout << "   ";
				}
				cout << "$ " << money_amount_input;
			}

			cout << endl << endl;
			cout << white << "Are both players okay with this trade?" << endl;
			cout << yellow << "1. Yes" << white << endl;
			cout << yellow << "2. No" << white << endl;
			cin >> player_choice_confirm;
			switch (player_choice_confirm)
			{
			case 1:
				// Calculate railroad change
				// Simplify this later ? (Spaghetti code)
				if (find(trade_away.begin(), trade_away.end(), 5) != trade_away.end()) 
				{
					player_list[current_player].railroads_owned -= 1;
					player_list[trade_player].railroads_owned += 1;
				}
				if (find(trade_away.begin(), trade_away.end(), 15) != trade_away.end())
				{
					player_list[current_player].railroads_owned -= 1;
					player_list[trade_player].railroads_owned += 1;
				}
				if (find(trade_away.begin(), trade_away.end(), 25) != trade_away.end())
				{
					player_list[current_player].railroads_owned -= 1;
					player_list[trade_player].railroads_owned += 1;
				}
				if (find(trade_away.begin(), trade_away.end(), 35) != trade_away.end())
				{
					player_list[current_player].railroads_owned -= 1;
					player_list[trade_player].railroads_owned += 1;
				}

				// Also do other vector
				if (find(trade_in.begin(), trade_in.end(), 5) != trade_in.end())
				{
					player_list[current_player].railroads_owned += 1;
					player_list[trade_player].railroads_owned -= 1;
				}
				if (find(trade_in.begin(), trade_in.end(), 15) != trade_in.end())
				{
					player_list[current_player].railroads_owned += 1;
					player_list[trade_player].railroads_owned -= 1;
				}
				if (find(trade_in.begin(), trade_in.end(), 25) != trade_in.end())
				{
					player_list[current_player].railroads_owned += 1;
					player_list[trade_player].railroads_owned -= 1;
				}
				if (find(trade_in.begin(), trade_in.end(), 35) != trade_in.end())
				{
					player_list[current_player].railroads_owned += 1;
					player_list[trade_player].railroads_owned -= 1;
				}

				// Do the same for utilities
				if (find(trade_away.begin(), trade_away.end(), 12) != trade_away.end())
				{
					player_list[current_player].utilities_owned -= 1;
					player_list[trade_player].utilities_owned += 1;
				}
				if (find(trade_away.begin(), trade_away.end(), 28) != trade_away.end())
				{
					player_list[current_player].utilities_owned -= 1;
					player_list[trade_player].utilities_owned += 1;
				}
				
				// Other vector
				if (find(trade_in.begin(), trade_in.end(), 12) != trade_in.end())
				{
					player_list[current_player].utilities_owned += 1;
					player_list[trade_player].utilities_owned -= 1;
				}
				if (find(trade_in.begin(), trade_in.end(), 28) != trade_in.end())
				{
					player_list[current_player].utilities_owned += 1;
					player_list[trade_player].utilities_owned -= 1;
				}

				for (int k = 0; k < trade_in.size(); k++)
				{
					property_list[trade_in[k]].combo = false;
					player_list[trade_player].combo = false;
					player_list[trade_player].properties_owned[trade_in[k]] = false;
					player_list[current_player].properties_owned[trade_in[k]] = true;
					cout << player_list[current_player].color << player_list[current_player].icon << white << " now owns " << property_list[trade_in[k]].color << property_list[trade_in[k]].name << white << "!" << endl;
					if (property_list[trade_in[k]].houses_owned > 0)
					{
						for (int j = 0; j < property_list[trade_in[k]].houses_owned; j++)
						{
							house_sell_money_input += ((property_list[trade_in[k]].house_price) / 2);
						}
						property_list[trade_in[k]].houses_owned = 0;
					}
				}

				for (int k = 0; k < trade_away.size(); k++)
				{
					property_list[trade_away[k]].combo = false;
					player_list[current_player].combo = false;
					player_list[trade_player].properties_owned[trade_away[k]] = true;
					player_list[current_player].properties_owned[trade_away[k]] = false;
					cout << player_list[trade_player].color << player_list[trade_player].icon << white << " now owns " << property_list[trade_away[k]].color << property_list[trade_away[k]].name << white << "!" << endl;
					if (property_list[trade_away[k]].houses_owned > 0)
					{
						for (int j = 0; j < property_list[trade_away[k]].houses_owned; j++)
						{
							
							house_sell_money_output += ((property_list[trade_away[k]].house_price) / 2);
						}
						property_list[trade_away[k]].houses_owned = 0;
					}
				}



				if ((trading_money_output == true) || (trading_money_input == true))
				{
					int money_difference_current_player = player_list[current_player].money;
					int money_difference_trade_player = player_list[trade_player].money;
					player_list[current_player].money -= money_amount_output;
					player_list[current_player].money += money_amount_input;
					player_list[trade_player].money += money_amount_output;
					player_list[trade_player].money -= money_amount_input;
					money_difference_current_player = player_list[current_player].money - money_difference_current_player;
					money_difference_trade_player = player_list[trade_player].money - money_difference_trade_player;

					cout << "Your money after trade: $ " << yellow << player_list[current_player].money << white << " (" << dark_yellow << money_difference_current_player << white << ")" << endl;
					cout << player_list[trade_player].color << player_list[trade_player].icon << white <<  " money afer trade: $ " << yellow << player_list[trade_player].money << white << " (" << dark_yellow << money_difference_trade_player << white << ")" << endl;
					
				}

				if ((house_sell_money_input) > 0 || (house_sell_money_output))
				{
					if (house_sell_money_output > 0)
					{
						cout << "Your money gained from selling houses: $ " << yellow << house_sell_money_output << white << endl;
						player_list[current_player].money += house_sell_money_output;
					}
					if (house_sell_money_input > 0)
					{
						cout << player_list[trade_player].color << player_list[trade_player].icon << white << " money gained from selling houses: $ " << yellow << house_sell_money_output << white << endl;
						player_list[trade_player].money += house_sell_money_output;
					}
				}


				break;
			case 2:
				fString.color_fast(red, "The trade was cancelled!");
				break;
			default:
				fString.color_fast(red, "Invalid input!");
				break;
			}






			// End trading option
			break;


		default:
			// This will run when the user inputs an invalid response aka player is a dummy
			if (player_list[current_player].time_in_jail > 0)
			{
				player_list[current_player].time_in_jail -= 1;
			}
			fString.color_fast(red, "Invalid input!");
			break;


		}
		cout << endl;
		fString.code_pause(false);
		fString.clear();

	}
}

int calculate_rent()
{
	// I didn't want to type this more than once
	return property_list[player_list[current_player].position].rent[property_list[player_list[current_player].position].houses_owned];
}

void show_stats()
{
	output_text_functions fString;
	number_calculation_functions fNumber;
	fString.code_pause(true);
	show_board();
	cout << "It is " << player_list[current_player].color << player_list[current_player].icon << "'s" << white << " turn!" << endl;
	cout << yellow << "Money: $ " << player_list[current_player].money << endl;
}
