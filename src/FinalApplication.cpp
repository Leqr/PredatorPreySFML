/*
 * prjsv 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#include <Config.hpp>
#include <FinalApplication.hpp>
#include <Env/Sheep.hpp>
#include <Env/Wolf.hpp>
#include <Env/VirtuousPlant.hpp>
#include <Env/StandardPlant.hpp>
#include <Env/ToxicPlant.hpp>
#include <Env/Cacti.hpp>
#include <iostream>
#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
    // Setup stats
	Application::onRun();
	addGraph(s::GENERAL, { s::WOLVES, s::SHEEP, s::PLANT, s::INFECTED}, 0, 300);
	addGraph(s::PLANT_INDIVIDUAL, {s::ENERGY}, 0, 1000);
	addGraph(s::ANIMAL_INDIVIDUAL, {s::ENERGY, s::HEALTH, s::VIRUS, s::ADASCORE, s::IMUNAC, s::SCORE}, 0, 800);
	focusOnStat(s::GENERAL);

}

void FinalApplication::onSimulationStart()
{
    // TODO add more stuff here
}

void FinalApplication::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
		//KeyPressed instead of keyRealeased allow adding multiple entities quickly.
        switch (event.key.code){

			case sf::Keyboard::W:
				getAppEnv().addEntity(new Wolf(getCursorPositionInView()));
				break;

			case sf::Keyboard::Num1:
				getAppEnv().addEntity(new Sheep(getCursorPositionInView(), 1));
				break;

			case sf::Keyboard::Num2:
				getAppEnv().addEntity(new Sheep(getCursorPositionInView(), 2));
				break;

			case sf::Keyboard::I:
				getAppEnv().infectEntity(getCursorPositionInView());
				break;

			case sf::Keyboard::F:
				getAppEnv().trackEntity(getCursorPositionInView());
				break;

			case sf::Keyboard::Z:
			    getAppEnv().stopTrackingAnyEntity();
				break;

			case sf::Keyboard::V:
				getAppEnv().addEntity(new VirtuousPlant(getCursorPositionInView()));
				break;

            case sf::Keyboard::N:
                getAppEnv().addEntity(new StandardPlant(getCursorPositionInView()));
                break;

            case sf::Keyboard::T:
                getAppEnv().addEntity(new ToxicPlant(getCursorPositionInView()));
                break;

            case sf::Keyboard::C:
                getAppEnv().addEntity(new Cacti(getCursorPositionInView()));
                break;

			case sf::Keyboard::P:
                getAppEnv().addRandomPlant(getCursorPositionInView());
                break;

			case sf::Keyboard::S:
				getStats().changeStatsScale();
				break;

        default:
            break;
        }
	}
}
