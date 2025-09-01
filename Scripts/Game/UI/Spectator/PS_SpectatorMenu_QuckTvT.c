modded class PS_SpectatorMenu
{
	protected TextWidget m_StepTimerText;
	
	protected PS_GameModeQuickTvT m_GameModeQuick;
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		m_StepTimerText = TextWidget.Cast(GetRootWidget().FindAnyWidget("StepTimerText"));
		m_GameModeQuick = PS_GameModeQuickTvT.Cast(GetGame().GetGameMode());
		GetGame().GetCallqueue().CallLater(UpdateTimer, 0, true);
		UpdateTimer();
	}
	
	void UpdateTimer()
	{
		int time = m_GameModeQuick.GetStepTime() + 999;
		
		int seconds = time/1000;
		int minutes = seconds / 60;
		seconds = Math.Mod(seconds, 60);
		
		m_StepTimerText.SetTextFormat("%1:%2", minutes.ToString(2), seconds.ToString(2));
	}
}