#pragma once 

#include <map>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "materials.hpp"
#include "config.hpp"

class Config {
public:
	/////////////////////////////////////////////////////////////
	// Singleton pattern ////////////////////////////////////////
	static Config& Instance() 
	{
		static Config instance;
		return instance;
	}

	Config(Config const&) = delete;             // Copy construct
	Config(Config&&) = delete;                  // Move construct
	Config& operator=(Config const&) = delete;  // Copy assign
	Config& operator=(Config &&) = delete;      // Move assign
	/////////////////////////////////////////////////////////////

    bool isPlaying = false;
	bool drawHeatmap = false;

    sf::Vector2i CandleSize = {21, 19};

    std::map<MaterialType, MaterialVars> Materials;

private:
	Config()
    {
        isPlaying = false;
        CandleSize = { 21, 19 };

        Materials = {
            { Air,            { 1.265, 1000, 0.022 } },      
            { Water,          { 1000, 4183, 0.6 } },
            { Iron,           { 3.8 * pow(10,3), 640.57, 92 } },
            { Paraffin,       { 915, 3220, 0.26 } },
            { ParaffinLiquid, { 915, 3220, 0.26 } },
            { Fire,           { 0, 0, 0 } }
        };
    }

	~Config() = default;

};
