/*
 * prjsv 2016
 * Marco Antognini
 */

#ifndef INFOSV_CONFIG_HPP
#define INFOSV_CONFIG_HPP

#include <string>
#include "JSON/JSON.hpp"
#include <SFML/Graphics.hpp>

// Define resources location
std::string const RES_LOCATION = "../res/";

// Define the location of the config file
std::string const DEFAULT_CFG = "app.json";

// Define the location of the font file (could be in the config instead of here)
std::string const FONT_LOCATION = RES_LOCATION + "sansation.ttf";


class Config
{
    private:
        j::Value mConfig;
        bool simulation_debug;

    public:
        Config(std::string path);

        // enables / disables debug mode
        void switchDebug();
        bool getDebug();

        // returns read
        j::Value& getJsonRead(){
            return mConfig;
        }

	    j::Value const& getJsonRead() const {
            return mConfig;
        }


        //window
        const double   window_simulation_width;
        const double  window_simulation_height;
        const double  window_stats_height;
        const double window_info_width;
        const std::string  window_title;
        const int window_antialiasing_level;

        // stats
        const double stats_refresh_rate;
        const std::string stats_log_folder = "logs/";
        const std::string stats_log_prefix = "log_";
        const std::string stats_log_header = "# Plot with GNUPLOT : gnuplot -e \"plot for [i=1:6] 'log_0.txt' u i w l title columnheader(i)\"";
        const double stats_upscaling_ratio;

        // debug
        const sf::Color debug_text_color = sf::Color::White;
	    const size_t default_debug_text_size = 20;

        //simulation
        const std::string simulation_world_texture;
	    const std::string simulation_world_debug_texture;
        const int  simulation_world_size;
        const double  simulation_time_factor;
        const sf::Time  simulation_time_max_dt;

        // living entity
        const std::string entity_texture_tracked = "target.png";
        const double entity_energy_critical = 5;

        // plant
        const std::string standard_plant_texture = "standard_plant.png";
//        const double plant_size; // deprecated
        const double plant_growth_surrounding_radius = 600.0;
        const double plant_growth_max_entities_nearby = 10.0;
        const double plant_initial_energy = 30;
        const double plant_max_energy = 300;
        const double plant_growth_factor = 3;
        const bool plant_reproduction;

        //virtuous plant
        const double virtuous_min_efficiency_factor = 1;
        const double virtuous_max_efficiency_factor = 1.3;
        const double virtuous_plant_probability = 0.2;
        const std::string virtuous_plant_texture = "virtuous_plant.png";

        //toxic plant
        const double toxic_plant_probability = 0.1;
        const std::string toxic_plant_texture = "toxic_plant.png";

        //cacti
        const double cacti_dammage = 400;
        const std::string cacti_texture = "cacti.png";

        // animal
        const bool animal_random_walk_low_energy;
//        const double animal_safe_distance = 1000;
        const double animal_meal_retention ;
        const float animal_feed_time;
        const double animal_deceleration;
        const double animal_escape_coeff1;
        const double animal_escape_coeff2;
        const float animal_delivery_time;
        const double animal_avoidance_coeff;
        const double animal_avoidance_threshold;
        const float animal_mating_time;
        const double animal_satiety_min;
        const double animal_satiety_max;
        const double animal_idle_probability;
        const float animal_idle_time_min;
        const float animal_idle_time_max;
        const float animal_base_energy_consumption;
	    const double animal_min_energy;
        const std::string animal_texture_leader = "crown.png";

        // sheep
        const double sheep_max_speed;
        const double sheep_mass;
        const double sheep_energy_loss_factor;
        const double sheep_view_range;
        const double sheep_view_distance;
        const double sheep_random_walk_jitter;
        const double sheep_random_walk_radius;
        const double sheep_random_walk_distance;
        const double sheep_debug_state_position_y;
        const double sheep_debug_state_position_x;
        const double sheep_debug_sex_position_y;
        const double sheep_debug_sex_position_x;
        const double sheep_debug_energy_position_y;
        const double sheep_debug_energy_position_x;
        const sf::Time sheep_longevity;
        const double sheep_energy_initial;
        const double sheep_energy_min_mating_female;
        const double sheep_energy_min_mating_male;
        const int sheep_reproduction_min_children;
        const int sheep_reproduction_max_children;
        const double sheep_reproduction_gestation_time;
        const double sheep_energy_loss_female_per_child;
        const double sheep_energy_loss_mating_male;
        const std::string sheep_texture_leader;
        const std::string sheep_texture_white;
        const std::string sheep_texture_black;
	    const double sheep_adult_size;
        const double sheep_energy_bite;

        // wolf
        const double wolf_max_speed;
        const double wolf_mass;
        const double wolf_energy_loss_factor;
        const double wolf_view_range;
        const double wolf_view_distance;
        const double wolf_random_walk_jitter;
        const double wolf_random_walk_radius;
        const double wolf_random_walk_distance;
        const double wolf_debug_state_position_y;
        const double wolf_debug_state_position_x;
        const double wolf_debug_sex_position_y;
        const double wolf_debug_sex_position_x;
        const double wolf_debug_energy_position_y;
        const double wolf_debug_energy_position_x;
        const sf::Time wolf_longevity;
        const double wolf_energy_initial;
        const double wolf_energy_min_mating_female;
        const double wolf_energy_min_mating_male;
        const int wolf_reproduction_min_children;
        const int wolf_reproduction_max_children;
        const double wolf_reproduction_gestation_time;
        const double wolf_energy_loss_female_per_child;
        const double wolf_energy_loss_mating_male;
        const std::string wolf_texture;
        const double wolf_adult_size;
//        const double wolf_energy_bite;
        const double wolf_idle_probability;
        const float wolf_idle_time_min;
        const float wolf_idle_time_max;
        const double wolf_satiety_min;
        const double wolf_satiety_max;

        // Virus
        const double virus_sparsity; // probability of component in mProfile to be non-zero
        const double virus_initial_quantity; // initial quantity of virus when infection occurs
        const double virus_min_quantity_for_infection; // minimal quantity of virus needed to infect other animals
        // TODO : make it dt-dependent
        const double virus_infection_probability; // probability of infecting an animal in range at each step.
        const std::string virus_texture_infected;
        const double virus_infection_range; // range within which the virus may propagate to another animal
        const double virus_max = 200; // maximal quantity the virus can reach
        const double virus_growth_rate = 1.3; // growth rate of the virus (*dt*saturation term)

        // ImmuneSystem
        const double immune_health_recovery; // speed at which health recovers when virus has been eradicated
        const double immune_health_penalty; // health loss due to virus quantity (*dt)
        const double immune_defense_effectiveness; // virus quantity reduction (*score*dt)
        const double immune_defense_random_variability; // random score added to each component of the score computation
        // -- >baseline mPwr
        const double immune_adaptive_baseline;
        const double immune_adaptive_rate = 0.05; // determines speed at which the immune system adapts (*dt*virus profile)
        const double immune_health_max = 100; // max health of the animals

        // Genome
        const double genome_black_allelle_prevalence;
        const double genome_mutability_innate_immmune_system;

        //Special texture
        const std::string heart_texture = "heart.png";
        const std::string grave_texture = "grave.png";
};

#endif // INFOSV_CONFIG_HPP
