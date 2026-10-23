#include "BattleState.hpp"

bool BattleState::lost() const{
	for (auto member : partyMembers) {
		if (member.isAlive() == true) {
			return true;
		}
	}
	return false;
}

bool BattleState::won() const{

}