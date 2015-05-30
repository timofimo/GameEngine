#pragma once

/*utility includes*/
#include <iostream>

class MTLLoader
{
public:
	MTLLoader();
	~MTLLoader();

	static void loadMTL(std::string filepath);
};

