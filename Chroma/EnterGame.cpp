#include <Action.h>
#include <VariantSender.h>

void Action::EnterGame(Client cli)
{
	Player* p = cli.GetPlayer();
	VariantSender::OnRequestWorldMenu(p);
	VariantSender::OnConsoleMessage(p, "`$Welcome to `6Chroma Growtopia Server``: Server is currently at `4development ``by our team.");
}