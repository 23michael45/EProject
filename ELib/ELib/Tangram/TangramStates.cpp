#include "Tangram/TangramStates.h"
#include "Tangram/TangramDetection.h"

TangramState::TangramState(std::shared_ptr<TangramDetector> sp) : State<TangramDetector>::State(sp)
{

}

TangramState::~TangramState()
{

}

bool TangramState::isValidNextState(std::shared_ptr<State<TangramDetector>> state)
{
	return true;
}

void NoBasePieceState::updateWithDeltaTime(double deltaTime)
{
}

void WithBasePieceState::updateWithDeltaTime(double deltaTime)
{

}
