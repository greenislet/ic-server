#pragma once

#include "config.hpp"

namespace ic_server
{

class IC_SERVER_API block
{
public:
	virtual ~block();

public:
	virtual void update(float delta) = 0;
};

class IC_SERVER_API dirt : public block
{
private:
	const float timeToGrow_;
	float elapsed_;
	bool grown_;

public:
	dirt();

public:
	void update(float delta) override;
};

}
