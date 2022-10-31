#include "DataModelV2/DataModelInstance.h"
#include "DataModelV2/LevelInstance.h"

LevelInstance::LevelInstance(void)
{
	Instance::Instance();
	name = "Level";
	className = "LevelService";
	winMessage = "You Won!";
	loseMessage = "You Lost. Try Again";
	timer = 60.0F;
	score = 0;
	canDelete = false;
}

LevelInstance::~LevelInstance(void)
{
}


static TCHAR* strActionType(int option)
{
	switch(option)
	{
		case Enum::ActionType::Nothing:
			return "Nothing";
		case Enum::ActionType::Pause:
			return "Pause";
		case Enum::ActionType::Lose:
			return "Lose";
		case Enum::ActionType::Draw:
			return "Draw";
		case Enum::ActionType::Win:
			return "Win";
	}
	return "Nothing";
}

static Enum::ActionType::Value EnumActionType(TCHAR* option)
{
	if(strcmp("Nothing", option) == 0)
		return Enum::ActionType::Nothing;
	if(strcmp("Pause", option) == 0)
		return Enum::ActionType::Pause;
	if(strcmp("Lose", option) == 0)
		return Enum::ActionType::Lose;
	if(strcmp("Draw", option) == 0)
		return Enum::ActionType::Draw;
	return Enum::ActionType::Win;
}

static TCHAR* strAffectType(int option)
{
	switch(option)
	{
		case Enum::AffectType::NoChange:
			return "NoChange";
		case Enum::AffectType::Increase:
			return "Increase";
		case Enum::AffectType::Decrease:
			return "Decrease";
	}
	return "NoChange";
}

static Enum::AffectType::Value EnumAffectType(TCHAR* option)
{
	if(strcmp("NoChange", option) == 0)
		return Enum::AffectType::NoChange;
	if(strcmp("Increase", option) == 0)
		return Enum::AffectType::Increase;
	return Enum::AffectType::Decrease;
}


char timerTxt[12];
char scoreTxt[12];

PROPERTIES_START(LevelInstance, Instance)
	
	DECLARE_STRING_PROPERTY(
		"Messages",
		"WinMessage",
		"The message that shows when the player wins.",
		winMessage.c_str()
	);
	
	DECLARE_STRING_PROPERTY(
		"Messages",
		"LoseMessage",
		"The message that shows when the player loses.",
		loseMessage.c_str()
	);
	
	DECLARE_BOOL_PROPERTY( // unused?
		"Gameplay",
		"HighScoreIsGood",
		"Some temporary string here",
		HighScoreIsGood
	);
	
	DECLARE_BOOL_PROPERTY( // unused?
		"Gameplay",
		"RunOnOpen",
		"Some temporary string here",
		RunOnOpen
	);

	sprintf_s(timerTxt, "%g", timer);
	sprintf_s(scoreTxt, "%d", score);
	
	DECLARE_STRING_PROPERTY(
		"Gameplay",
		"InitialTimerValue",
		"The amount of time in seconds the player has to complete this level.\r\n\r\nPut 0 if time is limitless.",
		timerTxt
	);
	
	DECLARE_STRING_PROPERTY(
		"Gameplay",
		"InitialScoreValue",
		"The amount of points the player starts with.",
		scoreTxt
	);
	
	DECLARE_ENUM_PROPERTY(
		"Gameplay",
		"TimerUpAction",
		"Some temporary string here",
		strActionType(TimerUpAction),
		"Nothing\0Pause\0Lose\0Draw\0Win\0"
	);
	
	DECLARE_ENUM_PROPERTY(
		"Gameplay",
		"TimerAffectsScore",
		"Some temporary string here",
		strAffectType(TimerAffectsScore),
		"NoChange\0Increase\0Decrease\0"
	);

PROPERTIES_END()

PROP_UPDATE_START(LevelInstance)
	DECLARE_PROP_UPDATE("InitialTimerValue")
		timer = atoi(GET_STRING_CONST());
	DECLARE_PROP_UPDATE_END() // InitialTimerValue
	
	DECLARE_PROP_UPDATE("InitialScoreValue")
		score = atof(GET_STRING_CONST());
	DECLARE_PROP_UPDATE_END() // InitialScoreValue
	
	DECLARE_PROP_UPDATE("LoseMessage")
		loseMessage = GET_STRING_CONST();
	DECLARE_PROP_UPDATE_END() // LoseMessage
	
	DECLARE_PROP_UPDATE("WinMessage")
		winMessage = GET_STRING_CONST();
	DECLARE_PROP_UPDATE_END() // WinMessage
	
	DECLARE_PROP_UPDATE("TimerUpAction")
		TimerUpAction = EnumActionType(GET_ENUM());
	DECLARE_PROP_UPDATE_END() // TimerUpAction

	DECLARE_PROP_UPDATE("TimerAffectsScore")
		TimerAffectsScore = EnumAffectType(GET_ENUM());
	DECLARE_PROP_UPDATE_END() // TimerAffectsScore
	
	DECLARE_PROP_UPDATE("HighScoreIsGood")
		HighScoreIsGood = GET_BOOL() == TRUE;
	DECLARE_PROP_UPDATE_END() // HighScoreIsGood
	
	DECLARE_PROP_UPDATE("RunOnOpen")
		RunOnOpen = GET_BOOL() == TRUE;
	DECLARE_PROP_UPDATE_END() // RunOnOpen
PROP_UPDATE_END(Instance)

void LevelInstance::winCondition()
{
	DataModelInstance* DataModel = (DataModelInstance*)getParent(); //If level parent gets changed to something other than Datamodel it could cause nasty data corruption bugs
	DataModel->setMessage(winMessage);
	DataModel->toggleRun();
}

void LevelInstance::loseCondition()
{
	DataModelInstance* DataModel = (DataModelInstance*)getParent();
	DataModel->setMessage(loseMessage);
	DataModel->toggleRun();
}

void LevelInstance::pauseCondition()
{
	DataModelInstance* DataModel = (DataModelInstance*)getParent();
	DataModel->toggleRun();
}

void LevelInstance::drawCondition()
{
	DataModelInstance* DataModel = (DataModelInstance*)getParent();
	DataModel->toggleRun();
}

void LevelInstance::Step(SimTime sdt)
{
	switch(TimerAffectsScore)
	{
		case Enum::AffectType::NoChange:
			break;
		case Enum::AffectType::Increase:
			score += 1;
			break;
		case Enum::AffectType::Decrease:
			if (score > 0)
				score -= 1;
			break;
	}
	if (timer >= sdt){
		timer -= sdt;
	}
	else{
		timer = 0.0f;
		switch(TimerUpAction)
		{
			case Enum::ActionType::Nothing:
				break;
			case Enum::ActionType::Pause:
				pauseCondition();
				break;
			case Enum::ActionType::Lose:
				loseCondition();
				break;
			case Enum::ActionType::Draw:
				drawCondition();
				break;
			case Enum::ActionType::Win:
				winCondition();
				break;
		}
	}
}