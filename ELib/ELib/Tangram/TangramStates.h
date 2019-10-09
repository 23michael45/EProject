#ifndef TangramStates_h__
#define TangramStates_h__
#include "Utility/FSM/FSM.h"
class  TangramDetector;

class TangramState : public State<TangramDetector>
{
public:
	TangramState(std::shared_ptr<TangramDetector> sp);;
	~TangramState();

	virtual bool isValidNextState(std::shared_ptr<State<TangramDetector>> state);;

private:

};

class NoBasePieceState : public TangramState
{
public:
	NoBasePieceState(std::shared_ptr<TangramDetector> sp) : TangramState(sp)
	{

	};
	virtual void updateWithDeltaTime(double deltaTime);;
};

class WithBasePieceState : public TangramState
{
public:
	WithBasePieceState(std::shared_ptr<TangramDetector> sp) : TangramState(sp)
	{

	};
	virtual void updateWithDeltaTime(double deltaTime);;
};


#endif // TangramStates_h__
