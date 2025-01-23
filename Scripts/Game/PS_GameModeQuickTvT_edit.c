modded class PS_GameModeQuickTvT : PS_GameModeCoop
{
	/*override void EOnFrame(IEntity owner, float timeSlice)
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
	}*/
	override bool CanJoinFaction(FactionKey factionKeyPlayer, FactionKey currentFaction)
	{
		Print("----m_iFactionsBalance: " + m_iFactionsBalance);
		if (m_iFactionsBalance == -1)
			return true;
		if (factionKeyPlayer == currentFaction)
			return true;
		
		map<FactionKey, int> players = new map<FactionKey, int>();
		map<FactionKey, int> playables = new map<FactionKey, int>();
		map<FactionKey, float> desiredratio = new map<FactionKey, float>();
		array<PS_PlayableContainer> playableComponents = m_playableManager.GetPlayablesSorted();
		
		int playablesammount = 0;
		int factionplayers = 0;
		foreach (PS_PlayableContainer playable : playableComponents)
		{
			playablesammount = playablesammount + 1;
			FactionKey factionKey = playable.GetFactionKey();
			if (!players.Contains(factionKey))
				players[factionKey] = 0;
			if (!playables.Contains(factionKey))
				playables[factionKey] = 0;
			
			playables[factionKey] = playables[factionKey] + 1;
			int playerId = m_playableManager.GetPlayerByPlayable(playable.GetRplId());
			if (playerId > 0)
			{
				players[factionKey] = players[factionKey] + 1;
				Print("----players[factionKey]: " + players[factionKey]);
				factionplayers = factionplayers + 1;
			}
			
		}
		if (currentFaction != "")
			players[currentFaction] = players[currentFaction] - 1;
		
		int playersCount = m_PlayerManager.GetPlayerCount();
		foreach (FactionKey factionKey, int count: playables)
		{	
			Print("----playablesammount: " + playablesammount + " count: " + count);
			desiredratio[factionKey] = count / playablesammount; 
		}
		int adjfactionsbalance = Math.Clamp(m_iFactionsBalance,0,(playersCount / 2));
		Print("----clamp: " + adjfactionsbalance + " (playersCount / 10): " + (playersCount / 10));
		
		float ratio = players[factionKeyPlayer] / (factionplayers + 1 + adjfactionsbalance);
		Print("----ratio " + ratio + " desiredratio[factionKeyPlayer] " + desiredratio[factionKeyPlayer] + " factionplayers: " + factionplayers + " players[factionKeyPlayer]: " + players[factionKeyPlayer]);
		return ratio <= desiredratio[factionKeyPlayer];
		// Scale
		/*int minFaction = 999;
		foreach (FactionKey factionKey, int count: players)
		{
			int scaledCount = players[factionKey] * (maxFaction / playables[factionKey]);
			Print("----minFaction: " + minFaction + " scaledCount:" + scaledCount + "players factionkey: " + players[factionKey] + "(maxFaction / playables[factionKey]): " + (maxFaction / playables[factionKey]));
			if (minFaction > scaledCount)
				minFaction = scaledCount;
		}
		
		int currentCount = players[factionKeyPlayer];
		int diff = currentCount - minFaction;
		Print("----diff: " + diff + " currentCount: " + players[factionKeyPlayer]);
			
		return diff <= m_iFactionsBalance;*/
	}
	
	
};


