#include "blocks.hpp"

namespace ic_server
{

block::~block()
{

}

dirt::dirt()
	: timeToGrow_(5)
	, elapsed_(0)
	, grown_(false)
{}

void dirt::update(float delta)
{
	elapsed_ += delta;

	if (elapsed_ > timeToGrow_)
		grown_ = true;
}

}
