class PS_GameModeQuickTvTClass: PS_GameModeCoopClass
{
};

class PS_GameModeQuickTvT : PS_GameModeCoop
{
	static const string m_QuickTvTConfigFilePath = "$profile:PS_QuickTvT_Config.json";
	protected static ref PS_QuickTvTMissionsConfig m_QuickTvTMissionsConfig;
	
	[Attribute("15000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iPreviewTime;
	
	[Attribute("30000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iSlotsTime;
	
	[Attribute("30000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iBriefingTime;
	
	[Attribute("480000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iGameTime;
	
	[Attribute("12000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iDebriefingTime;
	
	[RplProp()]
	protected int m_iStepTime;
	
	protected static int m_iMissionNum = 0;
	
	protected PlayerManager m_PlayerManager;
	
	int GetStepTime()
	{
		return m_iStepTime;
	}
	
	override void OnGameStart()
	{
		super.OnGameStart();
		
		if (!m_QuickTvTMissionsConfig)
		{
			m_QuickTvTMissionsConfig = new PS_QuickTvTMissionsConfig();
			SCR_JsonLoadContext configLoadContext = new SCR_JsonLoadContext();
			configLoadContext.LoadFromFile(m_QuickTvTConfigFilePath);
			configLoadContext.ReadValue("", m_QuickTvTMissionsConfig);
			
			m_QuickTvTMissionsConfig.SortRandom();
		}
		
		m_PlayerManager = GetGame().GetPlayerManager();
		
		if (m_iGameTime == 0)
		{
			m_iMissionNum = 0;
			ChangeToNextMission();
		}
		
		m_iStepTime = m_iPreviewTime;
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (!Replication.IsServer())
			return;
		
		if (m_iStepTime > 0)
		{
			int playersCount = m_PlayerManager.GetPlayerCount();
			if (GetState() != SCR_EGameModeState.PREGAME || playersCount > 1)
				m_iStepTime -= timeSlice * 1000;
			
			if (m_iStepTime <= 0)
				AdvanceGameState(GetState());
			
			Replication.BumpMe();
		}
	}
	
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
	}
	
	void CheckAlive()
	{
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		
		FactionKey checkFaction = "";
		array<PS_PlayableContainer> playables = playableManager.GetPlayablesSorted();
		foreach (PS_PlayableContainer playable : playables)
		{
			EDamageState damageState = playable.GetDamageState();
			if (damageState == EDamageState.DESTROYED)
				continue;
			
			FactionKey factionKey = playable.GetFactionKey();
			if (checkFaction != "" && checkFaction != factionKey)
			{
				return;
			}
			checkFaction = factionKey;
		}
		
		m_OnOnlyOneFactionAlive.Invoke(checkFaction);
		AdvanceGameState(SCR_EGameModeState.GAME);
		GetGame().GetCallqueue().Remove(CheckAlive);
	}
	
	void ChangeToNextMission()
	{
		int playersCount = m_PlayerManager.GetPlayerCount();
		
		// Get next mission
		m_iMissionNum++;
		if (m_iMissionNum >= m_QuickTvTMissionsConfig.Missions.Count())
		{
			m_iMissionNum = 0;
			m_QuickTvTMissionsConfig.SortRandom();
		}
		PS_QuickTvTMission mission = m_QuickTvTMissionsConfig.Missions[m_iMissionNum];
		
		// skip to next valide
		int i = 0;
		while (mission.MinPlayers > playersCount || mission.MaxPlayers < playersCount)
		{
			i++;
			if (i > 100) break;
			
			m_iMissionNum++;
			if (m_iMissionNum >= m_QuickTvTMissionsConfig.Missions.Count())
			{
				m_iMissionNum = 0;
				m_QuickTvTMissionsConfig.SortRandom();
			}
			
			mission = m_QuickTvTMissionsConfig.Missions[m_iMissionNum];
		}
		GameStateTransitions.RequestScenarioChangeTransition(mission.MissionConfig, "");
	}
};

class PS_QuickTvTMissionsConfig: JsonApiStruct
{
	ref array<ref PS_QuickTvTMission> Missions = {};
	ref RandomGenerator m_RandomGenerator = new RandomGenerator();
	
	void SortRandom()
	{
		array<ref PS_QuickTvTMission> MissionsNew = {};
		foreach (PS_QuickTvTMission mission : Missions)
		{
			MissionsNew.InsertAt(mission, m_RandomGenerator.RandInt(0, MissionsNew.Count()));
		}
		Missions = MissionsNew;
	}
	
	void PS_QuickTvTMissionsConfig()
	{
		RegV("Missions");
	}
}


class PS_QuickTvTMission: JsonApiStruct
{
	string MissionConfig;
	int MinPlayers;
	int MaxPlayers;
	
	void PS_QuickTvTMission()
	{
		RegV("MissionConfig");
		RegV("MinPlayers");
		RegV("MaxPlayers");
	}
}





















