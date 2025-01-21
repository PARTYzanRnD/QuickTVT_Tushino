/*modded class PS_GameModeQuickTvT : PS_GameModeCoop
{
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		
		PS_GameModeQuickTvT gameModeCoop = PS_GameModeQuickTvT.Cast(GetGame().GetGameMode());
		
		if (!Replication.IsServer())
			return;
		
		if (m_iStepTime > 0)
		{
			int playersCount = m_PlayerManager.GetPlayerCount();
			if (GetState() != SCR_EGameModeState.PREGAME || playersCount > 1)
				m_iStepTime -= timeSlice * 1000;
			
			if (m_iStepTime <= 0)
				Print("playersCount: " + playersCount);
				if (!(gameModeCoop.IsAdminMode() && (playersCount > 10) && ((GetState() == SCR_EGameModeState.SLOTSELECTION) || (GetState() == SCR_EGameModeState.BRIEFING))))
					AdvanceGameState(GetState());
			
			Replication.BumpMe();
		}
	}
	
	
	
};
*/