#include "sc2api/sc2_api.h"
#include <libvoxelbot/combat/simulator.h>
#include "sc2utils/sc2_arg_parser.h"
#include "sc2utils/sc2_manage_process.h"
#include "bot_examples.h"
#include <s2client-api\src\sc2api\sc2_game_settings.cc>

#include <iostream>
#include <string>

using namespace sc2;



void play_mcts_game(int game_number, std::string type, bool verbose) {
	initMappings();

	/*
	std::vector<CombatUnit> units = { {
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(5,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(6,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(7,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(8,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(9,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(10,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(11,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(12,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(13,2)),

		makeUnit(2, UNIT_TYPEID::ZERG_ROACH, sc2::Point2D(7,10)),
		makeUnit(2, UNIT_TYPEID::ZERG_ROACH, sc2::Point2D(8,10)),
		makeUnit(2, UNIT_TYPEID::ZERG_ROACH, sc2::Point2D(9,10)),
		makeUnit(2, UNIT_TYPEID::ZERG_ROACH, sc2::Point2D(10,10)),
	} };
	*/

	std::vector<CombatUnit> units = { {
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(5,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(6,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(7,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(8,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(9,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(10,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(11,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(12,2)),
		makeUnit(1, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(13,2)),

		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(5,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(6,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(7,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(8,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(9,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(10,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(11,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(12,10)),
		makeUnit(2, UNIT_TYPEID::TERRAN_MARINE, sc2::Point2D(13,10)),
	} };

	Mcts mcts_player1;
	Mcts mcts_player2;

	int win_1 = 0;
	int win_2 = 0;

	int player = 1;
	int rollout_nb = 5;
	float c = sqrt(2);

	for (int i = 0; i < game_number; i++) {
		std::cout << " ########################## " << std::endl;
		std::cout << " #### BEGNING NEW GAME #### " << std::endl;
		std::cout << " ########################## " << std::endl;

		Game game(units, player);
		std::cout << game.state.toString() << std::endl;

		while (game.isWin() == 0) {
			if (type == "flat") {
				game.executeAction(mcts_player1.flat_mcts(game, rollout_nb, verbose = true));
				if (verbose) {
					std::cout << game.state.toString() << std::endl;
				}
				if (game.isWin() == 0) {
					game.executeAction(mcts_player2.flat_mcts(game, rollout_nb, verbose = true));
					if (verbose) {
						std::cout << game.state.toString() << std::endl;
					}
				}
			}
			else if (type == "uct") {
				game.executeAction(mcts_player1.uct_mcts(game, rollout_nb, c, verbose = true));
				if (verbose) {
					std::cout << game.state.toString() << std::endl;
				}
				if (game.isWin() == 0) {
					game.executeAction(mcts_player2.uct_mcts(game, rollout_nb, c, verbose = true));
					if (verbose) {
						std::cout << game.state.toString() << std::endl;
					}
				}
			}
		}

		if (game.isWin() == 1) win_1++;
		else if (game.isWin() == 2) win_2++;
		std::cout << "player 1 won " << win_1 << " games // player 2 won " << win_2 << " games." << std::endl;
	}
}


//*************************************************************************************************
int main(int argc, char* argv[]) {
    srand(time(NULL));
    initMappings();    

	bool verbose = true;
	play_mcts_game(10,"ucb",verbose);

	// --------------------------------------------------------------------------------- //
	// - BOT PART WHERE THE ACTUAL GAME IS LAUNCHED BUT TOO SLOW TO HAVE A GOOD RESULT - //
	// --------------------------------------------------------------------------------- //
	
	/*
    sc2::Coordinator coordinator;
    if (!coordinator.LoadSettings(argc, argv)) {
        return 1;
    }

    coordinator.SetRealtime(true);

    // Add the custom bot, it will control the player.
    sc2::DefeatRoachesMctsBot bot;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Terran, &bot),
        CreateComputer(sc2::Race::Zerg),
    });

    // Start the game.
    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapDefeatRoaches);

    while (coordinator.Update()) {
        if (sc2::PollKeyPress()) {
            break;
        }
    }
    */
    return 0;
}