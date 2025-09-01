modded class PS_GameModeHeader
{
	protected TextWidget m_StepTimerText;
	
	protected PS_GameModeQuickTvT m_GameMode;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		m_StepTimerText = TextWidget.Cast(w.FindAnyWidget("StepTimerText"));
		m_GameMode = PS_GameModeQuickTvT.Cast(GetGame().GetGameMode());
		GetGame().GetCallqueue().CallLater(UpdateTimer, 0, true);
		UpdateTimer();
	}
	
	void UpdateTimer()
	{
		if (!m_GameMode)
			return;
		
		int time = m_GameMode.GetStepTime() + 999;
		
		int seconds = time/1000;
		int minutes = seconds / 60;
		seconds = Math.Mod(seconds, 60);
		
		m_StepTimerText.SetTextFormat("%1:%2", minutes.ToString(2), seconds.ToString(2));
	}
}