//------------------------------------------------------------------------------------------------
/*modded class SCR_CampaignBuildingStartUserAction : ScriptedUserAction
{
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_ProviderComponent || m_bTemporarilyBlockedAccess)
			return false;
		*/
		/*if (m_bRankLimitationCanBeUsed && m_ProviderComponent.GetAccessRank() > GetUserRank(user))
		{
			FactionAffiliationComponent factionAffiliationComp = FactionAffiliationComponent.Cast(user.FindComponent(FactionAffiliationComponent));
			if (!factionAffiliationComp)
				return false;
			
			string rankName;
			SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComp.GetAffiliatedFaction());
			if (faction)
				rankName = faction.GetRankName(m_ProviderComponent.GetAccessRank());
				
			SetCannotPerformReason(rankName);
			return false;
		}*/
		
		/*if (m_bAccessCanBeBlocked)
		{
			SetTemporaryBlockedAccess();
		
			if (m_bTemporarilyBlockedAccess)
			{
				SetCannotPerformReason("#AR-Campaign_Action_ShowBuildPreviewEnemyPresence");
				return false;
			}
		}

		return true;
	}
	
}*/
