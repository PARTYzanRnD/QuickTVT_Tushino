
modded class PS_PlayableManager : ScriptComponent
{	
	
	
	void StartTimeBriefing()
	{
		m_iStartTimerCounter -= 1;
		Replication.BumpMe();
		OnStartTimerCounterChanged();
		Print("-+-timerbrifign: " + m_iStartTimerCounter);
		if (m_iStartTimerCounter == 0)
		{
			PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			gameModeCoop.AdvanceGameState(SCR_EGameModeState.BRIEFING);
			GetGame().GetCallqueue().Remove(StartTimeBriefing);
		}
	}
	
	
	override void SetFactionReady(FactionKey factionKey, int readyValue)
	{
		RPC_SetFactionReady(factionKey, readyValue);
		Rpc(RPC_SetFactionReady, factionKey, readyValue);
		
		if (m_bFactionsReadySended)
			return;
		
		array<int> players = {};
		GetGame().GetPlayerManager().GetPlayers(players);
		bool allFactionsReady = true;
		foreach (int playerId : players)
		{
			factionKey = GetPlayerFactionKey(playerId);
			if (factionKey == "")
				continue;
			if (m_mFactionReady[factionKey])
				continue;
			allFactionsReady = false;
			break;
		}
		if (allFactionsReady)
		{
			m_bFactionsReadySended = true;
			
			SCR_ChatPanelManager chatPanelManager = SCR_ChatPanelManager.GetInstance();
			ChatCommandInvoker invoker = chatPanelManager.GetCommandInvoker("tmsg");
			invoker.Invoke(null, "Factions ready");
			//PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			//SCR_EGameModeState gameModeState = gameModeCoop.GetState();
			//gameModeCoop.AdvanceGameState(SCR_EGameModeState.BRIEFING);
			m_iStartTimerCounter = 3;
			GetGame().GetCallqueue().CallLater(StartTimeBriefing, 1000, true);
			
			///	m_iStartTimerCounter = 3;
			///	Replication.BumpMe();
			///	OnStartTimerCounterChanged();
			///	GetGame().GetCallqueue().CallLater(StartTime, 1000, true);
			
			//gameModeCoop.StartGameMode();
			//PS_GameModeCoop gameMode = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			//gameMode.StartGameMode();
			//AdvanceGameState(GetState());
		}
	}
	/*[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	modded void RPC_SetFactionReady(FactionKey factionKey, int readyValue)
	{
		m_mFactionReady[factionKey] = readyValue;
		m_eFactionReadyChanged.Invoke(factionKey, readyValue);
		PS_GameModeCoop gameMode = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		gameMode.StartGameMode();
	}*/
	
};