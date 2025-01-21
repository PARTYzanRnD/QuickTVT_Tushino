modded class PS_GameModeQuickTvT : PS_GameModeCoop
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
	
	/*
	override void OnGameStateChanged()
	{
		super.OnGameStateChanged();
		
		SCR_EGameModeState state = GetState();
		switch (state) 
		{
			case SCR_EGameModeState.PREGAME:
				m_iStepTime = m_iPreviewTime;
				break;
			case SCR_EGameModeState.SLOTSELECTION:
				m_iStepTime = m_iSlotsTime;
				break;
			case SCR_EGameModeState.CUTSCENE:
				break;
			case SCR_EGameModeState.BRIEFING:
				m_iStepTime = m_iBriefingTime;
				break;
			case SCR_EGameModeState.GAME:
				if (Replication.IsServer())
					GetGame().GetCallqueue().CallLater(CheckAlive, 3000, true);
				m_iStepTime = m_iGameTime;
				break;
			case SCR_EGameModeState.DEBRIEFING:
				GetGame().GetCallqueue().Remove(CheckAlive);
				m_iStepTime = m_iDebriefingTime;
				break;
			case SCR_EGameModeState.POSTGAME:
				ChangeToNextMission();
				break;
		}
	}*/
	
};