modded class PS_CharacterSelector : SCR_ButtonComponent
{
	override void OnClicked(SCR_ButtonBaseComponent button)
	{
		if (m_bStateClickSkip)
		{
			m_bStateClickSkip = false;
			return;
		}
		
		int playerId = m_CoopLobby.GetSelectedPlayer();
		if (m_iPlayerId == -2)
		{
			m_CoopLobby.SetPreviewPlayable(m_iPlayableId, true);
			AudioSystem.PlaySound("{C97850E4341F0CF9}Sounds/UI/Samples/Menu/UI_Button_Fail.wav");
			return;
		}
		if (m_iPlayerId > 0 && playerId != m_iPlayerId)
		{
			m_CoopLobby.SetPreviewPlayable(m_iPlayableId, true);
			AudioSystem.PlaySound("{C97850E4341F0CF9}Sounds/UI/Samples/Menu/UI_Button_Fail.wav");
			return;
		}
		PS_PlayableContainer playableContainer = m_PlayableManager.GetPlayableById(m_iPlayableId);
		if (playableContainer.GetDamageState() == EDamageState.DESTROYED)
		{
			m_CoopLobby.SetPreviewPlayable(m_iPlayableId, true);
			AudioSystem.PlaySound("{C97850E4341F0CF9}Sounds/UI/Samples/Menu/UI_Button_Fail.wav");
			return;
		}
	
		SCR_EGameModeState gameState = m_GameModeCoop.GetState();
		if (!PS_PlayersHelper.IsAdminOrServer())
		{
			RplId playableId = m_PlayableManager.GetPlayableByPlayer(m_iCurrentPlayerId);
			if (gameState == SCR_EGameModeState.BRIEFING && playableId != RplId.Invalid())
			{
				m_CoopLobby.SetPreviewPlayable(m_iPlayableId, true);
				return;
			}
		}
		
		
		
		if (playerId != m_iPlayerId)
		{
			if (!CanJoinFaction())
			{
				
				SCR_ChatPanelManager chatPanelManager = SCR_ChatPanelManager.GetInstance();
				ChatCommandInvoker invoker = chatPanelManager.GetCommandInvoker("lmsg");
				invoker.Invoke(null, "Где баланс?");
				SCR_ChatPanelManager.GetInstance().ShowHelpMessage("Соблюдайте баланс сторон");
				m_CoopLobby.SetPreviewPlayable(m_iPlayableId, true);
				return;
			}
			
			AudioSystem.PlaySound("{9500A96BBA3B0581}Sounds/UI/Samples/Menu/UI_Gadget_Select.wav");
			m_PlayableControllerComponent.MoveToVoNRoom(playerId, m_sFactionKey, m_sPlayableCallsign);
			m_PlayableControllerComponent.ChangeFactionKey(playerId, m_sFactionKey);
			m_PlayableControllerComponent.SetPlayerState(playerId, PS_EPlayableControllerState.NotReady);	
			m_PlayableControllerComponent.SetPlayerPlayable(playerId, m_iPlayableId);
		} else {
			AudioSystem.PlaySound("{9500A96BBA3B0581}Sounds/UI/Samples/Menu/UI_Gadget_Select.wav");
			m_PlayableControllerComponent.MoveToVoNRoom(playerId, m_sFactionKey, "#PS-VoNRoom_Faction");
			m_PlayableControllerComponent.ChangeFactionKey(playerId, "");
			m_PlayableControllerComponent.SetPlayerState(playerId, PS_EPlayableControllerState.NotReady);
			m_PlayableControllerComponent.SetPlayerPlayable(playerId, RplId.Invalid());
			if (PS_PlayersHelper.IsAdminOrServer())
				m_PlayableControllerComponent.UnpinPlayer(playerId);
		}
		
		if (PS_PlayersHelper.IsAdminOrServer() && playerId != m_iCurrentPlayerId && gameState == SCR_EGameModeState.GAME)
			m_PlayableControllerComponent.ForceSwitch(playerId);
		if (!PS_PlayersHelper.IsAdminOrServer() && playerId == m_iCurrentPlayerId && gameState == SCR_EGameModeState.BRIEFING)
			m_PlayableControllerComponent.SwitchToMenuServer(SCR_EGameModeState.BRIEFING);
	}
	
	override bool CanJoinFaction()
	{
		// Check faction balance
		PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if (m_PlayableContainer)
		{
			if (!gameModeCoop.CanJoinFaction(m_sFactionKey, m_PlayableManager.GetPlayerFactionKey(m_iCurrentPlayerId)))
				return false;
		}
		return true;
	}
};